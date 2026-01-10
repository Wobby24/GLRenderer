#include <Utils/Platform/getAboslutePath.hpp>
#include <Renderer/OpenGL/GLModel.hpp>
#include <Renderer/OpenGL/GLTexture.hpp>
#include <Renderer/OpenGL/GLTextureCache.hpp>
#include <filesystem>
#include <algorithm> // for std::replace
namespace fs = std::filesystem;

namespace Renderer {
	namespace OpenGL {
		GLModel::GLModel(const std::string& path) : path_(path), isInit_(false), isCleanedUp_(false) {}

		GLModel::~GLModel() {
			Cleanup();
		}

		void GLModel::Init() {
			loadModel(path_);
			for (auto& mesh : meshes_) {
				mesh.Init();
			}
			isInit_ = true;
		}

		void GLModel::Cleanup() {
			for (auto& mesh : meshes_)
				mesh.Cleanup();
			isCleanedUp_ = true;
		}

		void GLModel::Draw(IShader& shader) {
			for (auto& mesh : meshes_)
				mesh.Draw(shader);
		}

		void GLModel::DrawOptimized(IShader& shader) {
			std::unordered_map<std::shared_ptr<GLMaterial>, std::vector<GLMesh*>> materialGroups;

			for (auto& mesh : meshes_) {
				if (mesh.GetMaterial()) {
					materialGroups[mesh.GetMaterial()].push_back(&mesh);
				}
			}
			GLTextureBinder binder;

			for (auto& [material, groupMeshes] : materialGroups) {
				if (groupMeshes.empty()) continue;

				groupMeshes[0]->BindMaterialTextures(shader, binder);

				for (auto* mesh : groupMeshes) {
					mesh->DrawWithoutBinding();
				}
			}
		}

		void GLModel::loadModel(const std::string& path) {
			Assimp::Importer importer;
			std::string absoluteModelPath = Utils::Platform::getAbsoluteResourcePath(path, "GLRenderer/res/Models");
			const aiScene* scene = importer.ReadFile(absoluteModelPath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs |
				aiProcess_CalcTangentSpace |
				aiProcess_JoinIdenticalVertices |
				aiProcess_ImproveCacheLocality |
				aiProcess_RemoveRedundantMaterials |
				aiProcess_OptimizeMeshes |
				aiProcess_ValidateDataStructure
			);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				throw std::runtime_error("GLModel::loadModel: Assimp error: " + std::string(importer.GetErrorString()));
			}

			directory_ = fs::path(absoluteModelPath).parent_path().string();
			processNode(scene->mRootNode, scene);
		}

		void GLModel::processNode(aiNode* node, const aiScene* scene) {
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes_.push_back(processMesh(mesh, scene));
			}
			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}

		GLMesh GLModel::processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;

				vertex.position = {
					mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z
				};

				vertex.normal = {
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				};

				if (mesh->mTextureCoords[0]) {
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y; // Might need to flip this!
					vertex.texCoords = vec;
				}
				else {
					vertex.texCoords = glm::vec2(0.0f);
				}


				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				const aiFace& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseMaps = loadMaterialTextures(aiMat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
			auto specularMaps = loadMaterialTextures(aiMat, aiTextureType_SPECULAR, TextureType::SPECULAR);
			auto normalMaps = loadMaterialTextures(aiMat, aiTextureType_NORMALS, TextureType::NORMAL);
			auto emissionMaps = loadMaterialTextures(aiMat, aiTextureType_EMISSIVE, TextureType::EMISSION);

			float shininess = 0.0f;
			if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
				if (shininess <= 0.0f) {
					std::cout << "[Warning] Material shininess is 0. Using default of 32.0f\n";
					shininess = 32.0f;
				}
			}
			else {
				std::cout << "[Warning] Shininess not found. Using default of 32.0f\n";
				shininess = 32.0f;
			}

			// Create a unique key for the material based on texture paths
			std::string materialKey;
			for (const auto& tex : diffuseMaps)  materialKey += tex->getFilePath() + "|";
			for (const auto& tex : specularMaps) materialKey += tex->getFilePath() + "|";
			for (const auto& tex : emissionMaps) materialKey += tex->getFilePath() + "|";
			// (You can also include normalMaps if you want)

			std::shared_ptr<GLMaterial> material;

			// Check if the material already exists
			auto it = materialCache_.find(materialKey);
			if (it != materialCache_.end()) {
				material = it->second;  // Reuse existing material
			}
			else {
				// Create new material and cache it
				material = std::make_shared<GLMaterial>();
				material->setDiffuseTextures(diffuseMaps);
				material->setSpecularTextures(specularMaps);
				// material->setNormalTextures(normalMaps); // uncomment if you want to use normal maps
				material->setEmissionTextures(emissionMaps);
				material->setShininess(shininess);

				materialCache_[materialKey] = material;
			}

			return GLMesh(vertices, indices, material);
		}

		std::vector<std::shared_ptr<GLTexture2D>> GLModel::loadMaterialTextures(
			aiMaterial* mat,
			aiTextureType aiType,
			TextureType glType)
		{
			std::vector<std::shared_ptr<GLTexture2D>> textures;

			for (unsigned int i = 0; i < mat->GetTextureCount(aiType); ++i) {
				aiString str;
				mat->GetTexture(aiType, i, &str);

				// 1. Get the raw texture path from Assimp
				std::string textureRelativePath = str.C_Str();

				// 2. Normalize backslashes to forward slashes
				std::replace(textureRelativePath.begin(), textureRelativePath.end(), '\\', '/');

				// 3. Combine model directory (set in loadModel()) and texture path
				fs::path fullTexturePath = fs::path(directory_) / textureRelativePath;

				// 4. Normalize the full path
				fullTexturePath = fs::weakly_canonical(fullTexturePath);

				std::string fullPathStr = fullTexturePath.string();

				try {
					auto texture = GLTextureCache::LoadOrGet(fullPathStr, glType);
					texture->setVertTexFlip(false);

					if (!texture->isLoaded()) {
						texture->loadTexture();
					}

					textures.push_back(texture);
				}
				catch (const std::exception& e) {
					std::cerr << "Failed to load texture: " << fullPathStr << "\nReason: " << e.what() << "\n";
				}
			}

			return textures;
		}
	}
}
