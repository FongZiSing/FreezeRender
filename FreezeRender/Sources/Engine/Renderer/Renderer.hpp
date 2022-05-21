#pragma once

#include <Pattern/Singleton.hpp>
#include <Render/RenderTarget.hpp>



class RenderWorld;

/**
 * @brief The interface definition of renderer.
 */
class interface_as Renderer
{
public:
	virtual void Startup(unsigned int screenWidth, unsigned int screenHeight) = 0;

	virtual void Shutdown() = 0;

	virtual void ScreenResize(int inWidth, int inHeight) = 0;

	virtual ColorRenderTarget& Render(const RenderWorld* Scene) = 0;
};

extern UniqueResource<Renderer> GRenderer;