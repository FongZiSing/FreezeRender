#pragma once

#include <Common.hpp>
#include "Matrix.hpp"
#include "Rotator.hpp"
#include <Utility/Delegate.hpp>



/**
 * @brief The view state that the camera owned.
 */
struct ViewState
{
	Vector3 location = { 0.f, 0.f, 10.f };

	Rotator rotation = { 0.f, 0.f, 0.f };

	Matrix view = Matrix::Identity;

	Matrix projection = Matrix::Identity;
		
	int resolutionX = 0;

	int resolutionY = 0;

	float aspectRatio = 0;

	float fieldOfView = 45.f; // The horizontal field of view (in degrees).

	float halfFOV = 0; // The half of horizontal field of view (in radians).

	float nearPlane = 0.1f;

	float farPlane = 50.f;
};



/**
 * @brief The euler camera object. TODO: UVN Camera.
 */
struct Camera
{
	ViewState viewState;

	const float speed = 0.05f;
	
	SingleDelegate<void(const ViewState&)> handleUpdated;

	inline Camera(const int& inWidth, const int& inHeight);

	/**
	 * @brief Change the resolution of camera.
	 */
	force_inline void Resize(const int& inWidth, const int& inHeight);

	/// Camera movement operation.
	force_inline void MoveUp();
	force_inline void MoveDown();
	force_inline void MoveLeft();
	force_inline void MoveRight();
	force_inline void MoveForword();
	force_inline void MoveBack();
	force_inline void UpdateRotation(const Rotator& rotation);
	/// Camera movement operation.

	/**
	 * @brief Pass the view state via callback function. 
	 */
	force_inline void Update() { handleUpdated.Execute(viewState); }

protected:

	/**
	 * @brief Calculate view matrix. (for MVP Transformation)
	 */
	inline void UpdateViewMatrix();

	/**
	 * @brief Calculate projection matrix. (for MVP Transformation)
	 */
	inline void UpdateProjectionMatrix();
};



/**
 * @brief The Detail implemention of `camera` class.
 */
#ifndef CAMERA_HPP_CAMERA_IMPL
#define CAMERA_HPP_CAMERA_IMPL
	
	inline Camera::Camera(const int& inWidth, const int& inHeight)
	{
		// the two elements in the projection matrix should always be constant.s
		viewState.projection.m[3][2] = 1.f;
		viewState.projection.m[3][3] = 0.f;

		// Init
		viewState.resolutionX = inWidth;
		viewState.resolutionY = inHeight;
		viewState.aspectRatio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
		viewState.halfFOV = Math::Degrees2Radians(viewState.fieldOfView / 2.f);

		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::Resize(const int& inWidth, const int& inHeight)
	{
		viewState.resolutionX = inWidth;
		viewState.resolutionY = inHeight;
		viewState.aspectRatio = static_cast<float>(inWidth) / static_cast<float>(inHeight);

		UpdateViewMatrix();
		UpdateProjectionMatrix();
		Update();
	}

	force_inline void Camera::MoveUp()
	{
		viewState.location += viewState.rotation.GetUpVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::MoveDown()
	{
		viewState.location -= viewState.rotation.GetUpVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::MoveLeft()
	{
		viewState.location -= viewState.rotation.GetRightVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::MoveRight()
	{
		viewState.location += viewState.rotation.GetRightVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::MoveForword()
	{
		viewState.location += viewState.rotation.GetForwardVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::MoveBack()
	{
		viewState.location -= viewState.rotation.GetForwardVector() * speed;
		UpdateViewMatrix();
		Update();
	}

	force_inline void Camera::UpdateRotation(const Rotator& rotation)
	{
		viewState.rotation = rotation;
		UpdateViewMatrix();
		Update();
	}

	inline void Camera::UpdateViewMatrix()
	{
		Vector3& location = viewState.location;
		Rotator& rotation = viewState.rotation;
		Matrix& view = viewState.view;

		auto invRotation = rotation.ToInvMatrix();
		auto invTranslation = location.ToInvMatrix();
		view = invRotation * invTranslation;
	}

	inline void Camera::UpdateProjectionMatrix()
	{
		const float& nearPlane = viewState.nearPlane;
		const float& farPlane = viewState.farPlane;
		const float& halfFOV = viewState.halfFOV;
		const float& aspectRatio = viewState.aspectRatio;
		Matrix& projection = viewState.projection;

		// look at -z, so take a negative value. 
		const float& n = -nearPlane;
		const float& f = -farPlane;
		const float t = -1.f / std::tanf(halfFOV);
		const float n_add_f = n + f;
		const float n_sub_f = n - f;

		const float r00 = t / aspectRatio;
		const float r11 = t;
		const float r22 = n_add_f / n_sub_f;
		const float r23 = -2.f * n * f / n_sub_f;

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

#endif // !CAMERA_HPP_CAMERA_IMPL
