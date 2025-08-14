#include <GLRenderer/OpenGL/Scenes/C2/LightingCubes.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>

namespace GLRenderer {

	LightingCubes::LightingCubes() : view_(glm::mat4(1.0)), projection_(glm::mat4(1.0)), lightPos_(glm::vec3(1.2f, 1.0f, 2.0f)), lightColor_(glm::vec3(1.0f, 1.0f, 1.0f)), lightingShader_("res/Shaders/Scenes/C2/lighting.vert", "res/Shaders/Scenes/C2/lighting.frag"), lightSourceShader_("res/Shaders/Scenes/C2/lightSource.vert", "res/Shaders/Scenes/C2/lightSource.frag"), containerDiffuse_("res/Textures/container2.png"), containerSpecular_("res/Textures/container2_specular.png"), containerEmission_("res/Textures/matrix.jpg"), isInitialized_(false), isCleaned_(false), isWireframe_(false), imguiInitialized_(false), isPointerLocked_(true), windowWidth_(1280), windowHeight_(720) {}

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

        if (isPointerLocked_) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Pointer Locked");
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Pointer Not Locked");
        }

        //explain 

        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Pointer Lock hides and confines the cursor to the window.\n"
                "It's commonly used in 3D apps for camera control.");
            ImGui::EndTooltip();
        }

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
        // Small square preview size
        ImVec2 texSize(128, 128);

        // Track how much space is left on the current line
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float availWidth = ImGui::GetContentRegionAvail().x;

        // Reset tracker
        float usedWidth = 0.0f;

        auto showTexture = [&](const char* label, std::shared_ptr<const GLRenderer::GLTexture2D> texture) {
            if (!texture) return;

            ImGui::BeginGroup();  // Group label and image together
            ImGui::Text("%s", label);
            ImGui::Image((void*)(intptr_t)texture->getID(), texSize);
            ImGui::EndGroup();

            usedWidth += texSize.x + spacing;

            // If there’s still room on the same line, continue
            if (usedWidth + texSize.x <= availWidth)
                ImGui::SameLine();
            else
                usedWidth = 0.0f;  // Reset when wrapping
            };

        // Render each texture (if it exists)
        showTexture("Diffuse Map", containerMat_.getDiffuseTexture());
        showTexture("Specular Map", containerMat_.getSpecularTexture());
        showTexture("Emissive Map", containerMat_.getEmissiveTexture());

        ImGui::SliderFloat("Material Shininess", &containerMat_.getShininess(), 1.0f, 256.0f);

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
        //load textures
        containerDiffuse_.loadTexture();
        containerSpecular_.loadTexture();
        containerEmission_.loadTexture();
        //setup material values
        containerMat_.setupProperties(std::make_shared<GLTexture2D>(containerDiffuse_), std::make_shared<GLTexture2D>(containerSpecular_), std::make_shared<GLTexture2D>(containerEmission_));
        //shininess
        float containerShininess = 64.0f;
        containerMat_.setShininess(containerShininess);
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
       isPointerLocked_ = glfwGetInputMode(native, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
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
        
       lightingShader_.setVec3("viewPos", camera_.get()->getAttributes().position);

       // world transformation
       glm::mat4 model = glm::mat4(1.0f);
       lightingShader_.setMat4("model", model);
       //bind meshbuffer
       cubeMesh_.Bind();
       //bind texture
       containerDiffuse_.bind(0);
       containerSpecular_.bind(1);
       containerEmission_.bind(2);
       //set uniforms in shader for material
       containerMat_.applyProperties(lightingShader_);
       //apply point light properties
       pointLight_.applyProperties(lightingShader_);
       //draw
       glDrawArrays(GL_TRIANGLES, 0, 36);
       //unbind mesh 
       cubeMesh_.Unbind();
       //unbind
       containerDiffuse_.unbind();
       containerSpecular_.unbind();
       containerEmission_.unbind();

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
       containerDiffuse_.cleanup();
       containerSpecular_.cleanup();
       cleanupImGUI();
       isCleaned_ = true;
   }
}