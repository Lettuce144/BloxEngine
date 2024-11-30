#ifndef BLOXENGINE_ENTRYPOINT_HPP
#define BLOXENGINE_ENTRYPOINT_HPP
#pragma once

#include "gameinstance.h"

extern BloxEngine::GameInstance *CreateGameInstance();

int main(int argc, char **argv)
{
    BloxEngine::GameInstance* game = BloxEngine::CreateGameInstance();
    game->Run();
    delete game;

    return 0;
}

#endif // BLOXENGINE_ENTRYPOINT_HPP