#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GLRenderer/Interface/IModel.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/OpenGL/GLMesh.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>

namespace GLRenderer {
	class GLModel : public IModel {
	public:
		GLModel(const std::string& path);
		~GLModel() override;

		void Init();
		void Cleanup();

		void Draw(IShader& shader) override;
	private:
		// model data
		std::vector<GLMesh> meshes_;
		std::vector<std::shared_ptr<GLTexture2D>> texturesLoaded_;
		std::string directory_;
		std::string path_;

		bool isInit_ = false;
		bool isCleanedUp_ = false;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		GLMesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<GLTexture2D>>
			loadMaterialTextures(
			aiMaterial* mat,
			aiTextureType aiType,
			GLRenderer::TextureType glType);
	};
}