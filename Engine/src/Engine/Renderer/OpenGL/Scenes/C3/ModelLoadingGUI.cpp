#include <GLRenderer\OpenGL\Scenes\C3\ModelLoading.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLDirLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLSpotLight.hpp>
#include <GLRenderer/Utils/getAbsolutePath.hpp>

namespace Renderer {
    void ModelLoading::initImGUI(GLFWwindow* window) {
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

    bool ModelLoading::drawVec3Control(const std::string& label, glm::vec3& values, float resetValue, bool isColor) {
        bool valueChanged = false;

        ImGui::PushID(label.c_str());

        ImGui::Text("%s", label.c_str());
        ImGui::SameLine();

        if (isColor) {
            // Show color picker instead of custom XYZ sliders
            if (ImGui::ColorEdit3(("##" + label).c_str(), glm::value_ptr(values))) {
                valueChanged = true;
            }
        }
        else {
            // Show your custom XYZ control
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 0 });

            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            const char* labels[3] = { "X", "Y", "Z" };
            ImVec4 colors[3] = {
                ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f },
                ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f },
                ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }
            };
            ImVec4 hovered[3] = {
                ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f },
                ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f },
                ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }
            };

            float* components[3] = { &values.x, &values.y, &values.z };

            for (int i = 0; i < 3; ++i) {
                ImGui::PushStyleColor(ImGuiCol_Button, colors[i]);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered[i]);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors[i]);

                if (ImGui::Button(labels[i], buttonSize)) {
                    *components[i] = resetValue;
                    valueChanged = true;
                }
                ImGui::PopStyleColor(3);
                ImGui::SameLine();

                std::string dragLabel = "##" + label + labels[i];

                ImGui::PushItemWidth(60.0f);
                if (ImGui::DragFloat(dragLabel.c_str(), components[i], 0.01f, -1000.0f, 1000.0f)) {
                    valueChanged = true;
                }
                ImGui::PopItemWidth();

                if (i < 2)
                    ImGui::SameLine();
            }

            ImGui::PopStyleVar();
        }

        ImGui::PopID();

        return valueChanged;
    }

    void ModelLoading::renderTGLM_GUI() {
        if (ImGui::CollapsingHeader("Transformable Models", ImGuiTreeNodeFlags_DefaultOpen)) {
            static int selectedModelID = -1;

            // === NEW: UI for model loading ===
            static char modelPathBuffer[256];  // no "= \"\""
            ImGui::InputText("Model Path", modelPathBuffer, IM_ARRAYSIZE(modelPathBuffer));
            if (ImGui::Button("Load Model")) {
                std::string pathStr(modelPathBuffer);

                std::string absolutePath = Renderer::getAbsoluteResourcePath(pathStr, "GLRenderer/res/Models");

                if (!pathStr.empty() && std::filesystem::exists(absolutePath)) {
                    static int nextModelID = 0;
                    int newID = nextModelID++;

                    auto newModel = std::make_shared<TransformableGLModel>(pathStr);  // pass relative path only
                    transformableModels_[newID] = newModel;

                    newModel->Init();

                    selectedModelID = newID;
                }
                else {
                    std::cout << "[ERROR] Path does not exist: " << absolutePath << std::endl;
                }
            }

            ImGui::Separator();
            // === End of new UI ===

            // Generate model list
            std::vector<std::pair<int, std::string>> modelEntries;
            for (const auto& [id, model] : transformableModels_) {
                modelEntries.emplace_back(id, "Model #" + std::to_string(id));
            }

            // Determine selected index
            static int selectedIndex = -1;
            if (selectedModelID != -1) {
                selectedIndex = -1;
                for (int i = 0; i < (int)modelEntries.size(); ++i) {
                    if (modelEntries[i].first == selectedModelID) {
                        selectedIndex = i;
                        break;
                    }
                }
            }

            // Combo box
            if (ImGui::BeginCombo("Select Model", selectedIndex >= 0 ? modelEntries[selectedIndex].second.c_str() : "None")) {
                for (int i = 0; i < (int)modelEntries.size(); ++i) {
                    bool isSelected = (selectedIndex == i);
                    if (ImGui::Selectable(modelEntries[i].second.c_str(), isSelected)) {
                        selectedIndex = i;
                        selectedModelID = modelEntries[i].first;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (selectedModelID != -1) {
                auto& model = transformableModels_[selectedModelID];
                if (!model) {
                    selectedModelID = -1;
                    return;
                }

                ImGui::Separator();
                ImGui::Text("Editing Model #%d", selectedModelID);

                glm::vec3 position = model->GetPosition();
                glm::vec3 scale = model->GetScale();
                glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(model->GetRotation()));
                bool updated = false;

                if (drawVec3Control("Position", position)) {
                    model->SetPosition(position);
                    updated = true;
                }

                if (drawVec3Control("Rotation", eulerAngles)) {
                    glm::quat newRot = glm::quat(glm::radians(eulerAngles));
                    model->SetRotation(newRot);
                    updated = true;
                }

                if (drawVec3Control("Scale   ", scale)) {
                    model->SetScale(scale);
                    updated = true;
                }

                if (ImGui::Button("Reset Transform")) {
                    model->SetPosition(glm::vec3(0.0f));
                    model->SetRotation(glm::quat(glm::vec3(0.0f)));
                    model->SetScale(glm::vec3(1.0f));
                }

                auto& glModel = model->GetModel(); // GLModel&
                for (const auto& [materialName, materialPtr] : glModel.GetMaterialCache()) {
                    std::filesystem::path matPath(materialName);
                    std::string filename = matPath.filename().string();

                    if (ImGui::CollapsingHeader("Materials")) {
                        drawMaterialEditor(*materialPtr);
                    }
                }
            }
        }
    }

    void ModelLoading::renderLightingUI() {
        ImGui::Separator();
        ImGui::Text("Dynamic Lights");

        static int selectedLightID = -1;
        bool lightsChanged = false;

        if (ImGui::CollapsingHeader("Manage Lights", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Add Point Light")) {
                if (lightManager_->GetNumLights(LightType::Point) < Renderer::GLLightManager::MAX_POINT_LIGHTS) {
                    auto pointLight = std::make_shared<Renderer::GLPointLight>();
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
                if (lightManager_->GetNumLights(LightType::Directional) < Renderer::GLLightManager::MAX_DIR_LIGHTS) {
                    auto dirLight = std::make_shared<Renderer::GLDirLight>();
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
                if (lightManager_->GetNumLights(LightType::Spot) < Renderer::GLLightManager::MAX_SPOT_LIGHTS) {
                    auto spotLight = std::make_shared<Renderer::GLSpotLight>();
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

            std::vector<std::pair<int, std::string>> lightEntries;
            for (const auto& [id, light] : lightManager_->GetAllLights()) {
                std::string label = "Light #" + std::to_string(id) + " (" +
                    (light->getType() == LightType::Point ? "Point" :
                        light->getType() == LightType::Directional ? "Directional" :
                        light->getType() == LightType::Spot ? "Spot" : "Unknown") + ")";
                lightEntries.emplace_back(id, label);
            }

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

            const char* currentLabel = "None";
            if (selectedIndex >= 0 && selectedIndex < (int)lightEntries.size()) {
                currentLabel = lightEntries[selectedIndex].second.c_str();
            }

            if (ImGui::BeginCombo("Select Light", currentLabel)) {
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

            if (selectedLightID != -1) {
                auto light = lightManager_->GetLight(selectedLightID);
                if (!light) {
                    selectedLightID = -1;
                    return;
                }

                ImGui::Separator();
                ImGui::Text("Editing Light #%d", selectedLightID);

                if (light->getType() == LightType::Point) {
                    auto pl = std::dynamic_pointer_cast<Renderer::GLPointLight>(light);
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
                    if (drawVec3Control("Ambient  ", ambient, 0.0f, true)) {
                        pl->setAmbient(ambient);
                        lightsChanged = true;
                    }
                    if (drawVec3Control("Diffuse  ", diffuse, 0.0f, true)) {
                        pl->setDiffuse(diffuse);
                        lightsChanged = true;
                    }
                    if (drawVec3Control("Specular ", specular, 0.0f, true)) {
                        pl->setSpecular(specular);
                        lightsChanged = true;
                    }
                    if (drawVec3Control("Position ", position, 0.0f, false)) {
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
                    auto dirLight = std::dynamic_pointer_cast<Renderer::GLDirLight>(light);
                    if (dirLight) {
                        glm::vec3 direction = dirLight->getDirection();
                        glm::vec3 ambient = dirLight->getAmbient();
                        glm::vec3 diffuse = dirLight->getDiffuse();
                        glm::vec3 specular = dirLight->getSpecular();
                        float intensity = dirLight->getIntensity();

                        if (drawVec3Control("Ambient  ", ambient, 0.0f, true)) {
                            dirLight->setAmbient(ambient);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Diffuse  ", diffuse, 0.0f, true)) {
                            dirLight->setDiffuse(diffuse);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Specular ", specular, 0.0f, true)) {
                            dirLight->setSpecular(specular);
                            lightsChanged = true;
                        }

                        if (drawVec3Control("Direction", direction, 0.0f, false)) {
                            dirLight->setDirection(direction);
                            lightsChanged = true;
                        }

                        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f)) {
                            dirLight->setIntensity(intensity);
                            lightsChanged = true;
                        }
                    }
                }
                else if (light->getType() == LightType::Spot) {
                    auto sl = std::dynamic_pointer_cast<Renderer::GLSpotLight>(light);
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
                        if (drawVec3Control("Ambient  ", ambient, 0.0f, true)) {
                            sl->setAmbient(ambient);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Diffuse  ", diffuse, 0.0f, true)) {
                            sl->setDiffuse(diffuse);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Specular ", specular, 0.0f, true)) {
                            sl->setSpecular(specular);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Position ", position, 0.0f, false)) {
                            sl->setPosition(position);
                            lightsChanged = true;
                        }
                        if (drawVec3Control("Direction", direction, 0.0f, false)) {
                            sl->setDirection(direction);
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
                    }
                }

                if (ImGui::Button("Remove Light")) {
                    lightManager_->RemoveLight(selectedLightID);
                    selectedLightID = -1;
                    selectedIndex = -1; // Reset selectedIndex too!
                    lightsChanged = true;
                }
            }
        }

        if (lightsChanged) {
            lightManager_->ApplyLights(lightingShader_);
        }
    }

    void ModelLoading::drawMaterialEditor(Renderer::GLMaterial& material) {
        // Display shininess and emission intensity
        float shininess = material.getShininess();
        if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f)) {
            material.setShininess(shininess);
        }

        float emission = material.getEmissionIntensity();
        if (ImGui::SliderFloat("Emission Intensity", &emission, 0.0f, 10.0f)) {
            material.setEmissionIntensity(emission);
        }

        // Helper lambda to display textures of each type
        auto displayTextureList = [](const std::vector<std::shared_ptr<GLRenderer::GLTexture2D>>& textures, const char* label) {
            if (textures.empty()) {
                ImGui::Text("%s: None", label);
                return;
            }

            // Replace the old code here with the new table layout:

            ImGui::Text("%s:", label);
            ImGui::Indent();

            static std::unordered_map<unsigned int, bool> flipStates; // persistent flip states

            if (ImGui::BeginTable(("table_" + std::string(label)).c_str(), 4, ImGuiTableFlags_NoBordersInBody)) {
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Thumbnail", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                ImGui::TableSetupColumn("Flip", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < textures.size(); ++i) {
                    auto& tex = textures[i];
                    unsigned int texID = tex->getID();

                    std::filesystem::path texPath(tex->getFilePath());
                    std::string texFilename = texPath.filename().string();

                    // Initialize flip state if not set
                    if (flipStates.find(texID) == flipStates.end()) {
                        flipStates[texID] = tex->getVertTexFlip();
                    }

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    // Center the filename in the current table cell
                    float cellWidth = ImGui::GetContentRegionAvail().x;
                    ImVec2 textSize = ImGui::CalcTextSize(texFilename.c_str());
                    float padding = (cellWidth - textSize.x) * 0.5f;

                    if (padding > 0.0f)
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);

                    ImGui::Text("%s", texFilename.c_str());


                    ImGui::TableSetColumnIndex(1);
                    ImGui::Image((void*)(intptr_t)texID, ImVec2(128, 128));

                    ImGui::TableSetColumnIndex(2);
                    bool& flipped = flipStates[texID];
                    std::string checkboxLabel = flipped ? "true" : "false";
                    if (ImGui::Checkbox(checkboxLabel.c_str(), &flipped)) {
                        tex->setVertTexFlip(flipped);
                        tex->reload();
                    }

                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%u", texID);
                }

                ImGui::EndTable();
            }

            ImGui::Unindent();
            };

        // Show all texture types
        displayTextureList(material.getDiffuseTextures(), "Diffuse Textures");
        displayTextureList(material.getSpecularTextures(), "Specular Textures");
        displayTextureList(material.getEmissionTextures(), "Emission Textures");
    }

    void ModelLoading::cleanupImGUI() {
        if (!imguiInitialized_) return;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        imguiInitialized_ = false;
    }

    void ModelLoading::renderImGUI() {
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

            // Re-apply light uniforms
            lightManager_->ApplyLights(lightingShader_);
        }


        ImGui::Separator();
        ImGui::Text("Camera Settings");
        ImGui::SliderFloat("Speed", &camera_.get()->getAttributes().movementSpeed, 0.1f, 100.0f);
        ImGui::SliderFloat("Sensitivity", &camera_.get()->getAttributes().mouseSensitivity, 0.01f, 1.0f);
        ImGui::SliderFloat("Zoom", &camera_.get()->getAttributes().zoom, 1.0f, 45.0f);

        renderLightingUI();

        renderTGLM_GUI();

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
}

