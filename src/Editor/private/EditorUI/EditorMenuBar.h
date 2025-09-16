#ifndef EDITOR_MENU_BAR_H
#define EDITOR_MENU_BAR_H
#pragma once

#include <EditorUI/EditorElement.h>
#include <memory>

class Editor;

class EditorMenuBar : public BloxEngine::EditorUI::EditorElement {
public:
  EditorMenuBar(Editor &editor) : m_pEditor(editor) {}

  // Prevent copying
  EditorMenuBar(const EditorMenuBar &) = delete;
  EditorMenuBar &operator=(const EditorMenuBar &) = delete;

  // EditorMenuBar() = default;
  ~EditorMenuBar() = default;

  void DrawElement() const override;

private:
  // std::shared_ptr<Editor> m_pEditor;
  Editor &m_pEditor; // HACK: I'd like to use a shared pointer but that would require
                          //  Me rewriting the entrypoint and the gameinstance to use a shared pointer
                          //  However, a raw pointer is simple since deleting the instance happens when the entrypoint returns
};

#endif // EDITOR_MENU_BAR_H
