#pragma once

#include <Core/ShadingElement.hpp>
#include <Core/Matrix.hpp>
#include <Core/Color.hpp>
#include <Core/Texture.hpp>
#include <Core/Light.hpp>



namespace Shader
{
	/**
	 * @brief The data structure used by the fragment shader.
	 */
	struct FragmentPayload
	{
		struct
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;
		}
		vertices[3];

		struct
		{
			Vector3 position;
		}
		viewpoint;

		struct
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;
		}
		shadingpoint;

		const Texture* texture;

		const std::vector<PointLight>* pointlights;

		// Check out memory alignment.
		static_assert(sizeof(viewpoint) == sizeof(float) * 3, "[FreezeRender] view point size is invalid!");
		static_assert(sizeof(shadingpoint) == sizeof(float) * 8, "[FreezeRender] shading point size is invalid!");


		// Initialize.
		FragmentPayload(const ShadingTriangle& triange, const Texture* sampleTexture, const Vector3& cameraPosition, const std::vector<PointLight>* scenePointLights)
		{
			// Initialize triangle.
			vertices[0].position = triange.vertices[0].viewspace.position;
			vertices[1].position = triange.vertices[1].viewspace.position;
			vertices[2].position = triange.vertices[2].viewspace.position;

			vertices[0].normal = triange.vertices[0].viewspace.normal;
			vertices[1].normal = triange.vertices[1].viewspace.normal;
			vertices[2].normal = triange.vertices[2].viewspace.normal;

			vertices[0].uv = triange.vertices[0].localspace.uv;
			vertices[1].uv = triange.vertices[1].localspace.uv;
			vertices[2].uv = triange.vertices[2].localspace.uv;

			// Initialize texture.
			texture = sampleTexture;

			// Initialize viewpoint elements.
			viewpoint.position = cameraPosition;

			// Initialize lights.
			pointlights = scenePointLights;
		}


		/**
		 * @brief Copy property of specified shading point from a POD pointer,
		 *        please manually check memory order before call it.  
		 */
		void SetShadingPoint(const void* inShadingpoint)
		{
			// Initialize shadingpoint elements.
			shadingpoint = *static_cast<const decltype(shadingpoint)*>(inShadingpoint);
		}
	};
	

	/**
	 * @brief All supported fragment shader.
	 */
	namespace Fragment
	{
		static Color Nothing(const FragmentPayload& payload) { return Color::Black; }

		Color NormalShader(const FragmentPayload& payload);

		Color PhongShader(const FragmentPayload& payload);
	}


	/**
	 * @brief The entry of fragment shader.
	 */
	class FragmentShader
	{
		typedef Color(*PrivateHandle)(const FragmentPayload&);

		static constexpr const PrivateHandle handle = Fragment::PhongShader;

	public:

		force_inline without_globalvar auto operator() (const FragmentPayload& payload)
		{
			// Empty function optimize.
			// for example:
			//     line 1:    FragmentShader fs;
			//     line 2:    Color c = fs( FragmentPayload { ... } );
			//
			// if `VertexShader::handle == Vertex::Nothing` is true,
			// line 2 will be  compiled as `Color c = Color::Black`, and FragmentPayload constructor function will not even be called.
			//
			// if the compiler supports that...
			if constexpr (handle != Fragment::Nothing)
			{
				return handle(payload);
			}
			else
			{
				return Color::Black;
			}
		}
	};
}