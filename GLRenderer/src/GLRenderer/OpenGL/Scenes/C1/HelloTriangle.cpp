#include <GLRenderer/OpenGL/Scenes/C1/HelloTriangle.hpp>

namespace GLRenderer
{
    HelloTriangle::HelloTriangle() : isInitialized_(false), isCleaned_(false), isWireframe_(false), fragmentShader(0), vertexShader(0), shaderProgram(0) {}

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
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        //attach the shader source code to the shader object & compile vertex shader 
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        //check if it compiled successfully
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        // If compilation failed, retrieve the error log
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void HelloTriangle::SetupFragmentShader() {
        // Create fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        //attach the shader source code to the shader object & compile fragment shader
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        //check if it compiled successfully
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        // If compilation failed, retrieve the error log
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void HelloTriangle::SetupShaderProgram() {
        //Create shader program and link shaders to it
        shaderProgram = glCreateProgram();
        //attach shaders to the program
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        //check if it linked successfully
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        //use the shader program
        glUseProgram(shaderProgram);
        //no longer need the shader objects, so delete them
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void HelloTriangle::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateVertices(triVerts_, GLRenderer::POSITION);
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
        glUseProgram(shaderProgram);
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
