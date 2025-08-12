#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <utility> // For std::pair

#include "../Constants.h"
#include "../Player/Player.h"
#include "../GameBoard/GameBoard.h"

class ConnectFourGame {
public:
    ConnectFourGame();
    void run();

private:
    // --- Member Variables ---

    // Window and Core UI Elements
    sf::RenderWindow window;
    sf::Font font;
    sf::RectangleShape background;
    sf::Text titleText;
    sf::CircleShape hoverIndicator;

    // UI Text Objects
    std::vector<sf::Text> menuItems;
    sf::Text scoreText;
    sf::Text currentPlayerText;
    sf::Text gameOverText;
    sf::Text promptText; // For player setup

    // Game State and Logic
    GameBoard board;
    std::vector<Player> players;
    Player* currentPlayer;
    GameState gameState;
    GameMode gameMode;
    bool firstGame; // Kept to match original logic

    // Player Name Input
    std::string playerNames[2];
    std::string inputText;
    bool gettingPlayerName[2];

    // UX Enhancement
    std::vector<std::pair<int, int>> highlightedCells;

    // --- Private Methods ---

    // Core Game Loop Methods
    void setupUI();
    void handleEvents();
    void update();
    void render();

    // Event Handling Sub-routines
    void handleMainMenu(const sf::Event& event);
    void handlePlayerSetup(const sf::Event& event);
    void handlePlaying(const sf::Event& event);

    // Game Flow and State Management
    void startGame(GameMode mode);
    void completePlayerSetup();
    void makeMove(int col);
    void updateGameState();
    void switchPlayer();
    void resetToMainMenu();

    // Rendering Sub-routines
    void renderMainMenu();
    void renderPlayerSetup();
    void renderGame();
    void renderGameOver();

    // AI Logic
    void makeComputerMove();
    int findBestMove();
    bool tryMove(int col, char playerId);
    std::pair<int, int> minimax(GameBoard& board, int depth, int alpha, int beta, bool maximizing);
    int evaluateBoard(const GameBoard& board);
    int countThrees(const GameBoard& board, char playerId);
    int countTwos(const GameBoard& board, char playerId);
    int checkLine(const GameBoard& board, char playerId, int row, int col, int needed);

    // Scoring
    void updateScores();
};
