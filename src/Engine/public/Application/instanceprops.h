#ifndef INSTANCEPROPS_H
#define INSTANCEPROPS_H
#pragma once

#include <string>
#include <raylib.h>

namespace BloxEngine
{

    /**
     * The props of an instance, such as the app name,
     * background color and window props (Height, Width, Title)
     */
    struct InstanceProperties
    {
        std::string appName;
        int width;
        int height;
        bool fullscreen;

        Color clearcolor;

        InstanceProperties(const std::string &name = "App", int instanceWidth = 800, int instanceHeight = 600, bool fullScreen = false, Color clearColor = RAYWHITE)
            : appName(name), width(instanceWidth), height(instanceHeight), fullscreen(fullScreen), clearcolor(clearColor) {}
    };
} // namespace BloxEngine

#endif // INSTANCEPROPS_H
