#include "editorcamera.h"
#include "raylib.h"

namespace BloxEngine
{
  void EditorCamera::DrawCamera()
  {
    if (auto viewportPanel = m_editorViewportPanel.lock()) // Check if the weak pointer is valid
    {
      if (viewportPanel->IsFocused())
      {
        Update(CAMERA_FREE);
        DisableCursor();
      }
      else
      {
        ShowCursor();
      }
    }
  }
} // namespace BloxEngine
