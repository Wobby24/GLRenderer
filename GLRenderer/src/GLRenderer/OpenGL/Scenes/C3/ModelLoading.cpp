#include <GLRenderer/OpenGL/Scenes/C3/ModelLoading.hpp>
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLDirLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLSpotLight.hpp>

namespace GLRenderer {

    ModelLoading::ModelLoading() : view_(glm::mat4(1.0)), projection_(glm::mat4(1.0)), lightingShader_("Scenes/C3/modelLoading.vert", "Scenes/C3/modelLoading.frag"), lightSourceShader_("Scenes/C2/lightSource.vert", "Scenes/C2/lightSource.frag"), isInitialized_(false), isCleaned_(false), isWireframe_(false), imguiInitialized_(false), isPointerLocked_(true), showExitConfirmDialog(false), windowWidth_(1280), windowHeight_(720), nextModelID_(0){}

    ModelLoading::~ModelLoading() {
        if (!isInitialized_ || isCleaned_) return;
        Cleanup();
    }

    void ModelLoading::Init() {

        initResources();
        initCamera();
        SetupBuffers();

        //  Set the projection matrix here (at least once!)
        float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
        projection_ = glm::perspective(glm::radians(camera_->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);

        //set flag
        isInitialized_ = true;
    }


    void ModelLoading::initCamera() {
        camera_ = std::make_unique<GLCamera>();
        inputContext_.camera = camera_.get();

        inputHandler_ = std::make_unique<GLSceneInputHandler>(inputContext_);

        auto& attrs = camera_.get()->getAttributes();
        attrs.position = glm::vec3(0.0f, 0.0f, 3.0f);
        attrs.targetPosition = attrs.position;  
    }

    void ModelLoading::initResources() {
        lightManager_ = std::make_unique<GLLightManager>();

        lightManager_->Init();
        lightingShader_.init();
        lightSourceShader_.init();

        lightingShader_.use();
    }

    void ModelLoading::SetupBuffers() {
        lightMesh_.Bind();
        lightMesh_.CreateVertices(cubeVertices_, GLRenderer::VertexAttribFlags::POSITION);
        lightMesh_.Unbind();
    }

    void ModelLoading::SetWindow(Window::IWindow& window) {
        GLFWwindow* native = static_cast<GLFWwindow*>(window.GetNativeHandle());
        isPointerLocked_ = glfwGetInputMode(native, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

        if (inputHandler_) {
            inputHandler_->RegisterCallbacks(native);
        }
        initImGUI(native);  // initialize ImGui with the native handle here
    }

    void ModelLoading::Update(float deltaTime) {
        camera_.get()->updatePosition(deltaTime);
        if (inputHandler_) {
            inputHandler_->UpdateInput(deltaTime);
        }
    }

    void ModelLoading::OnWindowResize(int newWidth, int newHeight) {
        // Guard against zero sizes (minimized window)
        if (newWidth == 0 || newHeight == 0) return;

        windowWidth_ = newWidth;
        windowHeight_ = newHeight;
        float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
        projection_ = glm::perspective(glm::radians(camera_.get()->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);
    }


    void ModelLoading::Render() {
        if (!isInitialized_) {
            std::cerr << "LightingCubes Scene not initialized!" << std::endl;
            return;
        }
        if (isCleaned_) {
            std::cerr << "LightingCubes Scene already cleaned up!" << std::endl;
            return;
        }

        view_ = camera_.get()->getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (isWireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        lightingShader_.use();
        lightingShader_.setMat4("view", view_);
        lightingShader_.setMat4("projection", projection_);

        lightingShader_.setInt("numPointLights", lightManager_->GetNumLights(LightType::Point));
        lightingShader_.setInt("numSpotLights", lightManager_->GetNumLights(LightType::Spot));
        lightingShader_.setInt("numDirLights", lightManager_->GetNumLights(LightType::Directional));

        lightingShader_.setVec3("viewPos", camera_.get()->getAttributes().position);

        for (auto& [id, model] : transformableModels_) {
            if (!model) continue;

            model->Draw(lightingShader_); // Assumes model has a Draw() method
        }

        lightMesh_.Bind();

        lightSourceShader_.use();
        lightSourceShader_.setMat4("projection", projection_); 
        lightSourceShader_.setMat4("view", view_);

        glm::mat4 model = glm::mat4(1.0);

        for (const auto& [id, light] : lightManager_->GetAllLights()) {
            if (light->getType() != LightType::Point)
                continue;

            auto pointLight = std::dynamic_pointer_cast<GLRenderer::GLPointLight>(light);
            if (!pointLight) continue;

            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLight->getPosition());
            model = glm::scale(model, glm::vec3(0.2f));

            lightSourceShader_.setMat4("model", model);
            lightSourceShader_.setVec3("lightColor", pointLight->getDiffuse());

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightMesh_.Unbind();

        // Render ImGui UI on top
        renderImGUI();
    }

    void ModelLoading::Cleanup() {

        std::vector<int> toRemove;
        for (const auto& [id, model] : transformableModels_) {
            if (!model) toRemove.push_back(id);
        }
        for (int id : toRemove) {
            transformableModels_.erase(id);
        }

        lightMesh_.Cleanup();
        cleanupImGUI();
        isCleaned_ = true;
    }
}