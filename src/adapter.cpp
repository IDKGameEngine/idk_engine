#include "idk/core/adapter.hpp"
#include <SDL3/SDL_timer.h>

uint64_t idk_adapter::GetSysTimeMs() { return SDL_GetTicks(); }
uint64_t idk_adapter::GetSysTimeNs() { return SDL_GetTicksNS(); }

