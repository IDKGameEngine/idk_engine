#include "idk/platform/InputManager.hpp"
#include "idk/platform/PlatformContext.hpp"

#include <SDL3/SDL.h>


idk::InputManager::InputManager()
{

}

void idk::InputManager::update(idk::IPlatformContext *ctx)
{
    (void)ctx;

    // SDL_GetKeyboardState();
    // SDL_GetGamepadAxis()
}

bool idk::InputManager::isKeyDown(int keycode) const
{
    (void)keycode;
    return false;
}

bool idk::InputManager::isMouseButtonDown(int button) const
{
    (void)button;
    return false;
}

void idk::InputManager::getMousePosition(int& x, int& y) const
{
    x = 0;
    y = 0;
}
