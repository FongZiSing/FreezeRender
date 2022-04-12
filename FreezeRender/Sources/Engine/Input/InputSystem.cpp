#include "InputSystem.hpp"



/**
 * @brief Global InputSystem.
 */
InputSystem* GInput = nullptr;



InputSystem::InputSystem()
{
	GInput = this;
}

InputSystem::~InputSystem()
{
	GInput = nullptr;
}