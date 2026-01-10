#pragma once

namespace Renderer {
	class IShader;

    enum class LightType {
        Directional,
        Point,
        Spot,
        Unknown
    };

    class ILightSource {
    public:
        virtual ~ILightSource() = 0;

        // Add index parameter
        virtual void applyProperties(IShader& shader, int index) = 0;
        virtual LightType getType() const = 0;

        virtual int getID() const = 0; // Add this
        virtual void setID(int id) = 0;
    };


	inline ILightSource::~ILightSource() = default;
}