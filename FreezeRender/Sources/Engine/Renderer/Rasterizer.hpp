#pragma once

#include <Render/RenderGeometryBuffer.hpp>
#include <Render/RenderVisibilityBuffer.hpp>
#include <Render/ShadingCamera.hpp>
#include <Render/ShadingMeshlet.hpp>
#include <Render/ShadingLight.hpp>
#include <Render/RenderTarget.hpp>
#include <Math/Matrix.hpp>
#include <Shader/VertexShader.hpp>
#include <Shader/FragmentShader.hpp>



/**
 * @brief The core of multi-thread raster rendering.
 */
class Rasterizer
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

	void Startup(unsigned int screenWidth, unsigned int screenHeight) { ScreenResize(screenWidth, screenHeight); }

	void Shutdown() {}

	void ScreenResize(int inWidth, int inHeight);
	
	ColorRenderTarget& Render(const ShadingCamera& viewBuffer, const Array<ShadingMeshlet>& meshletBuffer, const Array<ShadingPointLight>& lightBuffer);


protected:
	void PrePass(const ShadingCamera& viewBuffer, const Array<ShadingMeshlet>& meshletBuffer);
	void BasePass(const ShadingCamera& viewBuffer, const Array<ShadingPointLight>& lightBuffer);

	void RasterizeTriangle(const ShadingTriangle& payload);
	bool BackFaceCulling(const ShadingTriangle& triangle);
	void HomogeneousClipping(const ShadingTriangle& inTriangle, ShadingTriangle* outTriangles, int& triangleNum);
};