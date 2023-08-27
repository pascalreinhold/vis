namespace lab {

    template<VertexInterface VT>
    void Mesh<VT>::upload(GpuResourceManager& gpu_resource_manager) {
        if (m_uploaded) {
            ErrorLogger::logFatalError("MeshInterface", "Trying to upload mesh that has already been uploaded");
        }
        if(m_vertices.empty()){
            ErrorLogger::logFatalError("MeshInterface", "Trying to upload mesh with no vertices");
        }
        if (m_indices.empty()) {
            ErrorLogger::logFatalError("MeshInterface", "Trying to upload indexed mesh with no indices");
        }

        // stage and create vertex buffer and index buffer to gpu [in the same buffer]
        vk::DeviceSize vertex_buffer_size = vertexCount() * sizeof(VT);
        vk::DeviceSize index_buffer_size = indexCount() * sizeof(uint32_t);

        auto vertex_index_buffer_handle = gpu_resource_manager.createBuffer(
                vertex_buffer_size + index_buffer_size,
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer,
                VMA_MEMORY_USAGE_GPU_ONLY);

        m_vertex_buffer = gpu_resource_manager.createBufferResource(vertex_index_buffer_handle, 0, vertex_buffer_size, {});
        gpu_resource_manager.stageBuffer(m_vertices.data(), vertex_buffer_size, *m_vertex_buffer);
        m_index_buffer = gpu_resource_manager.createBufferResource(vertex_index_buffer_handle, vertex_buffer_size, index_buffer_size, {});
        gpu_resource_manager.stageBuffer(m_indices.data(), index_buffer_size, *m_index_buffer);

        m_uploaded = true;
    }

    template<VertexInterface VT>
    const BufferResource& Mesh<VT>::vertexBuffer() const {
        if (!m_uploaded) {
            ErrorLogger::logFatalError("MeshInterface", "Trying to get vertex buffer handle before uploading");
        }
        return *m_vertex_buffer;
    }

    template<VertexInterface VT>
    void Mesh<VT>::clearCpuMeshData() {
        m_vertices.clear();
        m_indices.clear();
    }

    template<VertexInterface VT>
    const BufferResource& Mesh<VT>::indexBuffer() const {
        if (!m_uploaded) {
            ErrorLogger::logFatalError("MeshInterface", "Trying to get index buffer handle before uploading");
        }
        return *m_index_buffer;
    }

    template<VertexInterface VT>
    Model<VT>::Model(std::filesystem::path path, GpuResourceManager& gpu_resource_manager) {
        Assimp::Importer importer;
        auto flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenSmoothNormals;
        const aiScene *scene = importer.ReadFile((path.string().c_str()), flags); // scene is freed when importer is destroyed
        m_model_directory = path.remove_filename();

        if(!scene || ((scene->mFlags) & AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
            ErrorLogger::logFatalError("Model", importer.GetErrorString());
        }

        processNodeRecursive(scene->mRootNode, scene);

        for(auto& mesh : m_meshes){
            mesh.upload(gpu_resource_manager);
        }

    }

    template<VertexInterface VT>
    void Model<VT>::processNodeRecursive(const aiNode *node, const aiScene *scene) {
        // get all the node's meshes
        for(size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(getMeshFromScene(scene, mesh));
        }

        // recursively process all the children nodes
        for(size_t i = 0; i < node->mNumChildren; ++i) {
            processNodeRecursive(node->mChildren[i], scene);
        }
    }

    template<VertexInterface VT>
    Mesh<VT> Model<VT>::getMeshFromScene(const aiScene *scene, const aiMesh *mesh) {
        Mesh<VT> new_mesh;
        bool has_texture_coords = mesh->HasTextureCoords(0);
        bool has_normals = mesh->HasNormals();

        for(size_t i = 0; i < mesh->mNumVertices; ++i) {
            VT vertex;
            vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            if constexpr (VertexInterfaceWithNormal<VT>) {
                if(has_normals) vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }
            if constexpr (VertexInterfaceWithTexCoord<VT>) {
                if(has_texture_coords) vertex.tex_coord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            new_mesh.m_vertices.push_back(vertex);
        }

        for(size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for(size_t j = 0; j < face.mNumIndices; ++j) {
                new_mesh.m_indices.push_back(face.mIndices[j]);
            }
        }

        if(mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            new_mesh.m_textures.insert(new_mesh.m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            new_mesh.m_textures.insert(new_mesh.m_textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return new_mesh;
    }

    template<VertexInterface VT>
    std::vector<Texture> Model<VT>::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& type_name) {
        std::vector<Texture> textures;

        for(size_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, static_cast<unsigned int>(i), &str);

            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(auto &loaded_texture : m_loaded_textures) {
                if(std::strcmp(loaded_texture.path.data(), str.C_Str()) == 0) {
                    textures.push_back(loaded_texture);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip) {
                Texture texture;
                //texture.id = TextureFromFile(str.C_Str(), this->m_directory);
                texture.type = type_name;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_loaded_textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessarily load duplicate textures.
            }
        }
        return textures;
    }


} // namespace lab