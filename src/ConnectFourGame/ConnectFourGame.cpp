#include "ConnectFourGame.h"
#include <iostream>

// --- Constructor and Main Game Loop ---

ConnectFourGame::ConnectFourGame()
    : window(sf::VideoMode(800, 800), "Connect Four"),
      gameState(MAIN_MENU),
      gameMode(TWO_PLAYER),
      currentPlayer(nullptr),
      firstGame(true) {

    if (!font.loadFromFile("ARIAL.TTF")) {
        std::cerr << "Failed to load font!" << std::endl;
        exit(1);
    }
    setupUI();
}

void ConnectFourGame::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// --- Setup ---

void ConnectFourGame::setupUI() {
    background.setSize(sf::Vector2f(window.getSize()));
    background.setFillColor(ColorPalette::DarkBlue);

    titleText.setString("CONNECT FOUR");
    titleText.setFont(font);
    titleText.setCharacterSize(60);
    titleText.setFillColor(ColorPalette::AccentYellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(window.getSize().x / 2.0f - titleText.getLocalBounds().width / 2.0f, 50);

    std::vector<std::string> menuOptions = {"1. Player vs Player", "2. Player vs Computer", "3. Exit"};
    for (size_t i = 0; i < menuOptions.size(); i++) {
        sf::Text item(menuOptions[i], font, 36);
        item.setFillColor(ColorPalette::LightGray);
        item.setPosition(window.getSize().x / 2.0f - item.getLocalBounds().width / 2.0f, 250 + i * 70);
        menuItems.push_back(item);
    }

    hoverIndicator.setRadius(10);
    hoverIndicator.setOutlineThickness(2);
    hoverIndicator.setOutlineColor(ColorPalette::AccentYellow);
    hoverIndicator.setFillColor(sf::Color::Transparent);
}

// --- Core Game Loop Methods ---

void ConnectFourGame::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        switch (gameState) {
            case MAIN_MENU:    handleMainMenu(event); break;
            case PLAYER_SETUP: handlePlayerSetup(event); break;
            case PLAYING:      handlePlaying(event); break;
            case GAME_OVER:
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    resetToMainMenu();
                }
                break;
        }
    }
}

void ConnectFourGame::update() {
    // If it's the computer's turn in a vs Computer match, make a move.
    if (gameState == PLAYING && gameMode == VS_COMPUTER && currentPlayer != nullptr && currentPlayer->isComputer()) {
        makeComputerMove();
    }
}

void ConnectFourGame::render() {
    window.clear();
    window.draw(background);

    switch (gameState) {
        case MAIN_MENU:    renderMainMenu(); break;
        case PLAYER_SETUP: renderPlayerSetup(); break;
        case PLAYING:      renderGame(); break;
        case GAME_OVER:    renderGame(); renderGameOver(); break;
    }

    window.display();
}

// --- Event Handling Sub-routines ---

void ConnectFourGame::handleMainMenu(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) startGame(TWO_PLAYER);
        else if (event.key.code == sf::Keyboard::Num2) startGame(VS_COMPUTER);
        else if (event.key.code == sf::Keyboard::Num3) window.close();
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (menuItems[i].getGlobalBounds().contains(mousePos)) {
                if (i == 0) startGame(TWO_PLAYER);
                else if (i == 1) startGame(VS_COMPUTER);
                else if (i == 2) window.close();
                break;
            }
        }
    }
}

void ConnectFourGame::handlePlayerSetup(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b') { // Backspace
            if (!inputText.empty()) {
                inputText.pop_back();
            }
        } else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter
            if (gettingPlayerName[0]) {
                playerNames[0] = inputText.empty() ? "Player 1" : inputText;
                inputText = "";
                gettingPlayerName[0] = false;
                if (gameMode == TWO_PLAYER) {
                    gettingPlayerName[1] = true;
                } else {
                    completePlayerSetup();
                }
            } else if (gettingPlayerName[1]) {
                playerNames[1] = inputText.empty() ? "Player 2" : inputText;
                completePlayerSetup();
            }
        } else if (isprint(event.text.unicode) && inputText.size() < 15) {
            inputText += static_cast<char>(event.text.unicode);
        }
    }
}

void ConnectFourGame::handlePlaying(const sf::Event& event) {
    // Only handle clicks if it's a human player's turn
    if (!currentPlayer->isComputer() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int col = event.mouseButton.x / CELL_SIZE;
        makeMove(col);
    }
}

// --- Game Flow and State Management ---

void ConnectFourGame::startGame(GameMode mode) {
    gameMode = mode;
    gameState = PLAYER_SETUP;
    gettingPlayerName[0] = true;
    gettingPlayerName[1] = false;
    inputText = "";
    board.reset();
}

void ConnectFourGame::completePlayerSetup() {
    players.clear();
    players.push_back(Player(playerNames[0], 'X', ColorPalette::AccentRed));
    players.push_back(Player(gameMode == TWO_PLAYER ? playerNames[1] : "Computer", 'O', ColorPalette::AccentYellow, gameMode == VS_COMPUTER));

    currentPlayer = &players[0];
    gameState = PLAYING;
}

void ConnectFourGame::makeMove(int col) {
    if (board.makeMove(col, currentPlayer->getId())) {
        updateGameState();
    }
}

void ConnectFourGame::updateGameState() {
    updateScores();
    if (board.isFull()) {
        gameState = GAME_OVER;
        // Find all winning lines to highlight them at the end of the game
        highlightedCells.clear();
        std::vector<std::pair<int, int>> p1_fours = board.findFourInARows('X');
        std::vector<std::pair<int, int>> p2_fours = board.findFourInARows('O');
        highlightedCells.insert(highlightedCells.end(), p1_fours.begin(), p1_fours.end());
        highlightedCells.insert(highlightedCells.end(), p2_fours.begin(), p2_fours.end());
    } else {
        switchPlayer();
    }
}

void ConnectFourGame::switchPlayer() {
    currentPlayer = (currentPlayer == &players[0]) ? &players[1] : &players[0];
}

void ConnectFourGame::resetToMainMenu() {
    gameState = MAIN_MENU;
    firstGame = true;
    board.reset();
    players.clear();
    currentPlayer = nullptr;
}

// --- Rendering Sub-routines ---

void ConnectFourGame::renderMainMenu() {
    window.draw(titleText);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto& item : menuItems) {
        bool hover = item.getGlobalBounds().contains(mousePos);
        item.setFillColor(hover ? ColorPalette::AccentYellow : ColorPalette::LightGray);
        item.setStyle(hover ? sf::Text::Bold : sf::Text::Regular);
        window.draw(item);
    }
}

void ConnectFourGame::renderPlayerSetup() {
    std::string promptStr = (gettingPlayerName[0] ? "Enter Player 1 name:" : "Enter Player 2 name:");
    promptStr += "\n\n> " + inputText + "_";

    promptText.setString(promptStr);
    promptText.setFont(font);
    promptText.setCharacterSize(30);
    promptText.setFillColor(ColorPalette::LightGray);
    promptText.setPosition(100, 300);
    window.draw(promptText);
}

void ConnectFourGame::renderGame() {
    // Draw board background
    sf::RectangleShape boardBg(sf::Vector2f(CELL_SIZE * COLS, CELL_SIZE * ROWS));
    boardBg.setPosition(50, 150);
    boardBg.setFillColor(ColorPalette::BoardColor);
    boardBg.setOutlineThickness(5);
    boardBg.setOutlineColor(ColorPalette::DarkGray);
    window.draw(boardBg);

    // Draw slots and tokens
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            sf::CircleShape slot(RADIUS - 5);
            slot.setPosition(j * CELL_SIZE + 60, i * CELL_SIZE + 160);
            slot.setFillColor(ColorPalette::DarkBlue);
            window.draw(slot);

            char cell = board.getCell(i, j);
            if (cell != '*') {
                sf::CircleShape token(RADIUS - 5);
                token.setPosition(j * CELL_SIZE + 60, i * CELL_SIZE + 160);
                token.setFillColor(cell == 'X' ? players[0].getColor() : players[1].getColor());
                window.draw(token);
            }
        }
    }

    // Draw winning highlights if the game is over
    if (gameState == GAME_OVER) {
        for (const auto& cell : highlightedCells) {
            sf::CircleShape highlight(RADIUS);
            highlight.setPosition(cell.second * CELL_SIZE + 50, cell.first * CELL_SIZE + 150);
            highlight.setFillColor(sf::Color(255, 255, 0, 70)); // Semi-transparent yellow
            window.draw(highlight);
        }
    }

    // Draw UI text
    if (currentPlayer != nullptr) {
        currentPlayerText.setString(currentPlayer->getName() + "'s Turn");
        currentPlayerText.setFont(font);
        currentPlayerText.setCharacterSize(28);
        currentPlayerText.setFillColor(currentPlayer->getColor());
        currentPlayerText.setPosition(50, 100);
        window.draw(currentPlayerText);
    }

    if (!players.empty()) {
        std::string scoreStr = players[0].getName() + ": " + std::to_string(players[0].getScore()) +
                               "   " + players[1].getName() + ": " + std::to_string(players[1].getScore());
        scoreText.setString(scoreStr);
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(ColorPalette::LightGray);
        scoreText.setPosition(50, 50);
        window.draw(scoreText);
    }

    // Draw hover indicator for human players
    if (gameState == PLAYING && currentPlayer != nullptr && !currentPlayer->isComputer()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        int col = (mousePos.x - 50) / CELL_SIZE;
        if (col >= 0 && col < COLS) {
            hoverIndicator.setPosition(col * CELL_SIZE + 50 + (CELL_SIZE / 2.0f) - hoverIndicator.getRadius(), 120);
            if(board.isValidMove(col)) {
                hoverIndicator.setOutlineColor(currentPlayer->getColor());
                window.draw(hoverIndicator);
            }
        }
    }
}

void ConnectFourGame::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    std::string winnerMsg;
    if (players[0].getScore() > players[1].getScore()) {
        winnerMsg = players[0].getName() + " wins!";
    } else if (players[1].getScore() > players[0].getScore()) {
        winnerMsg = players[1].getName() + " wins!";
    } else {
        winnerMsg = "It's a tie!";
    }

    gameOverText.setString("Game Over!\n" + winnerMsg + "\n\nPress 'R' to return to menu");
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(ColorPalette::AccentYellow);
    gameOverText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    window.draw(gameOverText);
}

// --- AI Logic ---

void ConnectFourGame::makeComputerMove() {
    int col = findBestMove();
    if (col != -1) {
        makeMove(col);
    }
}

int ConnectFourGame::findBestMove() {
    // 1. Check for an immediate winning move for the computer ('O').
    for (int col = 0; col < COLS; col++) {
        if (board.isValidMove(col)) {
            GameBoard tempBoard = board;
            tempBoard.makeMove(col, 'O');
            if (tempBoard.countFours('O') > board.countFours('O')) {
                return col;
            }
        }
    }

    // 2. Block the opponent's immediate winning move.
    for (int col = 0; col < COLS; col++) {
        if (board.isValidMove(col)) {
            GameBoard tempBoard = board;
            tempBoard.makeMove(col, 'X');
            if (tempBoard.countFours('X') > board.countFours('X')) {
                return col;
            }
        }
    }

    // 3. Use Minimax for the optimal move.
    auto [score, bestCol] = minimax(board, MAX_DEPTH, -1000000, 1000000, true);
    if (bestCol != -1) {
        return bestCol;
    }

    // 4. Fallback to a random valid move if no other move is found.
    std::vector<int> validMoves;
    for (int col = 0; col < COLS; col++) {
        if (board.isValidMove(col)) {
            validMoves.push_back(col);
        }
    }
    return validMoves.empty() ? -1 : validMoves[rand() % validMoves.size()];
}

bool ConnectFourGame::tryMove(int col, char playerId) {
    if (!board.isValidMove(col)) return false;

    GameBoard temp = board;
    temp.makeMove(col, playerId);
    return temp.countFours(playerId) > board.countFours(playerId);
}


std::pair<int, int> ConnectFourGame::minimax(GameBoard& currentBoard, int depth, int alpha, int beta, bool maximizing) {
    if (depth == 0 || currentBoard.isFull() || currentBoard.countFours('O') > 0 || currentBoard.countFours('X') > 0) {
        return {evaluateBoard(currentBoard), -1};
    }

    int bestScore = maximizing ? -1000000 : 1000000;
    int bestCol = -1;

    for (int col = 0; col < COLS; col++) {
        if (currentBoard.isValidMove(col)) {
            GameBoard tempBoard = currentBoard;
            tempBoard.makeMove(col, maximizing ? 'O' : 'X');

            auto [score, _] = minimax(tempBoard, depth - 1, alpha, beta, !maximizing);

            if (maximizing) {
                if (score > bestScore) {
                    bestScore = score;
                    bestCol = col;
                }
                alpha = std::max(alpha, bestScore);
            } else {
                if (score < bestScore) {
                    bestScore = score;
                    bestCol = col;
                }
                beta = std::min(beta, bestScore);
            }

            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
    }
    return {bestScore, bestCol};
}

int ConnectFourGame::evaluateBoard(const GameBoard& b) {
    // --- Line Score ---
    // Evaluates based on the number of 2-in-a-rows and 3-in-a-rows.
    // A 4-in-a-row is an immediate win/loss condition.
    int line_score = 0;
    if (b.countFours('O') > 0) return 100000; // AI win is highest priority
    if (b.countFours('X') > 0) return -100000; // Player win is lowest priority

    line_score += countThrees(b, 'O') * 100;
    line_score += countTwos(b, 'O') * 10;
    line_score -= countThrees(b, 'X') * 100; // Penalize player's threats
    line_score -= countTwos(b, 'X') * 10;

    // --- Positional Score ---
    // Adds a small bonus for controlling the center columns, which are more strategic.
    int positional_score = 0;
    const int column_scores[] = {1, 2, 3, 4, 3, 2, 1}; // Center column (index 3) is most valuable

    for (int c = 0; c < COLS; ++c) {
        for (int r = 0; r < ROWS; ++r) {
            if (b.getCell(r, c) == 'O') {
                positional_score += column_scores[c];
            } else if (b.getCell(r, c) == 'X') {
                positional_score -= column_scores[c];
            }
        }
    }

    return line_score + positional_score;
}

int ConnectFourGame::countThrees(const GameBoard& b, char playerId) {
    int count = 0;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            count += checkLine(b, playerId, r, c, 3);
        }
    }
    return count;
}

int ConnectFourGame::countTwos(const GameBoard& b, char playerId) {
    int count = 0;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            count += checkLine(b, playerId, r, c, 2);
        }
    }
    return count;
}

int ConnectFourGame::checkLine(const GameBoard& b, char playerId, int r, int c, int needed) {
    int count = 0;
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (auto& dir : directions) {
        int playerPieces = 0;
        int emptySlots = 0;
        bool possible = true;
        for (int i = 0; i < 4; i++) {
            int curR = r + i * dir[0];
            int curC = c + i * dir[1];
            if (curR < 0 || curR >= ROWS || curC < 0 || curC >= COLS) {
                possible = false;
                break;
            }
            if (b.getCell(curR, curC) == playerId) {
                playerPieces++;
            } else if (b.getCell(curR, curC) == '*') {
                emptySlots++;
            }
        }
        if (possible && playerPieces == needed && emptySlots == (4 - needed)) {
            count++;
        }
    }
    return count;
}

// --- Scoring ---

void ConnectFourGame::updateScores() {
    if(players.size() == 2) {
        players[0].setScore(board.countFours('X'));
        players[1].setScore(board.countFours('O'));
    }
}
