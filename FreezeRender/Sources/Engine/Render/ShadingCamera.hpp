#pragma once

#include <Asset/Camera/Camera.hpp>



struct ShadingCamera
{
	Camera& camera;
	Matrix view       = Matrix::Identity;
	Matrix projection = Matrix::Identity;
	float halfFOV     = 0; // The half of horizontal field of view (in radians).


	ShadingCamera(Camera& target, const int& width, const int& height)
		: camera(target)
	{
		camera.resolutionX = width;
		camera.resolutionY = height;
		camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		projection.m[3][2] = 1.f;
		projection.m[3][3] = 0.f;
		UpdateView();
	}

	inline void UpdateView()
	{
		halfFOV = Math::Degrees2Radians(camera.fieldOfView / 2.f);
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	inline void UpdateViewMatrix()
	{
		Vector3& location   = camera.location;
		Rotator& rotation   = camera.rotation;
		auto invRotation    = rotation.ToInvMatrix();
		auto invTranslation = location.ToInvMatrix();

		view = invRotation * invTranslation;
	}

	inline void UpdateProjectionMatrix()
	{
		const float& nearPlane = camera.nearPlane;
		const float& farPlane = camera.farPlane;
		const float& aspectRatio = camera.aspectRatio;

		// look at -z, so take a negative value. 
		const float& n      = -nearPlane;
		const float& f      = -farPlane;
		const float t       = -1.f / std::tanf(halfFOV);
		const float n_add_f = n + f;
		const float n_sub_f = n - f;
		const float r00     = t / aspectRatio;
		const float r11     = t;
		const float r22     = n_add_f / n_sub_f;
		const float r23     = -2.f * n * f / n_sub_f;

		// projection = Matrix(
		//	{ r00,  0,   0,   0  },
		//	{ 0,   r11,  0,   0  },
		//	{ 0,    0,  r22, r23 },
		//	{ 0,    0,   1,   0  });
		projection.m[0][0] = r00;
		projection.m[1][1] = r11;
		projection.m[2][2] = r22;
		projection.m[2][3] = r23;
	}

	inline Vector2 GetNdcToScreen() const noexcept
	{
		return {
			(camera.farPlane - camera.nearPlane) / 2.f,
			(camera.farPlane + camera.nearPlane) / 2.f,
		};
	}
};