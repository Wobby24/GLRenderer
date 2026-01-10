// Application.cpp

#include "VR_Engine.hpp"
#include <stdexcept>
#include <memory>
#include <iostream>
#include <utility>

namespace VR_Engine {
    void VR_Engine::initialize()
    {
        //vsync by default is off. we can toggle it, and if needed, we can get its state. idk its not insanely clean, but its easy to use for input and isnt that much more work to do
        window_.get()->ToggleVSync();
        //pointer lock is also off by default, again we toggle it to enable
        window_.get()->TogglePointerLock();
        //set window icon
        window_.get()->SetWindowIcon("res/Other/ICON.png");

        auto ctxDesc = renderer_->CreateDefaultContextDesc(); // returns std::unique_ptr<IRendererContextDesc>
        renderer_->Initialize(*ctxDesc, window_->GetSurface());

        scene_->setRenderScene(std::move(systemScenes.renderScene_));
    }

    void VR_Engine::run()
    {
        if (!window_ || !renderer_) {
            throw std::runtime_error("Application not initialized!");
        }

        double lastTime = window_->GetTime();

        while (!window_->ShouldClose())
        {
            double currentTime = window_->GetTime();
            float deltaTime = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;

            if (inputService_) {
                inputService_->Update(deltaTime);
            }

            if (scene_) {
                scene_->handleInput(inputService_.get());
            }

            if (scene_) {
                scene_->update(deltaTime);
            }

            if (scene_ && scene_->getRenderScene()) {
                renderer_->RenderScene(*scene_->getRenderScene());
            }

            window_->SwapBuffers();
            window_->PollEvents();
        }
    }


    void VR_Engine::update(float deltaTime)
    {
        // Update game logic or animations here (empty for now)
        (void)deltaTime; // silence unused parameter warning
    }

    void VR_Engine::shutdown()
    {
        if (renderer_) {
            renderer_->Cleanup();
            renderer_.reset();
        }
        if (window_) {
            window_.reset();
        }
    }

}