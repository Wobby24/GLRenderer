#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GLRenderer/Interface/IModel.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/OpenGL/GLMesh.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp> // Include this if not already

namespace GLRenderer {

    class GLModel : public IModel {
    public:
        GLModel(const std::string& path);
        ~GLModel() override;

        void Init();
        void Cleanup();

        void Draw(IShader& shader) override;
        void DrawOptimized(IShader& shader);

        // Expose material cache
        const std::unordered_map<std::string, std::shared_ptr<GLMaterial>>& GetMaterialCache() const {
            return materialCache_;
        }

        std::unordered_map<std::string, std::shared_ptr<GLMaterial>>& GetMaterialCache() {
            return materialCache_;
        }

        // Optional utility getters
        const std::string& GetPath() const { return path_; }
        const std::vector<GLMesh>& GetMeshes() const { return meshes_; }

    private:
        std::vector<GLMesh> meshes_;
        std::unordered_map<std::string, std::shared_ptr<GLMaterial>> materialCache_;
        std::string directory_;
        std::string path_;

        bool isInit_ = false;
        bool isCleanedUp_ = false;

        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        GLMesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<GLTexture2D>> loadMaterialTextures(
            aiMaterial* mat,
            aiTextureType aiType,
            TextureType glType
        );
    };

}
