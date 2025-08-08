#include <GLRenderer/OpenGL/Scenes/C1/Quad3D.hpp>
#include <GLRenderer//Interface/Types/VertexAttribFlagsOperators.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace GLRenderer {
    Quad3D::Quad3D() : view(glm::mat4(1.0)), projection(glm::mat4(1.0)), mainShader_("res/Shaders/Scenes/C1/3D.vert", "res/Shaders/Scenes/C1/texture.frag"), texture_("res/Textures/container.jpg"), texture2_("res/Textures/awesomeface.png"), isInitialized_(false), isCleaned_(false), isWireframe_(false), windowWidth_(1280), windowHeight_(720) {}

    Quad3D::~Quad3D() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
    }

    void Quad3D::Init() {
        SetupBuffers();
        initResources();
        initCamera();

        //  Set the projection matrix here (at least once!)
        float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
        projection = glm::perspective(glm::radians(camera_->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);
         
        //set flag
        isInitialized_ = true;
    }

    void Quad3D::initCamera() {
        camera_ = std::make_unique<GLCamera>();
        inputContext_.camera = camera_.get();

        inputHandler_ = std::make_unique<GLSceneInputHandler>(inputContext_);

        auto& attrs = camera_.get()->getAttributes();
        attrs.position = glm::vec3(0.0f, 0.0f, 3.0f);
        attrs.targetPosition = attrs.position;  // This is key!
    }

    void Quad3D::initResources() {
        mainShader_.init();
        //load textures
        texture_.loadTexture();
        texture2_.loadTexture();
        //set the textures in teh shader
        mainShader_.use();
        mainShader_.setInt("texture1", 0);
        mainShader_.setInt("texture2", 1);
        //set cube angles
        cubeAngles_.resize(10, 0.0f); // Start all angles at 0
    }

    void Quad3D::SetupBuffers() {
        //bind the buffers so we can modify them
        meshBuffer_.Bind();
        //upload our data with the vert attrib 'position'
        meshBuffer_.CreateVertices(cubeVertices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS);
        //unbind to avoid unwanted modifications
        meshBuffer_.Unbind();
    }

    // Quad3D.cpp
    void Quad3D::SetWindow(Window::IWindow& window) {
        if (inputHandler_) {
            GLFWwindow* native = static_cast<GLFWwindow*>(window.GetNativeHandle());
            inputHandler_->RegisterCallbacks(native);
        }
    }


    void Quad3D::Update(float deltaTime) {
        camera_.get()->updatePosition(deltaTime);
        if (inputHandler_) {
            inputHandler_->UpdateInput(deltaTime);
        }
    }

    void Quad3D::OnWindowResize(int newWidth, int newHeight) {
        // Guard against zero sizes (minimized window)
        if (newWidth == 0 || newHeight == 0) return;

        windowWidth_ = newWidth;
        windowHeight_ = newHeight;
        float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
        projection = glm::perspective(glm::radians(camera_.get()->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);
    }

    void Quad3D::Render() {
        if (!isInitialized_) {
            std::cerr << "Quad3D Scene not initialized!" << std::endl;
            return;
        }
        if (isCleaned_) {
            std::cerr << "Quad3D Scene already cleaned up!" << std::endl;
            return;
        }

        view = camera_.get()->getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Wireframe toggle
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe_ ? GL_LINE : GL_FILL);

        mainShader_.use();
        mainShader_.setMat4("view", view);
        mainShader_.setMat4("projection", projection);

        // Bind textures
        texture_.bind(0);
        texture2_.bind(1);

        // Bind mesh
        meshBuffer_.Bind();

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            mainShader_.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        meshBuffer_.Unbind(); // Optional: Unbind if needed
    }

    void Quad3D::Cleanup() {
        //cleanup mesh buffer, set is cleaned to true
        meshBuffer_.Cleanup();
        texture_.cleanup();
        texture2_.cleanup();
        isCleaned_ = true;
    }
};

