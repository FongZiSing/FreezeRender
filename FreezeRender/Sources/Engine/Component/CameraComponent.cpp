#include "CameraComponent.hpp"

#include <Input/InputSystem.hpp>



void CameraComponent::TickComponent(float deltaTime)
{
	Vector3 deltaLocation = Vector3::Zero;
	bool bMoving = false;
	
	if (GInput->IsKeyPressing(InputSystem::VK_W))
	{
		deltaLocation += data.rotation.GetForwardVector() * speed;
		bMoving = true;
	}

	if (GInput->IsKeyPressing(InputSystem::VK_S))
	{
		deltaLocation -= data.rotation.GetForwardVector()* speed;
		bMoving = true;
	}

	if (GInput->IsKeyPressing(InputSystem::VK_A))
	{
		deltaLocation -= data.rotation.GetRightVector() * speed;
		bMoving = true;
	}

	if (GInput->IsKeyPressing(InputSystem::VK_D))
	{
		deltaLocation += data.rotation.GetRightVector() * speed;
		bMoving = true;
	}

	if (GInput->IsKeyPressing(InputSystem::VK_Q))
	{
		deltaLocation += data.rotation.GetUpVector() * speed;
		bMoving = true;
	}

	if (GInput->IsKeyPressing(InputSystem::VK_E))
	{
		deltaLocation -= data.rotation.GetUpVector() * speed;
		bMoving = true;
	}

	int mouseX = -1, mouseY = -1, zDelta = 0;
	if (GInput->IsWheelingMouse(mouseX, mouseY, zDelta))
	{
		float direction = float((zDelta > 0) * 2 - 1);
		deltaLocation += data.rotation.GetForwardVector() * speed * direction;
		bMoving = true;
	}

	static int cacheMouseX = mouseX;
	static int cacheMouseY = mouseY;
	static Rotator cacheRotattion;
	if (GInput->IsMousePressed(InputSystem::VM_L, mouseX, mouseY))
	{
		cacheMouseX = mouseX;
		cacheMouseY = mouseY;
		cacheRotattion = data.rotation;
	}

	else if (GInput->IsMousePressing(InputSystem::VM_L, mouseX, mouseY) && GInput->IsMovingMouse(mouseX, mouseY) && !GInput->WindowState().Resizing)
	{
		float offsetX = (cacheMouseX - mouseX) * (data.fieldOfView / data.resolutionX);
		float offsetY = (cacheMouseY - mouseY) * (data.fieldOfView / data.resolutionY);
		data.rotation = cacheRotattion + Rotator(offsetX, offsetY).Normalize();
		camera.UpdateViewMatrix();
	}

	if (bMoving)
	{
		data.location += deltaLocation;
		camera.UpdateViewMatrix();
	}
}
