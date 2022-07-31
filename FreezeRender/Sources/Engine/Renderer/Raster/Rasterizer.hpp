//
// Rasterizer.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of rasterizer.
//
#pragma once

#include "../Renderer.hpp"

#include <Engine/Render/GeometryBuffer.hpp>
#include <Engine/Render/VisibilityBuffer.hpp>
#include <Engine/Render/Shading/Camera.hpp>
#include <Engine/Render/Shading/Meshlet.hpp>
#include <Engine/Render/Shading/Light.hpp>
#include <Engine/Render/RenderTarget.hpp>
#include <Engine/Shader/VertexShader.hpp>
#include <Engine/Shader/FragmentShader.hpp>



namespace Pluto
{
	/**
	 * @brief The core of multi-thread raster rendering.
	 */
	class Rasterizer : public Renderer
	{
	private:
		friend class Engine;

		int width;
		int height;

		VisibilityBuffer VBuffer;
		WorklistBuffer WBuffer;
		GeometryBuffer GBuffer;
		ColorRenderTarget scene;

		Shader::VertexShader vertexShader;
		Shader::DeferredFragmentShader fragmentShader;


	public:
		Rasterizer();

		virtual ~Rasterizer();

		virtual void Startup(unsigned int screenWidth, unsigned int screenHeight) override { ScreenResize(screenWidth, screenHeight); }

		virtual void Shutdown() override {}

		virtual void ScreenResize(int inWidth, int inHeight) override;

		virtual ColorRenderTarget& Render(const RenderWorld* Scene) override;


	protected:
		void PrePass(const Camera& viewBuffer, const Array<Meshlet>& meshletBuffer);

		void BasePass(const Camera& viewBuffer, const Array<PointLight>& lightBuffer);

		void RasterizeTriangle(const ShadingTriangle& payload);

		bool BackFaceCulling(const ShadingTriangle& triangle);

		void HomogeneousClipping(const ShadingTriangle& inTriangle, ShadingTriangle* outTriangles, int& triangleNum);
	};
}