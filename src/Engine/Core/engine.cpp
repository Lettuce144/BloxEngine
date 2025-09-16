#include "Core/engine.h"

#include <nfd.h>
#include <raylib-cpp.hpp>

namespace BloxEngine::Engine
{
    float GetDeltaTime()
    {
        return GetFrameTime();
    }

    bool OpenFile(std::function<void(const std::string &)> successCallack, const std::vector<FileFilter> &filters)
    {
        NFD_Init(); // TODO: Move this later to the main thread, of course we don't have multithreading yet

        // Convert FileFilter vector to nfdu8filteritem_t array
        std::vector<nfdu8filteritem_t> filterItems;
        for (const auto &filter : filters)
        {
            nfdu8filteritem_t filterItem = {filter.FilterName.c_str(), filter.FilterExtension.c_str()};
            filterItems.push_back(filterItem);
        }

        nfdu8char_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialogU8(&outPath, filterItems.data(), filterItems.size(), NULL);

        if (result == NFD_OKAY)
        {
            successCallack(static_cast<std::string>(outPath));
            NFD_FreePathU8(outPath);
            NFD_Quit();
            return true;
        }
        else if (result == NFD_CANCEL || result == NFD_ERROR)
        {
            NFD_Quit();
            return false;
        }
    }

} // namespace BloxEngine::Engine