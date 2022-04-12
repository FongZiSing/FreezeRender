#include "FragmentShader.hpp"



namespace Shader::DeferredFragment
{
	Color PhongShader(const DeferredFragmentPayload& payload)
	{
		Vector3 diffuse = { (float)payload.diffuse.r, (float)payload.diffuse.g, (float)payload.diffuse.b };
		static const Vector3 ambientLightIntensity = { 10, 10, 10 };
		const Vector3 Kd = diffuse / 255.f;
		static const Vector3 Ks = { 0.7937f, 0.7937f, 0.7937f };
		static const Vector3 Ka = { 0.005f, 0.005f, 0.005f };
		static const float P = 150;

		const Vector3& viewpoint = payload.viewpoint;
		const Vector3& shadingpoint = payload.shadingpoint;
		const Vector3& normal = payload.normal;
		Vector3 result = { 0, 0, 0 };

		for (const ShadingPointLight& perPointLight : *(payload.pointlights))
		{
			PointLight& perLight = perPointLight.pointLight;
			const Vector3 lightAt = perLight.location - shadingpoint;
			const Vector3 viewAt = viewpoint - shadingpoint;
			const float distance = lightAt.LengthSquared();

			const Vector3 lightDirection = lightAt / std::sqrtf(distance);
			const Vector3 halfDirection = (lightAt + viewAt).Normalize();
			const Vector3 intensity = perLight.intensity / distance;

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