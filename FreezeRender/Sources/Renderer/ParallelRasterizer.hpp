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
 * @brief Visibility buffer structure.
 */
struct VisibilityBuffer
{
	ShadingPointBufferRenderTarget vertex1;
	ShadingPointBufferRenderTarget vertex2;
	ShadingPointBufferRenderTarget vertex3;
	InterpolationBufferRenderTarget interpolation;
	MaterialIdBufferRenderTarget materialid;
	// TODO: MaterialID, InstanceID

	VisibilityBuffer(int width, int height)
		: vertex1(width, height)
		, vertex2(width, height)
		, vertex3(width, height)
		, interpolation(width, height)
		, materialid(width, height)
	{}

	void Resize(int width, int height)
	{
		vertex1.Resize(width, height);
		vertex2.Resize(width, height);
		vertex3.Resize(width, height);
		interpolation.Resize(width, height);
		materialid.Resize(width, height);
	}

	force_inline void SetPixel(const int& index, const ShadingTriangle& triangle, const R128& zInverseAndInterpolation)
	{
		Register8Copy(&triangle.vertices[0], &vertex1.GetPixel(index));
		Register8Copy(&triangle.vertices[1], &vertex2.GetPixel(index));
		Register8Copy(&triangle.vertices[2], &vertex3.GetPixel(index));
		RegisterStoreAligned(zInverseAndInterpolation, &interpolation.GetPixel(index));
		materialid.SetPixel(index, triangle.material);
	}
};



/**
 * @brief Worklist buffer structure.
 */
struct WorklistBuffer : VisibilityBuffer
{
	IntRenderTarget screen;
	unsigned int number;

	WorklistBuffer(int width, int height)
		: VisibilityBuffer(width, height)
		, screen(width, height)
		, number(0)
	{}

	void Resize(int width, int height)
	{
		VisibilityBuffer::Resize(width, height);
		screen.Resize(width, height);
		number = 0;
	}

	void Clear()
	{
		number = 0;
	}

	void Push(const VisibilityBuffer& buffer, const int& index)
	{
		vertex1.SetPixel(number, buffer.vertex1.GetPixel(index));
		vertex2.SetPixel(number, buffer.vertex2.GetPixel(index));
		vertex3.SetPixel(number, buffer.vertex3.GetPixel(index));
		interpolation.SetPixel(number, buffer.interpolation.GetPixel(index));
		materialid.SetPixel(number, buffer.materialid.GetPixel(index));
		screen.SetPixel(number, index);
		number++;
	}
};

/**
 * @brief Geometry buffer structure.
 */
struct GeometryBuffer
{
	DepthRenderTarget depth;
	Float4RenderTarget position;
	Float4RenderTarget normal;
	ColorRenderTarget diffuse;

	GeometryBuffer(int width, int height)
		: depth(width, height)
		, position(width, height)
		, normal(width, height)
		, diffuse(width, height)
	{}

	void Resize(int width, int height)
	{
		depth.Resize(width, height);
		position.Resize(width, height);
		normal.Resize(width, height);
		diffuse.Resize(width, height);
	}
};

/**
 * @brief The core of multi-thread raster rendering.
 */
class ParallelRasterizer
{
	// The target render size.
	int width, height;

	// Visibility-Buffer.
	VisibilityBuffer VBuffer;

	// Worklist-Buffer.
	WorklistBuffer WBuffer;

	// Geometry-Buffer.
	GeometryBuffer GBuffer;

	// Final output.
	ColorRenderTarget scene;

	// A set of model object for rendering in every frame.
	std::vector<Meshlet> meshBuffer;

	// A set of point light for rendering in every frame.
	std::vector<PointLight> pointLightBuffer;

	// The camera status for rendering.
	ViewState viewStateBuffer;

	// The entry of vertex shader.
	Shader::VertexShader vertexShader;

	// The entry of fragment shader.
	Shader::DeferredFragmentShader fragmentShader;

public:
	void UpdateViewState(const ViewState& viewState) { viewStateBuffer = viewState; }

	auto& GetMeshBuffer() { return meshBuffer; }

	auto& GetPointLightBuffer() { return pointLightBuffer; }

	/**
	 * @brief Construct rasterizer with specific screen size.
	 */
	ParallelRasterizer(int inWidth, int inHeight);

	/**
	 * @brief Resize render target.
	 */
	void Resize(int inWidth, int inHeight);

	/**
	 * @brief The root entry for rendering in every frame.
	 */
	ColorRenderTarget& Draw();

protected:
	/**
	 * @brief Early pass including z-depth testing, etc. without shading.
	 */
	void PrePass();

	/**
	 * @brief Shading pass, rasterize triangles in parallel.
	 */
	void BasePass();

private:
	/**
	 * @brief The process of rasterize a triangle.
	 */
	void RasterizeTriangle(const ShadingTriangle& payload);

	/**
	 * @brief The process by which polygons that are not facing the camera are removed from the rendering pipeline.
	 */
	bool BackFaceCulling(const ShadingTriangle& triangle);

	/**
	 * @brief The process by which polygons that are at homogeneous coordinates are clipped for rendering£¬
	 *        it is positioned in the pipeline just after view coordinates (MVP) and just before normalized device coordinates (NDC).
	 */
	void HomogeneousClipping(const ShadingTriangle& inTriangle, ShadingTriangle* outTriangles, int& triangleNum);
};