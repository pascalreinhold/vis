#pragma once

#include "math.hpp"
#include "gpu_resource_manager.hpp"
#include "error_logger.hpp"

#include <filesystem>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace lab {

    // Forward declaration
    template<VertexInterface VT> class Model;

    struct Texture {
        size_t id = 0;
        std::string type;
        std::string path;  // we store the path of the texture to compare with other textures
    };

    template <VertexInterface VT>
    class Mesh {
    public:
        Mesh() = default;
        Mesh(std::vector<uint32_t> indices, std::vector<VT> vertices)
                : m_indices(std::move(indices)), m_vertices(std::move(vertices)) {}

        void upload(GpuResourceManager& gpu_resource_manager);
        void clearCpuMeshData();
        [[nodiscard]] size_t indexCount() const { return m_indices.size(); }
        [[nodiscard]] size_t vertexCount() const { return m_vertices.size(); }
        [[nodiscard]] const BufferResource& indexBuffer() const;
        [[nodiscard]] const BufferResource& vertexBuffer() const;
    protected:
        std::unique_ptr<BufferResource> m_index_buffer;
        std::unique_ptr<BufferResource> m_vertex_buffer;
        std::vector<uint32_t> m_indices;
        std::vector<VT> m_vertices;
        bool m_uploaded = false;

        std::vector<Texture> m_textures;
    friend Model<VT>;
    };



    template <VertexInterface VT>
    class Model {
    public:
        explicit Model(std::filesystem::path path, GpuResourceManager& gpu_resource_manager);
        ~Model() = default;

        [[nodiscard]] const std::vector<Mesh<VT>>& meshes() const { return m_meshes; }
    private:
        void processNodeRecursive(const aiNode *node, const aiScene *scene);
        [[nodiscard]] std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& type_name);
        [[nodiscard]] Mesh<VT> getMeshFromScene(const aiScene* scene, const aiMesh *mesh); // might add textures to m_loaded_textures
        std::filesystem::path m_model_directory;
        std::vector<Texture> m_loaded_textures;
        std::vector<Mesh<VT>> m_meshes;
    };





} // namespace lab

# include "templates/model.tpp"