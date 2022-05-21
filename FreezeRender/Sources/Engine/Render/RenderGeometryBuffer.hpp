#pragma once

#include "RenderTarget.hpp"



/**
 * @brief The visibility buffer for deferred shading.
 */
struct GeometryBuffer
{
	FloatRenderTarget depth;
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