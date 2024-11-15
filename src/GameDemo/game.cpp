#include <gameinstance.h>

int main()
{
    // TODO: Check if this should be allocated on the heap
    GameInstance* game = new GameInstance(1360, 768, "Blox Engine Editor");

    delete game;

    return 0;
}