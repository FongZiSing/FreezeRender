#pragma once
#include "RenderTarget.hpp"
#include <Render/ShadingMaterial.hpp>



struct ShadingPointBufferPixelTraits
{
	using Type = ShadingPoint;
	inline static float nan = Number::FLOAT_NAN;
	inline static const Type defaultPixelValue = {
		{ nan, nan, nan },
		{ nan, nan, nan },
		{ nan, nan }
	};
}; typedef RenderTarget<ShadingPointBufferPixelTraits> ShadingPointBufferRenderTarget;



struct InterpolationBufferDescriptorPixelTraits
{
	using Type = ShadingInterpolation;
	inline static const Type defaultPixelValue = {
		Number::FLOAT_INF,
		Number::FLOAT_INF,
		Number::FLOAT_INF,
		Number::FLOAT_INF
	};
}; typedef RenderTarget<InterpolationBufferDescriptorPixelTraits> InterpolationBufferRenderTarget;



struct MaterialIdBufferPixelTraits
{
	// Using pointer of material as material id.
	using Type = ShadingMaterial*;
	inline static const Type defaultPixelValue = { nullptr };
}; typedef RenderTarget<MaterialIdBufferPixelTraits> MaterialIdBufferRenderTarget;



/**
 * @brief The visibility buffer for acceleration.
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
		materialid.SetPixel(index, triangle.materialEntity);
	}
};



struct WorklistBuffer : public VisibilityBuffer
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