#include <GLRenderer/OpenGL/Scenes/C1/HelloTriangle.hpp>
#include <GLRenderer//Interface/Types/VertexAttribFlagsOperators.hpp>

namespace GLRenderer
{
    HelloTriangle::HelloTriangle() :  mainShader_("res/Shaders/Scenes/C1/main.vert", "res/Shaders/Scenes/C1/main.frag"), isInitialized_(false), isCleaned_(false), isWireframe_(false) {}

    HelloTriangle::~HelloTriangle() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
    }

    void HelloTriangle::Init() {
        SetupBuffers();
        //make sure to always init out shader before using it!!! an exception WILL be thrown, plus we can't really do anything with it if we haven't created our shaders internally
        mainShader_.init();
        isInitialized_ = true;
    }


    void HelloTriangle::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateVertices(triVerts_, GLRenderer::VertexAttribFlags::POSITION);
        //unbind to avoid unwanted modifications
        meshBuffer_.Unbind();
    }

    void HelloTriangle::Update(float deltaTime) {
        (void)deltaTime; // suppress unused parameter warning
    }

    void HelloTriangle::Render() {
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
        //Render the triangle(s)
        mainShader_.use();
        //bind the mesh for drawing
        meshBuffer_.Bind();
        //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for drawing 2 trianges with indices
        //draw triange
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle using the vertex data
        //unbind
        meshBuffer_.Unbind();
    }

    void HelloTriangle::Cleanup() {
        //cleanup mesh buffer, set is cleaned to true
        meshBuffer_.Cleanup();
        isCleaned_ = true;
    }
}
