#ifndef BLOXENGINE_ENTRYPOINT_HPP
#define BLOXENGINE_ENTRYPOINT_HPP
#pragma once

#include "gameinstance.h"

extern BloxEngine::GameInstance *CreateGameInstance();

int main(int argc, char **argv)
{
    // TODO: Use a shared pointer here
    BloxEngine::GameInstance* game = BloxEngine::CreateGameInstance();
    game->OnAttach();
    game->Run();
    game->OnDetach();
    delete game;

    return 0;
}

#endif // BLOXENGINE_ENTRYPOINT_HPP