#include "FragmentShader.hpp"



namespace Pluto
{
	namespace Shader::DeferredFragment
	{
		Color PhongShader(const DeferredFragmentPayload& payload)
		{
			return payload.diffuse;
			Vector3f diffuse = { (float)payload.diffuse.r, (float)payload.diffuse.g, (float)payload.diffuse.b };

			static const Vector3f ambientLightIntensity = { 10, 10, 10 };
			const Vector3f Kd = diffuse / 255.f;
			static const Vector3f Ks = { 0.7937f, 0.7937f, 0.7937f };
			static const Vector3f Ka = { 0.005f, 0.005f, 0.005f };
			static const float P = 150;

			const Vector3f& viewpoint = payload.viewpoint;
			const Vector3f& shadingpoint = payload.shadingpoint;
			const Vector3f& normal = payload.normal;
			Vector3f result = { 0, 0, 0 };

			for (const PointLight& perPointLight : *(payload.pointlights))
			{
				APointLight& perLight = perPointLight.data;
				const Vector3f lightAt = perLight.location - shadingpoint;
				const Vector3f viewAt = viewpoint - shadingpoint;
				const float distance = lightAt.LengthSquared();

				const Vector3f lightDirection = lightAt / std::sqrtf(distance);
				const Vector3f halfDirection = (lightAt + viewAt).Normalize();
				const Vector3f intensity = perLight.intensity / distance;

				// Diffuse.
				result += (Kd * intensity) * std::max(0.f, normal | lightDirection);

				// Specular.
				result += (Ks * intensity) * std::powf(std::max(0.f, normal | halfDirection), P);

				// Ambient.
				result += Ka * ambientLightIntensity;
			}

			return Color::FromLinearVector(result);
		}
	}
}