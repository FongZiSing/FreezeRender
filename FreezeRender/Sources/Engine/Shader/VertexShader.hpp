//
// VertexShader.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of vertex shader.
//
#pragma once

#include <Engine/Render/Shading/Polygon.hpp>



namespace Pluto
{
	namespace Shader
	{
		/**
		 * @brief The data structure used by the vertex shader.
		 */
		struct VertexPayload
		{
			const ShadingTriangle& triangle;
			const Matrix44f& modelViewMatrix;
			const Matrix44f& modelViewInvMatrix;
			const Matrix44f& modelViewProjectionMatrix;


			// Initialize.
			VertexPayload(ShadingTriangle& inOutTriange, const Matrix44f& inModelViewMatrix, const Matrix44f& inModelViewInvMatrix, const Matrix44f& inModelViewProjectionMatrix)
				: triangle(inOutTriange)
				, modelViewMatrix(inModelViewMatrix)
				, modelViewInvMatrix(inModelViewInvMatrix)
				, modelViewProjectionMatrix(inModelViewProjectionMatrix)
			{
				// TODO
			}
		};


		/**
		 * @brief All supported vertex shader.
		 */
		namespace Vertex
		{
			static void Nothing(const VertexPayload& payload) { }
		}


		/**
		 * @brief The entry of vertex shader.
		 */
		class VertexShader
		{
			typedef void(*PrivateHandle)(const VertexPayload&);

			static constexpr const PrivateHandle handle = Vertex::Nothing;

		public:
			force_noinline without_globalvar void operator() (const VertexPayload& payload)
			{
				// Empty function optimize.
				// for example:
				//     line 1:    VertexShader vs;
				//     line 2:    vs( VertexPayload { ... } );
				//
				// if `VertexShader::handle == Vertex::Nothing` is true,
				// line 2 will be ignored by the compiler and VertexPayload constructor function will not even be called.
				//
				// if the compiler supports that...
				if constexpr (VertexShader::handle != Vertex::Nothing)
				{
					handle(payload);
				}
			}
		};
	}
}