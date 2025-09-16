#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <string>
#include <functional>
#include <vector>

// Contains mostly 'wrapper' functions for external libraries
namespace BloxEngine::Engine
{
    typedef struct
    {
        std::string FilterName;
        std::string FilterExtension;
    } FileFilter;

    /**
     * @brief Returns the time in seconds since the last frame.
     *
     * This function is a wrapper for the Raylib GetFrameTime() function, which returns the time in seconds since the last
     * frame.
     *
     * @return The time in seconds since the last frame.
     */
    float GetDeltaTime();

    /**
     * @brief Opens a file dialog with specified filters and calls the provided callback with the selected file path.
     *
     * This function initializes the Native File Dialog (NFD) library, converts the provided file filters to the format
     * required by NFD, and opens a file dialog. If a file is selected, the provided callback function is called with
     * the selected file path. The function returns true if a file is successfully selected, and false otherwise.
     *
     * @param successCallback A callback function that takes a const std::string& parameter. This function is called with
     *                        the selected file path if a file is successfully selected.
     * @param filters A vector of FileFilter structures that specify the file types to filter in the file dialog. Each
     *                FileFilter contains a FilterName and a FilterExtension.
     * @return true if a file is successfully selected, false if the dialog is canceled or an error occurs.
     *
     * @note The NFD_Init() and NFD_Quit() functions are called within this function. For better performance and to avoid
     *       repeated initialization and cleanup, consider moving these calls to the main thread or a higher-level
     *       initialization and cleanup routine.
     *
     * Example usage:
     * @code
     * std::vector<BloxEngine::Engine::FileFilter> filters = {
     *     {"Text Files", "txt"},
     *     {"Image Files", "png,jpg,jpeg"}
     * };
     * BloxEngine::Engine::OpenFile([](const std::string &filePath) {
     *     std::cout << "Selected file: " << filePath << std::endl;
     * }, filters);
     * @endcode
     */
    bool OpenFile(std::function<void(const std::string &)> successCallack, const std::vector<FileFilter> &filters);
    
}
#endif // ENGINE_H