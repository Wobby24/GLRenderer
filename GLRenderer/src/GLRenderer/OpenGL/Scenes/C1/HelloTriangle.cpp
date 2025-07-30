#include <GLRenderer/OpenGL/Scenes/C1/HelloTriangle.hpp>

namespace GLRenderer
{
    HelloTriangle::HelloTriangle() : isInitialized_(false), isCleaned_(false), isWireframe_(false), fragmentShader_(0), vertexShader_(0), shaderProgram_(0) {}

    HelloTriangle::~HelloTriangle() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
    }

    void HelloTriangle::Init() {
        SetupBuffers();
        SetupShaders();
        isInitialized_ = true;
    }

    void HelloTriangle::SetupShaders() {
        SetupVertexShader();
        SetupFragmentShader();
        SetupShaderProgram();
    }

    void HelloTriangle::SetupVertexShader() {
        //create vertex shader
        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
        //attach the shader source code to the shader object & compile vertex shader 
        glShaderSource(vertexShader_, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader_);
        //check if it compiled successfully
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
        // If compilation failed, retrieve the error log
        if (!success)
        {
            glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void HelloTriangle::SetupFragmentShader() {
        // Create fragment shader
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
        //attach the shader source code to the shader object & compile fragment shader
        glShaderSource(fragmentShader_, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader_);
        //check if it compiled successfully
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
        // If compilation failed, retrieve the error log
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void HelloTriangle::SetupShaderProgram() {
        //Create shader program and link shaders to it
        shaderProgram_ = glCreateProgram();
        //attach shaders to the program
        glAttachShader(shaderProgram_, vertexShader_);
        glAttachShader(shaderProgram_, fragmentShader_);
        glLinkProgram(shaderProgram_);
        //check if it linked successfully
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        //use the shader program
        glUseProgram(shaderProgram_);
        //no longer need the shader objects, so delete them
        glDeleteShader(vertexShader_);
        glDeleteShader(fragmentShader_);
    }

    void HelloTriangle::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateVertices(triVerts_, GLRenderer::POSITION | GLRenderer::COLOR);
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
        //allow wireframe toggle
        if (isWireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe mode
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set fill mode
        }
        //Render the triangle(s)
        glUseProgram(shaderProgram_);
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
