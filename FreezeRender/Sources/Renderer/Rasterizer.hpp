#pragma once

#include <Core/Meshlet.hpp>
#include <Core/Light.hpp>
#include <Core/Camera.hpp>
#include <Core/Matrix.hpp>
#include <Core/RenderTarget.hpp>
#include <Core/Shadingon.hpp>
#include <Shader/VertexShader.hpp>
#include <Shader/FragmentShader.hpp>
#include <vector>



/**
 * @brief The core of raster rendering.
 */
class Rasterizer
{
	// The target render size.
	int width, height;

	// The final render output buffer.
	ColorRenderTarget sceneBuffer;

	// The z-depth testing buffer.
	DepthRenderTarget depthBuffer;

	// A set of model object for rendering in every frame.
	std::vector<Meshlet> meshBuffer;

	// A set of point light for rendering in every frame.
	std::vector<PointLight> pointLightBuffer;

	// The camera status for rendering.
	ViewState viewStateBuffer;

	// The entry of vertex shader.
	Shader::VertexShader vertexShader;

	// The entry of fragment shader.
	Shader::FragmentShader fragmentShader;

public:
	void UpdateViewState(const ViewState& viewState) { viewStateBuffer = viewState; }

	auto& GetMeshBuffer() { return meshBuffer; }

	auto& GetPointLightBuffer() { return pointLightBuffer; }

	/**
	 * @brief Construct rasterizer with specific screen size.
	 */
	Rasterizer(int inWidth, int inHeight);

	/**
	 * @brief Resize render target.
	 */
	void Resize(int inWidth, int inHeight);

	/**
	 * @brief The root entry for rendering in every frame.
	 */
	ColorRenderTarget& Draw();

private:
	/**
	 * @brief The process by which polygons that are not facing the camera are removed from the rendering pipeline.
	 */
	bool BackFaceCulling(const ShadingTriangle& triangle);

	/**
	 * @brief The process by which polygons that are at homogeneous coordinates are clipped for rendering，
	 *        it is positioned in the pipeline just after view coordinates (MVP) and just before normalized device coordinates (NDC).
	 */
	void HomogeneousClipping(const ShadingTriangle& inTriangle, ShadingTriangle* outTriangles, int& triangleNum);

	/**
	 * @brief The process of rasterize a triangle. 
	 */
	void RasterizerTriangle(const ShadingTriangle& payload);
};