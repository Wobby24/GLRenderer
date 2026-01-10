#pragma once

#include <Renderer/Interface/IModel.hpp>
#include <Renderer/Interface/ITransformable.hpp>
#include <memory>

namespace Renderer {
    struct ModelInstance {
        std::shared_ptr<IModel> model;
        std::shared_ptr<ITransformable> transform; // or a concrete Transform class
    };
}