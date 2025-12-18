// Application.cpp

#include "Engine.hpp"
#include <stdexcept>
#include <memory>
#include <iostream>
#include <utility>
#include "GLRenderer/OpenGL/Scenes/C3/ModelLoading.hpp"
#include <GLFW/glfw3.h>  // for glfwGetTime, glfwPollEvents, etc.

void Engine::initialize()
{
    // Create your OpenGL context descriptor
    GLRenderer::GLRendererContextDesc ctxDesc{};
    ctxDesc.width = 1280;
    ctxDesc.height = 720;
    ctxDesc.majorVersion = 4;  // specify your desired OpenGL version
    ctxDesc.minorVersion = 6;

    // Create window with requested context version 
    window_ = std::make_unique<GLRenderer::Window::GLFW_Window>(
        ctxDesc.width, ctxDesc.height, "GLRenderer App",
        ctxDesc.majorVersion, ctxDesc.minorVersion);
    //vsync by default is off. we can toggle it, and if needed, we can get its state. idk its not insanely clean, but its easy to use for input and isnt that much more work to do
    window_.get()->ToggleVSync();
    //pointer lock is also off by default, again we toggle it to enable
    window_.get()->TogglePointerLock();
    //set window icon
    window_.get()->SetWindowIcon("res/Other/ICON.png");

    // Create renderer instance
    renderer_ = std::make_unique<GLRenderer::GLRenderer>();

    // create init scene (for testing)
    std::unique_ptr<GLRenderer::ModelLoading> modelScene;

    modelScene = std::make_unique<GLRenderer::ModelLoading>();
 
    // Initialize renderer with context and window reference
    renderer_->Initialize(ctxDesc, *window_->GetSurface());

    renderer_->SetWindow(*window_);

    modelScene->Init();

    renderer_->SetScene(std::move(modelScene));
}

void Engine::run()
{
    if (!window_ || !renderer_) {
        throw std::runtime_error("Application not initialized!");
    }

    double lastTime = glfwGetTime();

    while (!window_->ShouldClose())
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        update(deltaTime);

        renderer_->RenderFrame();

        window_->SwapBuffers();
        window_->PollEvents();
    }
}

void Engine::update(float deltaTime)
{
    // Update game logic or animations here (empty for now)
    (void)deltaTime; // silence unused parameter warning
}

void Engine::shutdown()
{
    if (renderer_) {
        renderer_->Cleanup();
        renderer_.reset();
    }
    if (window_) {
        window_.reset();
    }
}
