#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H
#pragma once
#include <toml++/impl/forward_declarations.hpp>

#include "scene.h"

#include <Application/instanceprops.h>
#include <raylib-cpp.hpp>

#include <toml++/toml.hpp>

#include <vfspp/VFS.h>

#include <memory>

namespace BloxEngine
{
  // TODO: Make an application class that holds the game instance
  class GameInstance
  {
  public:
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

    // Functions for serialization
    void OpenScene(const std::string &path);
    void SaveScene();

    const std::shared_ptr<BloxEngine::Scene> &GetActiveScene() const
    {
      return m_ptrActiveScene;
    }

    const std::unique_ptr<raylib::Window> &GetInstanceWindow() const
    {
      return m_ptrWindow;
    }

    /**
     * @brief Get the Virtual File System instance.
     *
     * @return A shared pointer to the Virtual File System.
     */
    std::shared_ptr<vfspp::VirtualFileSystem> GetVFS() const
    {
      return vfs;
    }

  private:
    std::unique_ptr<raylib::Window> m_ptrWindow;
    std::shared_ptr<BloxEngine::Scene> m_ptrActiveScene;

    //std::shared_ptr<vfspp::VirtualFileSystem> vfs; // vfspp Virtual File System

    vfspp::VirtualFileSystemPtr vfs;

    std::unique_ptr<vfspp::NativeFileSystem> m_ptrAssetsFS;
    std::unique_ptr<vfspp::NativeFileSystem> m_ptrProjectsFS;
    std::unique_ptr<vfspp::NativeFileSystem> m_ptrResourcesFS;
  };

  /**
   * @brief Creates a new GameInstance object. All derived classes should call
   * this.
   *
   * @return A pointer to the newly created GameInstance object.
   */
  GameInstance *CreateGameInstance();
} // namespace BloxEngine
#endif // GAMEINSTANCE_H
