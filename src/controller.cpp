#include "idk/engine/controller.hpp"


void idk::TestCharacterController::update()
{
    const bool *kbstate = SDL_GetKeyboardState(NULL);

    glm::vec3 dm(0.0f);
    if (kbstate[SDL_SCANCODE_A]) dm.x -= 1.0f;
    if (kbstate[SDL_SCANCODE_D]) dm.x += 1.0f;
    if (kbstate[SDL_SCANCODE_W]) dm.z += 1.0f;
    if (kbstate[SDL_SCANCODE_S]) dm.z -= 1.0f;
    this->move(dm);

    mButtonA = kbstate[SDL_SCANCODE_1];
    mButtonB = kbstate[SDL_SCANCODE_2];
    mButtonX = kbstate[SDL_SCANCODE_3];
    mButtonY = kbstate[SDL_SCANCODE_4];

}

