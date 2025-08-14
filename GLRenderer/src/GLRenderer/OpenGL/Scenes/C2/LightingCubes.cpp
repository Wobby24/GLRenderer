#include <GLRenderer/OpenGL/Scenes/C2/LightingCubes.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>

namespace GLRenderer {

	LightingCubes::LightingCubes() : view_(glm::mat4(1.0)), projection_(glm::mat4(1.0)), lightPos_(glm::vec3(1.2f, 1.0f, 2.0f)), lightColor_(glm::vec3(1.0f, 1.0f, 1.0f)), lightingShader_("res/Shaders/Scenes/C2/lighting.vert", "res/Shaders/Scenes/C2/lighting.frag"), lightSourceShader_("res/Shaders/Scenes/C2/lightSource.vert", "res/Shaders/Scenes/C2/lightSource.frag"), copper_("res/Textures/copper.png"), isInitialized_(false), isCleaned_(false), isWireframe_(false), imguiInitialized_(false), windowWidth_(1280), windowHeight_(720) {}

	LightingCubes::~LightingCubes() {
		if (!isInitialized_ || isCleaned_) return;
		Cleanup();
	}

	void LightingCubes::Init() {
        SetupBuffers();
        initResources();
        initCamera();

        //  Set the projection matrix here (at least once!)
        float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
        projection_ = glm::perspective(glm::radians(camera_->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);

        //set flag
        isInitialized_ = true;
	}

    void LightingCubes::initImGUI(GLFWwindow* window) {
        if (imguiInitialized_) {
            // Already initialized, just return early
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, false); //the false is for callbacks, we register our own though, imgui just gets put in them
        ImGui_ImplOpenGL3_Init("#version 330 core");

        imguiInitialized_ = true;
    }

    void LightingCubes::cleanupImGUI() {
        if (!imguiInitialized_) return;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        imguiInitialized_ = false;
    }

    void LightingCubes::renderImGUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

        static bool first_frame = true;
        if (first_frame) {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
            first_frame = false;
        }

        // Apply window styling
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.5f));  // semi-transparent background
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));

        ImGui::Begin("Stats & Settings", nullptr, window_flags);

        // FPs
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Spacing();

        // Wireframe toggle
        ImGui::Checkbox("Wireframe Mode", &isWireframe_);

        // Reload shaders button
        if (ImGui::Button("Reload Shaders")) {
            lightingShader_.reload();
        }

        ImGui::Separator();
        ImGui::Text("Camera Settings");
        ImGui::SliderFloat("Speed", &camera_.get()->getAttributes().movementSpeed, 0.1f, 100.0f);
        ImGui::SliderFloat("Sensitivity", &camera_.get()->getAttributes().mouseSensitivity, 0.01f, 1.0f);
        ImGui::SliderFloat("Zoom", &camera_.get()->getAttributes().zoom, 1.0f, 45.0f);

        ImGui::Separator();
        ImGui::Text("Lighting");

        ImGui::SliderFloat3("Light Position", &pointLight_.getPosition()[0], -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Ambient", &pointLight_.getAmbient()[0], ImGuiColorEditFlags_DisplayRGB);
        ImGui::ColorEdit3("Light Diffuse", &pointLight_.getDiffuse()[0], ImGuiColorEditFlags_DisplayRGB);
        ImGui::ColorEdit3("Light Specular", &pointLight_.getSpecular()[0], ImGuiColorEditFlags_DisplayRGB);

        ImGui::Text("Material Settings");

        ImGui::ColorEdit3("Material Ambient", &copperMat_.getAmbient()[0], ImGuiColorEditFlags_DisplayRGB);
        ImGui::ColorEdit3("Material Diffuse", &copperMat_.getDiffuse()[0], ImGuiColorEditFlags_DisplayRGB);
        ImGui::ColorEdit3("Material Specular", &copperMat_.getSpecular()[0], ImGuiColorEditFlags_DisplayRGB);
        ImGui::SliderFloat("Material Shininess", &copperMat_.getShininess(), 1.0f, 256.0f);

        ImGui::Separator();
        ImGui::Text("Renderer Info");

        const char* cachedRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* cachedVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* cachedVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        ImGui::Text("Window Size: %d x %d", windowWidth_, windowHeight_);
        ImGui::Text("GPU: %s", cachedRenderer);
        ImGui::Text("Vendor: %s", cachedVendor);
        ImGui::Text("GL Version: %s", cachedVersion);

        ImGui::End();

        // Pop styling
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void LightingCubes::initCamera() {
        camera_ = std::make_unique<GLCamera>();
        inputContext_.camera = camera_.get();

        inputHandler_ = std::make_unique<GLSceneInputHandler>(inputContext_);

        auto& attrs = camera_.get()->getAttributes();
        attrs.position = glm::vec3(0.0f, 0.0f, 3.0f);
        attrs.targetPosition = attrs.position;  // This is key!
    } 

    void LightingCubes::initResources() {
        lightingShader_.init();
        lightSourceShader_.init();
        lightingShader_.use();
        copper_.loadTexture();
        lightingShader_.setInt("diffuseTexture", 0);
        //basic material stuff for copper. this will be the copper material based off real life values
        glm::vec3 cpAmbient = glm::vec3(0.19125f, 0.0735f, 0.0225f);
        glm::vec3 cpDiffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f);
        glm::vec3 cpSpecular = glm::vec3(0.256777f, 0.137622f, 0.086014f);
        float cpShininess = 64.0f;
        copperMat_.setProperties(cpAmbient, cpDiffuse, cpSpecular, cpShininess);
        //point light config
        glm::vec3 plPosition(1.2f, 1.0f, 2.0f);
        glm::vec3 plAmbient(0.2f, 0.2f, 0.2f);
        glm::vec3 plDiffuse(0.5f, 0.5f, 0.5f);
        glm::vec3 plSpecular(1.0f, 1.0f, 1.0f);
        pointLight_.setProperties(plPosition, plAmbient, plDiffuse, plSpecular);
    }

   void LightingCubes::SetupBuffers() {
       //we will later need a seperate buffer for the cube and the light source
       //bind the buffers so we can modify them
       cubeMesh_.Bind();
       //upload our data with the vert attrib 'position'
       cubeMesh_.CreateVertices(texNormVertices, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS | GLRenderer::VertexAttribFlags::NORMAL);
       //unbind to avoid unwanted modifications
       cubeMesh_.Unbind();

       lightMesh_.Bind();

       lightMesh_.CreateVertices(cubeVertices_, GLRenderer::VertexAttribFlags::POSITION);

       lightMesh_.Unbind();
    }

   void LightingCubes::SetWindow(Window::IWindow& window) {
       GLFWwindow* native = static_cast<GLFWwindow*>(window.GetNativeHandle());
       if (inputHandler_) {
           inputHandler_->RegisterCallbacks(native);
       }
       initImGUI(native);  // initialize ImGui with the native handle here
   }

   void LightingCubes::Update(float deltaTime) {
       camera_.get()->updatePosition(deltaTime);
       if (inputHandler_) {
           inputHandler_->UpdateInput(deltaTime);
       }
   }

   void LightingCubes::OnWindowResize(int newWidth, int newHeight) {
       // Guard against zero sizes (minimized window)
       if (newWidth == 0 || newHeight == 0) return;

       windowWidth_ = newWidth;
       windowHeight_ = newHeight;
       float aspectRatio = static_cast<float>(windowWidth_) / windowHeight_;
       projection_ = glm::perspective(glm::radians(camera_.get()->getAttributes().zoom), aspectRatio, 0.1f, 100.0f);
   }


   void LightingCubes::Render() {
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

       glPolygonMode(GL_FRONT_AND_BACK, isWireframe_ ? GL_LINE : GL_FILL);

       lightingShader_.use();
       lightingShader_.setMat4("view", view_);
       lightingShader_.setMat4("projection", projection_);
        
       lightingShader_.setVec3("lightPos", lightPos_);
       lightingShader_.setVec3("lightColor", lightColor_);
       lightingShader_.setVec3("viewPos", camera_.get()->getAttributes().position);

       // world transformation
       glm::mat4 model = glm::mat4(1.0f);
       lightingShader_.setMat4("model", model);
       //bind meshbuffer
       cubeMesh_.Bind();
       //bind texture
       copper_.bind(0);
       //set uniforms in shader for material
       copperMat_.applyProperties(lightingShader_);
       //apply point light properties
       pointLight_.applyProperties(lightingShader_);
       //draw
       glDrawArrays(GL_TRIANGLES, 0, 36);
       //unbind mesh 
      cubeMesh_.Unbind();
       //unbind
       copper_.unbind();

       lightMesh_.Bind();

       lightSourceShader_.use();
       lightSourceShader_.setMat4("projection", projection_);
       lightSourceShader_.setMat4("view", view_);
       lightSourceShader_.setVec3("lightColor", pointLight_.getDiffuse());

       model = glm::mat4(1.0f);
       model = glm::translate(model, pointLight_.getPosition());
       model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
       lightSourceShader_.setMat4("model", model);

       glDrawArrays(GL_TRIANGLES, 0, 36);

       lightMesh_.Unbind();

       // Render ImGui UI on top
       renderImGUI();
   }

   void LightingCubes::Cleanup() {
       //cleanup mesh buffer, set is cleaned to true
       cubeMesh_.Cleanup();
       lightMesh_.Cleanup();
       copper_.cleanup();
       cleanupImGUI();
       isCleaned_ = true;
   }
}