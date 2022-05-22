#pragma once
#pragma once

#include "../Renderer.hpp"

#include <Render/RayTracing/Ray.hpp>


class RayTracingRenderer : public Renderer
{
private:
	int width;
	int height;

	ColorRenderTarget scene;

	PrimitiveRay primitiveRay;
	bool bRegeneratePrimitiveRay;


public:
	RayTracingRenderer();

	virtual void Startup(unsigned int screenWidth, unsigned int screenHeight) override { ScreenResize(screenWidth, screenHeight); }

	virtual void Shutdown() override {}

	virtual void ScreenResize(int inWidth, int inHeight) override;

	virtual ColorRenderTarget& Render(const RenderWorld* Scene) override;

protected:

};