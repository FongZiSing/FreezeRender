#pragma once

#include <Platform/Windows/D2DApp.hpp>



/**
 * @brief The FreezeRender root system.
 */
class Engine final : public D2DApp
{
public:
	Engine();

	~Engine() override;

protected:
	//~ Begin override message handle.

	virtual bool HandleCreateEvent(UINT width, UINT height) override;
	virtual void HandleKeyDownEvent(WPARAM nKey) override;
	virtual void HandleKeyUpEvent(WPARAM nKey) override;
	virtual void HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleLeftMouseUpEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMiddleMouseDownEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMiddleMouseUpEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleRightMouseDownEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleRightMouseUpEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMouseMoveEvent(WPARAM nFlags, int x, int y) override;
	virtual void HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y) override;
	virtual void HandleResizeEvent(UINT width, UINT height) override;
	virtual void HandleDestroyEvent() override;

	//~ End override message handle.

	/**
	 * @brief The root entry of tick system, call every frame.
	 */
	virtual void Tick(const float deltaTime) override;
};

