#include <GLRenderer/OpenGL/Scenes/C2/LightingCubes.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLDirLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLSpotLight.hpp>

namespace GLRenderer {

	LightingCubes::LightingCubes() : view_(glm::mat4(1.0)), projection_(glm::mat4(1.0)), lightPos_(glm::vec3(1.2f, 1.0f, 2.0f)), lightColor_(glm::vec3(1.0f, 1.0f, 1.0f)), lightingShader_("res/Shaders/Scenes/C2/lighting.vert", "res/Shaders/Scenes/C2/lighting.frag"), lightSourceShader_("res/Shaders/Scenes/C2/lightSource.vert", "res/Shaders/Scenes/C2/lightSource.frag"), containerDiffuse_("res/Textures/container2.png"), containerSpecular_("res/Textures/container2_specular.png"), containerEmission_("res/Textures/matrix.jpg"), isInitialized_(false), isCleaned_(false), isWireframe_(false), imguiInitialized_(false), isPointerLocked_(true), showExitConfirmDialog(false), windowWidth_(1280), windowHeight_(720) {}

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

    void LightingCubes::renderLightingUI() {
        ImGui::Separator();
        ImGui::Text("Dynamic Lights");

        static int selectedLightID = -1;
        bool lightsChanged = false; // Track if any light property changed

        if (ImGui::CollapsingHeader("Manage Lights", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Add Point Light")) {
                if (lightManager_->GetNumLights(LightType::Point) < GLRenderer::GLLightManager::MAX_POINT_LIGHTS) {
                    auto pointLight = std::make_shared<GLRenderer::GLPointLight>();
                    pointLight->setPosition({ 0.f, 0.f, 0.f });
                    pointLight->setAmbient({ 0.05f, 0.05f, 0.05f });
                    pointLight->setDiffuse({ 0.8f, 0.8f, 0.8f });
                    pointLight->setSpecular({ 1.0f, 1.0f, 1.0f });
                    pointLight->setIntensity(1.0f);
                    selectedLightID = lightManager_->AddLight(pointLight);
                    lightsChanged = true;
                }
            }

            if (ImGui::Button("Add Directional Light")) {
                if (lightManager_->GetNumLights(LightType::Directional) < GLRenderer::GLLightManager::MAX_DIR_LIGHTS) {
                    auto dirLight = std::make_shared<GLRenderer::GLDirLight>();
                    dirLight->setDirection({ -0.2f, -1.0f, -0.3f });
                    dirLight->setAmbient({ 0.1f, 0.1f, 0.1f });
                    dirLight->setDiffuse({ 0.5f, 0.5f, 0.5f });
                    dirLight->setSpecular({ 1.0f, 1.0f, 1.0f });
                    dirLight->setIntensity(1.0f);
                    selectedLightID = lightManager_->AddLight(dirLight);
                    lightsChanged = true;
                }
            }

            if (ImGui::Button("Add Spot Light")) {
                if (lightManager_->GetNumLights(LightType::Spot) < GLRenderer::GLLightManager::MAX_SPOT_LIGHTS) {
                    auto spotLight = std::make_shared<GLRenderer::GLSpotLight>();
                    spotLight->setPosition({ 0.f, 0.f, 0.f });
                    spotLight->setDirection({ 0.f, -1.f, 0.f });
                    spotLight->setAmbient({ 0.05f, 0.05f, 0.05f });
                    spotLight->setDiffuse({ 0.9f, 0.9f, 0.9f });
                    spotLight->setSpecular({ 1.f, 1.f, 1.f });
                    spotLight->setIntensity(1.0f);
                    selectedLightID = lightManager_->AddLight(spotLight);
                    lightsChanged = true;
                }
            }

            ImGui::Separator();

            // Prepare lights list for dropdown
            std::vector<std::pair<int, std::string>> lightEntries;
            for (const auto& [id, light] : lightManager_->GetAllLights()) {
                std::string label = "Light #" + std::to_string(id) + " (" +
                    (light->getType() == LightType::Point ? "Point" :
                        light->getType() == LightType::Directional ? "Directional" :
                        light->getType() == LightType::Spot ? "Spot" : "Unknown") + ")";
                lightEntries.emplace_back(id, label);
            }

            // Find index of selectedLightID
            static int selectedIndex = -1;
            if (selectedLightID != -1) {
                selectedIndex = -1;
                for (int i = 0; i < (int)lightEntries.size(); ++i) {
                    if (lightEntries[i].first == selectedLightID) {
                        selectedIndex = i;
                        break;
                    }
                }
            }
            else {
                selectedIndex = -1;
            }

            // Combo box to select light
            if (ImGui::BeginCombo("Select Light", selectedIndex >= 0 ? lightEntries[selectedIndex].second.c_str() : "None")) {
                for (int n = 0; n < (int)lightEntries.size(); n++) {
                    bool isSelected = (selectedIndex == n);
                    if (ImGui::Selectable(lightEntries[n].second.c_str(), isSelected)) {
                        selectedIndex = n;
                        selectedLightID = lightEntries[n].first;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Edit selected light
            if (selectedLightID != -1) {
                auto light = lightManager_->GetLight(selectedLightID);
                if (!light) {
                    selectedLightID = -1;
                    return;
                }

                ImGui::Separator();
                ImGui::Text("Editing Light #%d", selectedLightID);

                if (light->getType() == LightType::Point) {
                    auto pl = std::dynamic_pointer_cast<GLRenderer::GLPointLight>(light);
                    if (!pl) {
                        std::cerr << "Error: dynamic_pointer_cast to GLPointLight failed" << std::endl;
                        return;
                    }
                    float intensity = pl->getIntensity();
                    glm::vec3 ambient = pl->getAmbient();
                    glm::vec3 diffuse = pl->getDiffuse();
                    glm::vec3 specular = pl->getSpecular();
                    glm::vec3 position = pl->getPosition();
                    float constant = pl->getConstant();
                    float linear = pl->getLinear();
                    float quadratic = pl->getQuadratic();

                    if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f)) {
                        pl->setIntensity(intensity);
                        lightsChanged = true;
                    }
                    if (ImGui::ColorEdit3("Ambient", &ambient[0])) {
                        pl->setAmbient(ambient);
                        lightsChanged = true;
                    }
                    if (ImGui::ColorEdit3("Diffuse", &diffuse[0])) {
                        pl->setDiffuse(diffuse);
                        lightsChanged = true;
                    }
                    if (ImGui::ColorEdit3("Specular", &specular[0])) {
                        pl->setSpecular(specular);
                        lightsChanged = true;
                    }
                    if (ImGui::SliderFloat3("Position", &position[0], -100.0f, 100.0f)) {
                        pl->setPosition(position);
                        lightsChanged = true;
                    }
                    if (ImGui::SliderFloat("Constant", &constant, 0.0f, 2.0f)) {
                        pl->setConstant(constant);
                        lightsChanged = true;
                    }
                    if (ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f)) {
                        pl->setLinear(linear);
                        lightsChanged = true;
                    }
                    if (ImGui::SliderFloat("Quadratic", &quadratic, 0.0f, 1.0f)) {
                        pl->setQuadratic(quadratic);
                        lightsChanged = true;
                    }
                }
                else if (light->getType() == LightType::Directional) {
                    auto dirLight = std::dynamic_pointer_cast<GLRenderer::GLDirLight>(light);
                    if (dirLight) {
                        glm::vec3 direction = dirLight->getDirection();
                        glm::vec3 ambient = dirLight->getAmbient();
                        glm::vec3 diffuse = dirLight->getDiffuse();
                        glm::vec3 specular = dirLight->getSpecular();
                        float intensity = dirLight->getIntensity();

                        if (ImGui::SliderFloat3("Direction", &direction[0], -1.0f, 1.0f)) {
                            dirLight->setDirection(direction);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Ambient", &ambient[0])) {
                            dirLight->setAmbient(ambient);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Diffuse", &diffuse[0])) {
                            dirLight->setDiffuse(diffuse);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Specular", &specular[0])) {
                            dirLight->setSpecular(specular);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f)) {
                            dirLight->setIntensity(intensity);
                            lightsChanged = true;
                        }
                    }
                }
                else if (light->getType() == LightType::Spot) {
                    auto sl = std::dynamic_pointer_cast<GLRenderer::GLSpotLight>(light);
                    if (sl) {
                        float intensity = sl->getIntensity();
                        glm::vec3 ambient = sl->getAmbient();
                        glm::vec3 diffuse = sl->getDiffuse();
                        glm::vec3 specular = sl->getSpecular();
                        glm::vec3 position = sl->getPosition();
                        glm::vec3 direction = sl->getDirection();
                        float cutOff = sl->getCutOff();
                        float outerCutOff = sl->getOuterCutOff();
                        float constant = sl->getConstant();
                        float linear = sl->getLinear();
                        float quadratic = sl->getQuadratic();

                        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f)) {
                            sl->setIntensity(intensity);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Ambient", &ambient[0])) {
                            sl->setAmbient(ambient);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Diffuse", &diffuse[0])) {
                            sl->setDiffuse(diffuse);
                            lightsChanged = true;
                        }
                        if (ImGui::ColorEdit3("Specular", &specular[0])) {
                            sl->setSpecular(specular);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat3("Position", &position[0], -100.0f, 100.0f)) {
                            sl->setPosition(position);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat3("Direction", &direction[0], -1.0f, 1.0f)) {
                            sl->setDirection(direction);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Constant", &constant, 0.0f, 2.0f)) {
                            sl->setConstant(constant);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f)) {
                            sl->setLinear(linear);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Quadratic", &quadratic, 0.0f, 1.0f)) {
                            sl->setQuadratic(quadratic);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Cutoff", &cutOff, 0.0f, 90.0f)) {
                            sl->setCutOff(cutOff);
                            lightsChanged = true;
                        }
                        if (ImGui::SliderFloat("Outer Cutoff", &outerCutOff, 0.0f, 90.0f)) {
                            sl->setOuterCutOff(outerCutOff);
                             lightsChanged = true;
                        }
                    }
                }

                if (ImGui::Button("Remove Light")) {
                    lightManager_->RemoveLight(selectedLightID);
                    selectedLightID = -1;
                    lightsChanged = true;
                }
            }
        }

        if (lightsChanged) {
            lightManager_->ApplyLights(lightingShader_);
        }
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

            // Re-bind and re-set all relevant shader uniforms
            lightingShader_.use();

            // Update projection/view matrices
            lightingShader_.setMat4("view", view_);
            lightingShader_.setMat4("projection", projection_);

            // Re-apply material properties
            containerMat_.applyProperties(lightingShader_);

            // Re-apply light uniforms
            lightManager_->ApplyLights(lightingShader_);
        }


        ImGui::Separator();
        ImGui::Text("Camera Settings");
        ImGui::SliderFloat("Speed", &camera_.get()->getAttributes().movementSpeed, 0.1f, 100.0f);
        ImGui::SliderFloat("Sensitivity", &camera_.get()->getAttributes().mouseSensitivity, 0.01f, 1.0f);
        ImGui::SliderFloat("Zoom", &camera_.get()->getAttributes().zoom, 1.0f, 45.0f);

        ImGui::Separator();
        ImGui::Text("Lighting");

        renderLightingUI();

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
        showTexture("Emission Map", containerMat_.getEmissiveTexture());

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
        lightManager_ = std::make_unique<GLLightManager>();

        lightManager_->Init();
        lightingShader_.init();
        lightSourceShader_.init();

        lightingShader_.use();
        //load textures
        containerDiffuse_.loadTexture();
        containerSpecular_.loadTexture();
        containerEmission_.loadTexture();

        containerDiffuse_.setType(TextureType::DIFFUSE);
        containerSpecular_.setType(TextureType::SPECULAR);
        containerEmission_.setType(TextureType::EMISSION);
        //setup material values
        containerMat_.setupProperties(std::make_shared<GLTexture2D>(containerDiffuse_), std::make_shared<GLTexture2D>(containerSpecular_), std::make_shared<GLTexture2D>(containerEmission_));
        //shininess
        float containerShininess = 64.0f;
        containerMat_.setShininess(containerShininess);
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

       planeMesh_.Bind();
       planeMesh_.CreateAll(planeVertices_, planeIndices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS | GLRenderer::VertexAttribFlags::NORMAL);
       planeMesh_.Unbind();
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

       if (isWireframe_) {
           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       }
       else {
           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       }

       lightingShader_.use();
       lightingShader_.setMat4("view", view_);
       lightingShader_.setMat4("projection", projection_);
        
       lightingShader_.setVec3("viewPos", camera_.get()->getAttributes().position);
       //bind meshbuffer
       cubeMesh_.Bind();
       //bind texture
       containerDiffuse_.bind(0);
       containerSpecular_.bind(1);
       containerEmission_.bind(2);
       //set uniforms in shader for material
       containerMat_.applyProperties(lightingShader_);
       lightingShader_.setInt("numPointLights", lightManager_->GetNumLights(LightType::Point));
       lightingShader_.setInt("numSpotLights", lightManager_->GetNumLights(LightType::Spot));
       lightingShader_.setInt("numDirLights", lightManager_->GetNumLights(LightType::Directional));
       //draw
       glm::mat4 model = glm::mat4(1.0f);
       for (unsigned int i = 0; i < 10; i++)
       {
           model = glm::mat4(1.0f);
           model = glm::translate(model, cubePositions[i]);
           float angle = 20.0f * i;
           model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
           lightingShader_.setMat4("model", model);

           glDrawArrays(GL_TRIANGLES, 0, 36);
       }
       //unbind mesh 
       cubeMesh_.Unbind();

       planeMesh_.Bind();

       model = glm::mat4(1.0f);
       model = glm::translate(model, glm::vec3(0.0f, -5.0f, -5.0f));
       model = glm::scale(model, glm::vec3(15.0f));

       lightingShader_.use();
       lightingShader_.setMat4("model", model);

       glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(planeIndices_.size()), GL_UNSIGNED_INT, 0);

       planeMesh_.Unbind();
       //unbind
       containerDiffuse_.unbind();
       containerSpecular_.unbind();
       containerEmission_.unbind();

       lightMesh_.Bind();

       lightSourceShader_.use();
       lightSourceShader_.setMat4("projection", projection_);
       lightSourceShader_.setMat4("view", view_);

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

   void LightingCubes::Cleanup() {
       //cleanup mesh buffer, set is cleaned to true
       cubeMesh_.Cleanup();
       lightMesh_.Cleanup();
       planeMesh_.Cleanup();
       containerDiffuse_.cleanup();
       containerSpecular_.cleanup();
       cleanupImGUI();
       isCleaned_ = true;
   }
}