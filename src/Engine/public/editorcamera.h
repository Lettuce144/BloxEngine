#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H
#pragma once
#include <memory>

#include "EditorUI/EditorViewportPanel.h"
#include "raylib-cpp.hpp"

namespace BloxEngine {
class EditorCamera : public raylib::Camera3D {
public:
  EditorCamera()
      : raylib::Camera3D(raylib::Vector3{0.0f, 10.0f, 10.0f},
                         raylib::Vector3{0.0f, 0.0f, 0.0f},
                         raylib::Vector3{0.0f, 1.0f, 0.0f}, 45.0f,
                         CameraProjection::CAMERA_PERSPECTIVE) {}
  void SetViewportPanel(const std::shared_ptr<EditorUI::EditorViewportPanel>& viewportPanel) {
    m_editorViewportPanel = viewportPanel;
  }

  ~EditorCamera() = default;

  void DrawCamera();

private:
  std::weak_ptr<EditorUI::EditorViewportPanel> m_editorViewportPanel;
};
} // namespace BloxEngine

#endif // EDITORCAMERA_H
