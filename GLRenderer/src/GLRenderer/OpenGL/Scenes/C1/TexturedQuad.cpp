#include <GLRenderer/OpenGL/Scenes/C1/TexturedQuad.hpp>
#include <GLRenderer//Interface/Types/VertexAttribFlagsOperators.hpp>

namespace GLRenderer {
    TexturedQuad::TexturedQuad() : mainShader_("res/Shaders/Scenes/C1/texture.vert", "res/Shaders/Scenes/C1/texture.frag"), texture_("res/Textures/container.jpg"), texture2_("res/Textures/awesomeface.png"), isInitialized_(false), isCleaned_(false), isWireframe_(false) {}

    TexturedQuad::~TexturedQuad() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
	}

	void TexturedQuad::Init() {
		SetupBuffers();
		mainShader_.init();
        //load textures
        texture_.loadTexture();
        texture2_.loadTexture();
        //set the textures in teh shader
        mainShader_.use();
        mainShader_.setInt("texture1", 0);
        mainShader_.setInt("texture2", 1);
        //set flag
        isInitialized_ = true;
	}

    void TexturedQuad::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateAll(vertexData_, vIndices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS);
        //unbind to avoid unwanted modifications
        meshBuffer_.Unbind();
    }

	void TexturedQuad::Update(float deltaTime) {
		(void)deltaTime; // suppress unused parameter warning
	}

	void TexturedQuad::Render() {
        //only render if we are initialized
        if (!isInitialized_) {
            std::cerr << "TriangleScene not initialized!" << std::endl;
            return;
        }
        //only render if we haven't cleaned up yet
        if (isCleaned_) {
            std::cerr << "TriangleScene already cleaned up!" << std::endl;
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //allow wireframe toggle
        if (isWireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe mode
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set fill mode
        }
        // Render the quad
        mainShader_.use();
        //bind texture
        texture_.bind(0);
        texture2_.bind(1);
        // bind the mesh for drawing
        meshBuffer_.Bind();
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for drawing 2 trianges with indices
        // draw triangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // unbind
        meshBuffer_.Unbind();
	}

    void TexturedQuad::Cleanup() {
        //cleanup mesh buffer, set is cleaned to true
        meshBuffer_.Cleanup();
        texture_.cleanup();
        isCleaned_ = true;
    }
};

