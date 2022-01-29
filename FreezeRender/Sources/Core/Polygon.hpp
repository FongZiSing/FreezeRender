#pragma once

#include "Matrix.hpp"
#include "Color.hpp"



struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;


	force_inline bool operator == (const Vertex& rhs);
};



struct VertexIndex
{
	unsigned int index = 0;
	Vertex* ptr = nullptr;
};



struct Triangle
{
	Vertex vertices[3];
};



#ifndef POLYGON_HPP_OPERATION_IMPL
#define POLYGON_HPP_OPERATION_IMPL

	force_inline bool Vertex::operator == (const Vertex& rhs)
	{
		return
			(position - rhs.position).LengthSquared() < Number::SMALL_NUMBER &&
			(  normal - rhs.normal  ).LengthSquared() < Number::SMALL_NUMBER &&
			(      uv - rhs.uv      ).LengthSquared() < Number::SMALL_NUMBER;
	}

#endif // !POLYGON_HPP_OPERATION_IMPL