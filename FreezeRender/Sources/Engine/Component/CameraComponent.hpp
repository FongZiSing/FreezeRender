//
// CameraComponent.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of camera component.
//
#pragma once

#include <Engine/Render/Shading/Camera.hpp>



namespace Pluto
{
	class CameraComponent
	{
	private:
		ACamera& data;
		Camera& camera;

		const float speed = 0.05f;

	public:
		explicit CameraComponent(ACamera& asset, Camera& target)
			: data(asset)
			, camera(target)
		{}

		inline void UpdateView(const int& width, const int& height)
		{
			data.resolutionX = width;
			data.resolutionY = height;
			data.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			camera.UpdateView();
		}

		void TickComponent(float deltaTime);
	};
}