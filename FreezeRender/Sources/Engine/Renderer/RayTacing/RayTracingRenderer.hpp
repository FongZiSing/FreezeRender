//
// RayTracingRenderer.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of ray tracing renderer.
//
#pragma once

#include "../Renderer.hpp"
#include <Render/RayTracing/Ray.hpp>



namespace Pluto
{
	class RayTracingRenderer : public Renderer
	{
	private:
		int width;
		int height;

		ColorRenderTarget scene;
		PosFloatRenderTarget depth;
		PrimitiveRay primitiveRay;

	public:
		RayTracingRenderer();

		virtual void Startup(unsigned int screenWidth, unsigned int screenHeight) override { ScreenResize(screenWidth, screenHeight); }

		virtual void Shutdown() override {}

		virtual void ScreenResize(int inWidth, int inHeight) override;

		virtual ColorRenderTarget& Render(const RenderWorld* Scene) override;

	protected:
	};
}