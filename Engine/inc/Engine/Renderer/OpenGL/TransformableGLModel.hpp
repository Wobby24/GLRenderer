#pragma once

#include "GLModel.hpp"
#include "Transformable.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <typeinfo>

namespace Renderer {
    namespace OpenGL {
        class TransformableGLModel : public IModel, public ITransformable {
        public:
            TransformableGLModel(const std::string& path)
                : model_(path) {
            }

            // IModel interface
            void Draw(IShader& shader) override {
                shader.setMat4("model", GetTransformMatrix());
                model_.Draw(shader);
            }

            void DrawOptimized(IShader& shader) {
                shader.setMat4("model", GetTransformMatrix());
                model_.DrawOptimized(shader);
            }

            void Init() {
                model_.Init();
            }

            void Cleanup() {
                model_.Cleanup();
            }

            // ITransformable interface - const accessors
            const glm::vec3& GetPosition() const override { return transform_.GetPosition(); }
            const glm::quat& GetRotation() const override { return transform_.GetRotation(); }
            const glm::vec3& GetScale() const override { return transform_.GetScale(); }

            // ITransformable interface - non-const accessors
            glm::vec3& GetPosition() override { return transform_.GetPosition(); }
            glm::quat& GetRotation() override { return transform_.GetRotation(); }
            glm::vec3& GetScale() override { return transform_.GetScale(); }

            void SetPosition(const glm::vec3& pos) override { transform_.SetPosition(pos); }
            void SetRotation(const glm::quat& rot) override { transform_.SetRotation(rot); }
            void SetScale(const glm::vec3& scl) override { transform_.SetScale(scl); }

            // Provide read-only access to the underlying GLModel
            const GLModel& GetModel() const { return model_; }

            // Or if you need to modify it:
            GLModel& GetModel() { return model_; }

            glm::mat4 GetTransformMatrix() const override {
                return transform_.GetTransformMatrix();
            }

        private:
            GLModel model_;
            Transformable transform_;
        };

    }
}
