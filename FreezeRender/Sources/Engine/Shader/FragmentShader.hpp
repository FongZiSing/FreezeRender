#pragma once

#include <Container/Array.hpp>
#include <Render/ShadingMaterial.hpp>
#include <Render/ShadingLight.hpp>



namespace Shader
{
	/**
	 * @brief The BRDF data structure used by the deferred fragment shader.
	 */
	struct DeferredFragmentPayload
	{
		const Array<ShadingPointLight>* pointlights;
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