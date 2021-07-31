#include "FragmentShader.hpp"



namespace Shader::Fragment
{
	Color NormalShader(const FragmentPayload& payload)
	{
		const Vector3 NormalVisualizeColor = (payload.shadingpoint.normal + Vector3(1.f, 1.f, 1.f)) / 2.f;
		return Color::FromLinearVector(NormalVisualizeColor);
	}

	Color PhongShader(const FragmentPayload& payload)
	{
		Color texel = payload.texture->Sample(payload.shadingpoint.uv);
		Vector3 texelColor = { (float)texel.r, (float)texel.g, (float)texel.b };

		static const Vector3 ambientLightIntensity = { 10, 10, 10 };
		const Vector3 Kd = texelColor / 255.f;
		const Vector3 Ks = { 0.7937f, 0.7937f, 0.7937f };
		const Vector3 Ka = { 0.005f, 0.005f, 0.005f };
		const float P = 150;

		const Vector3& eye = payload.viewpoint.position;
		const Vector3& Point = payload.shadingpoint.position;
		const Vector3& normal = payload.shadingpoint.normal;

		Vector3 result = { 0, 0, 0 };

		for (const PointLight& perLight : *(payload.pointlights))
		{
			const Vector3 lightAt = perLight.location - Point;
			const Vector3 viewAt = eye - Point;
			const float distance = lightAt.LengthSquared();

			const Vector3 lightDirection = lightAt / std::sqrtf(distance);
			const Vector3 halfDirection = (lightAt + viewAt).Normalize();
			const Vector3 intensity = perLight.intensity / distance;

			// Diffuse.
			result += (Kd * intensity) * Math::Max(0.f, normal | lightDirection);

			// Specular.
			result += (Ks * intensity) * std::powf(Math::Max(0.f, normal | halfDirection), P);

			// Ambient.
			result += Ka * ambientLightIntensity;
		}

		Color out = Color::FromLinearVector(result);
		return out;
	}
}