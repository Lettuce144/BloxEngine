#include "EditorUI/EditorPanel.h"

namespace BloxEngine::EditorUI
{
    EditorPanel::EditorPanel()
    {
        m_panelName = "Editor Panel";
    }

    EditorPanel::~EditorPanel()
    {
    }

    void EditorPanel::Draw()
    {
        ImGui::Begin(m_panelName);
        for (const auto &element : m_elements)
        {
            element->DrawElement();
        }
        ImGui::End();
    }

    void EditorPanel::AddElement(std::unique_ptr<EditorElement> element)
    {
        m_elements.push_back(std::move(element));
    }
} // namespace BloxEngine