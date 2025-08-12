#include "ConnectFourGame/ConnectFourGame.h"
#include <cstdlib> // For srand
#include <ctime>   // For time

int main() {
    // Seed the random number generator, used for the AI's fallback random move.
    // This ensures different random moves each time the game is played.
    srand(static_cast<unsigned int>(time(0)));

    // Create and run the game instance.
    ConnectFourGame game;
    game.run();

    return 0;
}
