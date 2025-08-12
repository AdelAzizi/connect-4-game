#pragma once

#include <SFML/Graphics.hpp>

// UI and Board Dimensions
const int ROWS = 6;
const int COLS = 7;
const int CELL_SIZE = 100;
const int RADIUS = 40;

// AI Settings
const int MAX_DEPTH = 5;

// Game States Enum
enum GameState {
    MAIN_MENU,
    PLAYER_SETUP,
    PLAYING,
    GAME_OVER
};

// Game Modes Enum
enum GameMode {
    TWO_PLAYER,
    VS_COMPUTER
};

// UI Color Palette
namespace ColorPalette {
    const sf::Color DarkBlue(26, 42, 108);
    const sf::Color LightBlue(100, 149, 237);
    const sf::Color AccentYellow(255, 215, 0);
    const sf::Color AccentRed(220, 20, 60);
    const sf::Color LightGray(230, 230, 230);
    const sf::Color DarkGray(50, 50, 50);
    const sf::Color BoardColor(30, 144, 255);
}
