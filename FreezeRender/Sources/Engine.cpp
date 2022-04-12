#include "Engine.hpp"

#include <Renderer/Rasterizer.hpp>
#include <World/RenderWorld.hpp>
#include <Input/InputSystem.hpp>



Engine::Engine()
{
}

Engine::~Engine()
{
}

bool Engine::HandleCreateEvent(UINT width, UINT height)
{
	raster = std::make_unique<Rasterizer>();
	world = std::make_unique<RenderWorld>();
	input = std::make_unique<InputSystem>();
	
	raster->Startup(width, height);
	world->Startup(width, height);
	return true;
}

void Engine::HandleKeyDownEvent(WPARAM nKey)
{
	input->PressedKey(static_cast<unsigned char>(nKey));
}

void Engine::HandleKeyUpEvent(WPARAM nKey)
{
	input->ReleasedKey(static_cast<unsigned char>(nKey));
}

void Engine::HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y)
{
	input->PressedMouse(InputSystem::VM_L, x, y);
}

void Engine::HandleLeftMouseUpEvent(WPARAM nFlags, int x, int y)
{
	input->ReleasedMouse(InputSystem::VM_L, x, y);
}

void Engine::HandleMiddleMouseDownEvent(WPARAM nFlags, int x, int y)
{
	input->PressedMouse(InputSystem::VM_M, x, y);
}

void Engine::HandleMiddleMouseUpEvent(WPARAM nFlags, int x, int y)
{
	input->ReleasedMouse(InputSystem::VM_M, x, y);
}

void Engine::HandleRightMouseDownEvent(WPARAM nFlags, int x, int y)
{
	input->PressedMouse(InputSystem::VM_R, x, y);
}

void Engine::HandleRightMouseUpEvent(WPARAM nFlags, int x, int y)
{
	input->ReleasedMouse(InputSystem::VM_R, x, y);
}

void Engine::HandleMouseMoveEvent(WPARAM nFlags, int x, int y)
{
	input->MovingMouse(x, y);
}

void Engine::HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y)
{
	input->WheelingMouse(x, y, zDelta);
}

void Engine::HandleResizeEvent(UINT width, UINT height)
{
	world->ScreenResize(width, height);
	raster->ScreenResize(width, height);
}

void Engine::HandleDestroyEvent()
{
	raster->Shutdown();
	world->Shutdown();
}

void Engine::Tick(const float deltaTime)
{
	// Update window state.
	input->Window.Resizing = IsResizing();
	input->Window.Maximum = IsWindowMaximum();

	// Tick logic.
	world->Tick(deltaTime);

	// Tick Render.
	auto& result = raster->Render(world->render.cameras[0], world->render.meshlets, world->render.pointlights);
	Draw(result.Data(), (UINT)result.Width(), (UINT)result.Height(), result.Width() * 4u);
	
	// Reset input.
	input->Reset();
}