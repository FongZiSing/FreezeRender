#pragma once

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
		FragmentPayload(const Texture* sampling, const Vector3& cameraPosition, const std::vector<PointLight>* scenePointLights)
		{
			// Initialize texture.
			texture = sampling;

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


	/**
	 * @brief The BRDF data structure used by the deferred fragment shader.
	 */
	struct DeferredFragmentPayload
	{
		const std::vector<PointLight>* pointlights = nullptr;
		Vector3 viewpoint;
		Vector3 shadingpoint;

		Vector3 normal;
		Color diffuse;
	};


	/**
	 * @brief All supported deferred fragment shader.
	 */
	namespace DeferredFragment
	{
		static Color Nothing(const DeferredFragmentPayload& payload) { return Color::Black; }

		Color PhongShader(const DeferredFragmentPayload& payload);
	}


	/**
	 * @brief The entry of fragment shader.
	 */
	class DeferredFragmentShader
	{
		typedef Color(*PrivateHandle)(const DeferredFragmentPayload&);

		static constexpr const PrivateHandle handle = DeferredFragment::PhongShader;

	public:

		force_inline without_globalvar auto operator() (const DeferredFragmentPayload& payload)
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
			if constexpr (handle != DeferredFragment::Nothing)
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