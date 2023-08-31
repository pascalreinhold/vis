# Install script for directory: C:/code/lab/third_party/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/lab")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.5-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/code/lab/cmake-build-release-visual-studio/third_party/assimp/lib/assimp-vc143-mt.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/code/lab/third_party/assimp/code/../include/assimp/anim.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/aabb.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ai_assert.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/camera.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/color4.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/color4.inl"
    "C:/code/lab/cmake-build-release-visual-studio/third_party/assimp/code/../include/assimp/config.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ColladaMetaData.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/commonMetaData.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/defs.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/cfileio.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/light.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/material.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/material.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/matrix3x3.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/matrix3x3.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/matrix4x4.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/matrix4x4.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/mesh.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ObjMaterial.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/pbrmaterial.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/GltfMaterial.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/postprocess.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/quaternion.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/quaternion.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/scene.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/metadata.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/texture.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/types.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/vector2.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/vector2.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/vector3.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/vector3.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/version.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/cimport.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/AssertHandler.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/importerdesc.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Importer.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/DefaultLogger.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ProgressHandler.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/IOStream.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/IOSystem.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Logger.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/LogStream.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/NullLogger.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/cexport.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Exporter.hpp"
    "C:/code/lab/third_party/assimp/code/../include/assimp/DefaultIOStream.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/DefaultIOSystem.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SceneCombiner.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/fast_atof.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/qnan.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/BaseImporter.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Hash.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ParsingUtils.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/StreamReader.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/StreamWriter.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/StringComparison.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/StringUtils.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SGSpatialSort.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/GenericProperty.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SpatialSort.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SmallVector.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SmoothingGroups.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/SmoothingGroups.inl"
    "C:/code/lab/third_party/assimp/code/../include/assimp/StandardShapes.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/RemoveComments.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Subdivision.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Vertex.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/LineSplitter.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/TinyFormatter.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Profiler.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/LogAux.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Bitmap.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/XMLTools.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/IOStreamBuffer.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/CreateAnimMesh.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/XmlParser.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/BlobIOSystem.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/MathFunctions.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Exceptional.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/ByteSwapper.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/code/lab/third_party/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Compiler/poppack1.h"
    "C:/code/lab/third_party/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/code/lab/cmake-build-release-visual-studio/third_party/assimp/code/assimp-vc143-mt.pdb")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/code/lab/cmake-build-release-visual-studio/third_party/assimp/code/assimp-vc143-mt.pdb")
  endif()
endif()

