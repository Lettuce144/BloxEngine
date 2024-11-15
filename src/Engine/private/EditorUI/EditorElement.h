#ifndef EDITOR_ELEMENT_HPP
#define EDITOR_ELEMENT_HPP
#pragma once

namespace BloxEngine::EditorUI
{

    class EditorElement
    {
    public:
        virtual ~EditorElement() = default;
        virtual void DrawElement() const = 0;
    };

} // namespace BloxEngine::EditorUI
#endif // EDITOR_ELEMENT_HPP