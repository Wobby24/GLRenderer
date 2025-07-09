// Application.cpp

#include "Engine.hpp"
#include <stdexcept>
#include <iostream>
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
    window_ = std::make_unique<GLRenderer::Window::Window>(
        ctxDesc.width, ctxDesc.height, "GLRenderer App",
        ctxDesc.majorVersion, ctxDesc.minorVersion);

    // Create renderer instance
    renderer_ = std::make_unique<GLRenderer::GLRenderer>();

    // Initialize renderer with context and window reference
    renderer_->Initialize(ctxDesc, *window_);
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
