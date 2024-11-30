#ifndef EDITOR_PANEL_H
#define EDITOR_PANEL_H
#pragma once

#include "EditorElement.h"

#include <memory>
#include <vector>
#include <imgui.h>

namespace BloxEngine::EditorUI
{
    // Base class for all editor panels
    class EditorPanel
    {
    public:
        // Constructor and destructor
        EditorPanel();
        ~EditorPanel();

        virtual void Draw();

        void AddElement(std::unique_ptr<EditorElement> element);
    private:
        const char *m_panelName;
        std::vector<std::unique_ptr<EditorElement>> m_elements;
    };

} // namespace BloxEngine

#endif // EDITOR_PANEL_H