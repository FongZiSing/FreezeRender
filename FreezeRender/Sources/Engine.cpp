﻿#include "Engine.hpp"

#include <Renderer/RayTacing/RayTracingRenderer.hpp>
#include <Renderer/Raster/Rasterizer.hpp>
#include <World/RenderWorld.hpp>
#include <Input/InputSystem.hpp>



/**
 * @brief Global InputSystem.
 */
UniqueResource<Renderer> GRenderer;
UniqueResource<RenderWorld> GWorld;
UniqueResource<InputSystem> GInput;



Engine::Engine()
{
}

Engine::~Engine()
{
}

bool Engine::HandleCreateEvent(UINT width, UINT height)
{
	GRenderer.Initialize(new RayTracingRenderer());
	GWorld.Initialize(new RenderWorld());
	GInput.Initialize(new InputSystem());

	GRenderer->Startup(width, height);
	GWorld->Startup(width, height);
	return true;
}

void Engine::HandleKeyDownEvent(WPARAM nKey)
{
	GInput->PressedKey(static_cast<unsigned char>(nKey));
}

void Engine::HandleKeyUpEvent(WPARAM nKey)
{
	GInput->ReleasedKey(static_cast<unsigned char>(nKey));
}

void Engine::HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y)
{
	GInput->PressedMouse(InputSystem::VM_L, x, y);
}

void Engine::HandleLeftMouseUpEvent(WPARAM nFlags, int x, int y)
{
	GInput->ReleasedMouse(InputSystem::VM_L, x, y);
}

void Engine::HandleMiddleMouseDownEvent(WPARAM nFlags, int x, int y)
{
	GInput->PressedMouse(InputSystem::VM_M, x, y);
}

void Engine::HandleMiddleMouseUpEvent(WPARAM nFlags, int x, int y)
{
	GInput->ReleasedMouse(InputSystem::VM_M, x, y);
}

void Engine::HandleRightMouseDownEvent(WPARAM nFlags, int x, int y)
{
	GInput->PressedMouse(InputSystem::VM_R, x, y);
}

void Engine::HandleRightMouseUpEvent(WPARAM nFlags, int x, int y)
{
	GInput->ReleasedMouse(InputSystem::VM_R, x, y);
}

void Engine::HandleMouseMoveEvent(WPARAM nFlags, int x, int y)
{
	GInput->MovingMouse(x, y);
}

void Engine::HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y)
{
	GInput->WheelingMouse(x, y, zDelta);
}

void Engine::HandleResizeEvent(UINT width, UINT height)
{
	GWorld->ScreenResize(width, height);
	GRenderer->ScreenResize(width, height);
}

void Engine::HandleDestroyEvent()
{
	GRenderer->Shutdown();
	GWorld->Shutdown();
}

void Engine::Tick(const float deltaTime)
{
	// Update window state.
	GInput->Window.Resizing = IsResizing();
	GInput->Window.Maximum = IsWindowMaximum();

	// Tick logic.
	GWorld->Tick(deltaTime);

	// Tick Render.
	ColorRenderTarget& result = GRenderer->Render(*GWorld);
	Draw(result.Data(), (UINT)result.Width(), (UINT)result.Height(), result.Width() * 4u);
	
	// Reset input.
	GInput->Reset();
}