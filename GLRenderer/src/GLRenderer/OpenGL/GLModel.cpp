#include <GLRenderer/OpenGL/GLModel.hpp>

namespace GLRenderer {
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
		for (unsigned int i = 0; i < meshes_.size(); i++)
			meshes_[i].Cleanup();
		isCleanedUp_ = true;
	}

	void GLModel::Draw(IShader& shader) {
		for (unsigned int i = 0; i < meshes_.size(); i++) 
			meshes_[i].Draw(shader);
	}

	void GLModel::loadModel(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
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

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			const std::string Assimp_error = importer.GetErrorString();
			const std::string full_error = "GLModel::loadModel: Assimp ran into an error loading: " + Assimp_error;
			throw std::runtime_error(full_error);

			return;
		}

		directory_ = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void GLModel::processNode(aiNode* node, const aiScene* scene) {
		std::cout << "Processing node with " << node->mNumMeshes << " meshes and " << node->mNumChildren << " children\n";
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::cout << " Processing mesh index: " << node->mMeshes[i] << " with " << mesh->mNumVertices << " vertices\n";
			meshes_.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	GLMesh GLModel::processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<ITexture>> textures;  // Correct type

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			// populate vertex.position, vertex.normal, etc.
			//position
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			//normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			//tex coords
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		//indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		//textures
		   // process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

	   // loadMaterialTextures should now return shared_ptr<ITexture>
		std::vector<std::shared_ptr<ITexture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<std::shared_ptr<ITexture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<std::shared_ptr<ITexture>> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<std::shared_ptr<ITexture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::EMISSION);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


		std::cout << "Loaded " << diffuseMaps.size() << " diffuse textures\n";
		std::cout << "Loaded " << specularMaps.size() << " specular textures\n";
		std::cout << "Loaded " << normalMaps.size() << " normal textures\n";
		std::cout << "Loaded " << heightMaps.size() << " emission textures\n";

		std::cout << "Processed mesh with " << vertices.size() << " vertices, " << indices.size() << " indices, and " << textures.size() << " textures\n";


		return GLMesh(vertices, indices, textures);  // Now matches constructor
	}

	std::vector<std::shared_ptr<ITexture>> GLModel::loadMaterialTextures(
		aiMaterial* mat,
		aiTextureType aiType,
		TextureType glType)
	{
		std::vector<std::shared_ptr<ITexture>> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(aiType); ++i)
		{
			aiString str;
			mat->GetTexture(aiType, i, &str);
			std::string texturePath = directory_ + "/" + std::string(str.C_Str());

			// Deduplication check
			bool skip = false;
			for (const auto& loadedTex : texturesLoaded_)
			{
				auto glTex = std::dynamic_pointer_cast<GLRenderer::GLTexture2D>(loadedTex);
				if (glTex && glTex->getFilePath() == texturePath)
				{
					textures.push_back(loadedTex);
					skip = true;
					break;
				}
			}

			// Not loaded yet — create it
			if (!skip)
			{
				auto texture = std::make_shared<GLRenderer::GLTexture2D>(texturePath, glType);
				try {
					texture->loadTexture();
					textures.push_back(texture);
					texturesLoaded_.push_back(texture);
				}
				catch (const std::exception& e) {
					std::cerr << "Failed to load texture at path: " << texturePath << "\n"
						<< "Reason: " << e.what() << std::endl;
				}
			}
		}

		return textures;
	}
}