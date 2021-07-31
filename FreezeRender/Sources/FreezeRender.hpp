#pragma once

#include <Windows/D2DApp.hpp>
#include <memory>

/// forward declaration.
class Rasterizer;
struct Camera;
/// forward declaration.



/**
 * @brief The FreezeRender root system.
 */
class FreezeRender final : public D2DApp
{
	// The core of raster rendering.
	std::unique_ptr<Rasterizer> rasterizer;

	// The main player's perspective in the world scene.
	std::unique_ptr<Camera> camera;

public:
	FreezeRender();

	~FreezeRender() override;

protected:
	/**
	 * @brief Initialize the world scene.
	 */
	void InitScene();

	/// Override message handle.
	virtual HRESULT HandleCreateEvent(UINT width, UINT height) override;
	virtual HPAINTRESULT HandlePaintEvent(const float deltaTime) override;
	virtual void HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMouseMoveEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y) override;
	virtual void HandleResizeEvent(UINT width, UINT height) override;
	/// Override message handle.

	/**
	 * @brief The root entry of tick system, call every frame.
	 */
	virtual void Tick(const float deltaTime) override;
};

