#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <raylib-cpp.hpp>

// Mostly wrapper functions for raylib
namespace BloxEngine::Engine
{
    static float GetDeltaTime()
    {
        return GetFrameTime();
    }
}
#endif // ENGINE_H