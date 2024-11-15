#ifndef EDITOR_BUTTON_H
#define EDITOR_BUTTON_H
#pragma once

#include "EditorElement.h"

#include <functional>
#include <string>
#include <imgui.h>

namespace BloxEngine::EditorUI
{
    class EditorButton : public EditorElement
    {
    public:

        // Only here for simple usage a button always has a callback
        // 'Normal' constructor
        EditorButton(const std::string &label, const ImVec2 &size = ImVec2(0, 0), std::function<void()> onClick = nullptr)
            : m_label(label.c_str()), m_size(size), m_onClick(onClick)
        {
        }

        // Support for C strings
        EditorButton(const char *label, const ImVec2 &size = ImVec2(0, 0), std::function<void()> onClick = nullptr)
            : m_label(label), m_size(size), m_onClick(onClick)
        {
        }

        void DrawElement() const override
        {
            if (ImGui::Button(m_label, m_size))
            {
                if (m_onClick)
                {
                    m_onClick();
                }
            }
        }

        // virtual void OnClick() = 0; TODO: Make this do what DrawElement currently does

    private:
        const char *m_label;
        ImVec2 m_size;
        
        // Callback function
        std::function<void()> m_onClick = nullptr;
    };
} // namespace BloxEngine::EditorUI

#endif // EDITOR_BUTTON_H