#include <gameinstance.h>

int main()
{
    // TODO: Check if this should be allocated on the heap
    GameInstance* game = new GameInstance(800, 450, "Game"); 
    // game.draw(); TODO: Implement this function

    delete game;

    return 0;
}