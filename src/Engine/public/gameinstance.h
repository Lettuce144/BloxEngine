#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H
#pragma once

#include "scene.h"
#include <memory>


class GameInstance
{
public:
    // title is a char for compatibility reasons with raylib
    GameInstance(const int& width, const int& height, const char* title);
    ~GameInstance() = default;
private:
    // TODO: Store a modern pointer to window
    // std::unique_ptr<raylib::Window> m_ptrWindow;
    std::shared_ptr<BloxEngine::Scene> m_ptrActiveScene;
};
#endif // GAMEINSTANCE_H