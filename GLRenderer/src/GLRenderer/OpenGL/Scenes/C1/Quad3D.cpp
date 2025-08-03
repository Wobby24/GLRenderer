#include <GLRenderer/OpenGL/Scenes/C1/Quad3D.hpp>
#include <GLRenderer//Interface/Types/VertexAttribFlagsOperators.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace GLRenderer {
    Quad3D::Quad3D() : isInitialized_(false), isCleaned_(false), isWireframe_(false), mainShader_("res/Shaders/Scenes/C1/3D.vert", "res/Shaders/Scenes/C1/texture.frag"), texture_("res/Textures/container.jpg"), texture2_("res/Textures/awesomeface.png"), trans(glm::mat4(1.0f)) {}

    Quad3D::~Quad3D() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
    }

    void Quad3D::Init() {
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

    void Quad3D::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateAll(vertexData_, vIndices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS);
        //unbind to avoid unwanted modifications
        meshBuffer_.Unbind();
    }

    void Quad3D::Update(float deltaTime) {
        // Assuming 'trans' is a member glm::mat4 you want to update each frame:
        trans = glm::mat4(1.0f); // reset to identity

        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

        // Use deltaTime to increment rotation angle smoothly
        static float rotationAngle = 0.0f;
        rotationAngle += glm::radians(90.0f) * deltaTime; // rotate 90 degrees per second

        trans = glm::rotate(trans, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    }


    void Quad3D::Render() {
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
        //allow wireframe toggle
        if (isWireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe mode
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set fill mode
        }

        //set it in the shader
        mainShader_.use();
        glUniformMatrix4fv(mainShader_.getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(trans));

        /*Render the quad*/ 

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

    void Quad3D::Cleanup() {
        //cleanup mesh buffer, set is cleaned to true
        meshBuffer_.Cleanup();
        texture_.cleanup();
        texture2_.cleanup();
        isCleaned_ = true;
    }
};

