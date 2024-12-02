#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H
#pragma once

#include "scene.h"
#include <memory>

#include <Application/instanceprops.h>
#include <raylib-cpp.hpp>

namespace BloxEngine
{
    // TODO: Make an application class that holds the game instance
    class GameInstance
    {
    public:
        // TODO: Change the parameters to a struct.
        // title is a char for compatibility reasons with raylib
        GameInstance(const InstanceProperties &props);
        virtual ~GameInstance() = default;

        // This runs the game loop
        void Run();

        // Is called every frame when the instance window is open
        virtual void OnUpdate() = 0;

        // Called when the game instance is created
        virtual void OnAttach() = 0;

        // Called when the game instance is destroyed
        virtual void OnDetach() = 0;

        const std::shared_ptr<BloxEngine::Scene> &GetActiveScene() const { return m_ptrActiveScene; }
        const std::unique_ptr<raylib::Window> &GetInstanceWindow() const { return m_ptrWindow; }

    private:
        std::unique_ptr<raylib::Window> m_ptrWindow;
        std::shared_ptr<BloxEngine::Scene> m_ptrActiveScene;
    };

    /**
     * @brief Creates a new GameInstance object. All derived classes should call this.
     *
     * @param width The width of the game window.
     * @param height The height of the game window.
     * @param title The title of the game window.
     *
     * @return A pointer to the newly created GameInstance object.
     */
    GameInstance *CreateGameInstance();
}
#endif // GAMEINSTANCE_H