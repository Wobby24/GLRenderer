#include <GLRenderer/OpenGL/Scenes/C1/HelloTriangle.hpp>

namespace GLRenderer
{
    HelloTriangle::HelloTriangle() : VBO_(0), VAO_(0), EBO_(0), isInitialized_(false), isCleaned_(false), isWireframe_(false), fragmentShader(0), vertexShader(0), shaderProgram(0) {}

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
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_); // <-- BIND FIRST

        glGenBuffers(1, &VBO_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices_), tri_vertices_, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //EBO (Element Buffer Object) for indexed drawing
    //	glGenBuffers(1, &EBO_);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

        // Don't unbind EBO here. Let the VAO keep the EBO binding.
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO (not strictly necessary, but good practice)
        glBindVertexArray(0); // optional: unbind VAO
    }

    void HelloTriangle::Update(float deltaTime) {
        (void)deltaTime; // suppress unused parameter warning
    }

    void HelloTriangle::Render() {
        if (!isInitialized_) {
            std::cerr << "TriangleScene not initialized!" << std::endl;
            return;
        }

        if (isCleaned_) {
            std::cerr << "TriangleScene already cleaned up!" << std::endl;
            return;
        }

        if (isWireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe mode
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set fill mode
        }
        //Render the triangle(s)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO_);
        //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle using the vertex data
        glBindVertexArray(0);
    }

    void HelloTriangle::Cleanup() {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
        isCleaned_ = true;
    }
}
