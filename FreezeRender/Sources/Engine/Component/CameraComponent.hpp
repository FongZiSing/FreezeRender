#pragma once

#include <Render/ShadingCamera.hpp>



class CameraComponent
{
private:
	Camera& entity;
	ShadingCamera& camera;

	const float speed = 0.05f;

public:
	explicit CameraComponent(Camera& targetEntity, ShadingCamera& targetCamera)
		: entity(targetEntity)
		, camera(targetCamera)
	{}

	inline void UpdateView(const int& width, const int& height)
	{
		entity.resolutionX = width;
		entity.resolutionY = height;
		entity.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		camera.UpdateView();
	}

	void TickComponent(float deltaTime);
};