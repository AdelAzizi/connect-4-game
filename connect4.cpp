#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace sf;
using namespace std;

namespace ColorPalette {
    const Color DarkBlue(26, 42, 108), LightBlue(100, 149, 237), 
                AccentYellow(255, 215, 0), AccentRed(220, 20, 60),
                LightGray(230, 230, 230), DarkGray(50, 50, 50),
                BoardColor(30, 144, 255);
}

enum GameState { MAIN_MENU, PLAYER_SETUP, PLAYING, GAME_OVER };
enum GameMode { TWO_PLAYER, VS_COMPUTER };

class Player {
private:
    string name;
    char id;
    int score;
    Color color;
    bool computerFlag;

public:
    Player(const string& name = "", char id = ' ', const Color& color = Color::White, bool isComputer = false)
        : name(name), id(id), score(0), color(color), computerFlag(isComputer) {}

    string getName() const { return name; }
    char getId() const { return id; }
    int getScore() const { return score; }
    Color getColor() const { return color; }
    bool isComputer() const { return computerFlag; }

    void setName(const string& newName) { name = newName; }
    void setScore(int newScore) { score = newScore; }
};

class GameBoard {
public:
    static const int ROWS = 6;
    static const int COLS = 7;
    char board[ROWS][COLS];

    GameBoard() { reset(); }

    void reset() {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                board[i][j] = '*';
    }

    bool isValidMove(int col) const {
        return (col >= 0 && col < COLS && board[0][col] == '*');
    }

    bool makeMove(int col, char playerId) {
        if (!isValidMove(col)) return false;

        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == '*') {
                board[row][col] = playerId;
                return true;
            }
        }
        return false;
    }

    bool isFull() const {
        for (int col = 0; col < COLS; col++)
            if (board[0][col] == '*') return false;
        return true;
    }

    int countFours(char playerId) const {
        int count = 0;
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (checkDirection(row, col, 0, 1, playerId)) count++;
                if (checkDirection(row, col, 1, 0, playerId)) count++;
                if (checkDirection(row, col, 1, 1, playerId)) count++;
                if (checkDirection(row, col, 1, -1, playerId)) count++;
            }
        }
        return count;
    }

    char getCell(int row, int col) const { return board[row][col]; }

private:
    bool checkDirection(int row, int col, int rowDir, int colDir, char playerId) const {
        for (int i = 0; i < 4; i++) {
            int r = row + i*rowDir, c = col + i*colDir;
            if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != playerId)
                return false;
        }
        return true;
    }
};

class ConnectFourGame {
private:
    static const int CELL_SIZE = 100;
    static const int RADIUS = 40;
    static const int MAX_DEPTH = 5;

    RenderWindow window;
    Font font;
    GameBoard board;
    vector<Player> players;
    Player* currentPlayer;
    GameState gameState;
    GameMode gameMode;
    bool firstGame;
    
    string playerNames[2];
    string inputText;
    bool gettingPlayerName[2];
    
    RectangleShape background;
    Text titleText;
    vector<Text> menuItems;
    Text scoreText;
    Text currentPlayerText;
    Text gameOverText;
    CircleShape hoverIndicator;

public:
    ConnectFourGame() : window(VideoMode(800, 800), "Connect Four"), 
                      gameState(MAIN_MENU), gameMode(TWO_PLAYER),
                      currentPlayer(nullptr), firstGame(true) {
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Failed to load font!" << endl;
            exit(1);
        }
        setupUI();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void setupUI() {
        background.setSize(Vector2f(window.getSize()));
        background.setFillColor(ColorPalette::DarkBlue);

        titleText.setString("CONNECT FOUR");
        titleText.setFont(font);
        titleText.setCharacterSize(60);
        titleText.setFillColor(ColorPalette::AccentYellow);
        titleText.setStyle(Text::Bold);
        titleText.setPosition(window.getSize().x/2 - titleText.getLocalBounds().width/2, 50);

        vector<string> menuOptions = {"1. Player vs Player", "2. Player vs Computer", "3. Exit"};
        for (size_t i = 0; i < menuOptions.size(); i++) {
            Text item(menuOptions[i], font, 36);
            item.setFillColor(ColorPalette::LightGray);
            item.setPosition(window.getSize().x/2 - item.getLocalBounds().width/2, 250 + i * 70);
            menuItems.push_back(item);
        }

        hoverIndicator.setRadius(10);
        hoverIndicator.setOutlineThickness(2);
        hoverIndicator.setOutlineColor(ColorPalette::AccentYellow);
    }

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            switch (gameState) {
                case MAIN_MENU: handleMainMenu(event); break;
                case PLAYER_SETUP: handlePlayerSetup(event); break;
                case PLAYING: handlePlaying(event); break;
                case GAME_OVER: 
                    if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
                        resetToMainMenu();
                    break;
            }
        }
    }

    void handleMainMenu(const Event& event) {
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Num1) startGame(TWO_PLAYER);
            else if (event.key.code == Keyboard::Num2) startGame(VS_COMPUTER);
            else if (event.key.code == Keyboard::Num3) window.close();
        }
        
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
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

    void startGame(GameMode mode) {
        gameMode = mode;
        gameState = PLAYER_SETUP;
        gettingPlayerName[0] = true;
        gettingPlayerName[1] = false;
        inputText = "";
    }

    void handlePlayerSetup(const Event& event) {
        if (event.type == Event::TextEntered) {
            if (event.text.unicode == '\b') {
                if (!inputText.empty()) inputText.pop_back();
            }
            else if (event.text.unicode == '\r') {
                if (gettingPlayerName[0]) {
                    playerNames[0] = inputText.empty() ? "Player 1" : inputText;
                    inputText = "";
                    gettingPlayerName[0] = false;
                    if (gameMode == TWO_PLAYER) gettingPlayerName[1] = true;
                    else completePlayerSetup();
                }
                else if (gettingPlayerName[1]) {
                    playerNames[1] = inputText.empty() ? "Player 2" : inputText;
                    completePlayerSetup();
                }
            }
            else if (isprint(event.text.unicode) && inputText.size() < 15) {
                inputText += static_cast<char>(event.text.unicode);
            }
        }
    }

    void completePlayerSetup() {
        players = {
            Player(playerNames[0], 'X', ColorPalette::AccentRed),
            Player(gameMode == TWO_PLAYER ? playerNames[1] : "Computer", 'O', 
                  ColorPalette::AccentYellow, gameMode == VS_COMPUTER)
        };
        currentPlayer = &players[0];
        gameState = PLAYING;
    }

    void handlePlaying(const Event& event) {
        if (!currentPlayer->isComputer() && event.type == Event::MouseButtonPressed && 
            event.mouseButton.button == Mouse::Left) {
            makeMove(event.mouseButton.x / CELL_SIZE);
        }
    }

    void makeMove(int col) {
        if (board.makeMove(col, currentPlayer->getId())) {
            updateGameState();
        }
    }

    void makeComputerMove() {
        int col = findBestMove();
        if (col != -1) {
            board.makeMove(col, currentPlayer->getId());
            updateGameState();
        }
    }

    void updateGameState() {
        updateScores();
        if (board.isFull()) gameState = GAME_OVER;
        else {
            switchPlayer();
            if (gameMode == VS_COMPUTER && currentPlayer->isComputer()) {
                makeComputerMove();
            }
        }
    }

    void updateScores() {
        players[0].setScore(board.countFours('X'));
        players[1].setScore(board.countFours('O'));
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == &players[0]) ? &players[1] : &players[0];
    }

    int findBestMove() {
        // Check for immediate win
        for (int col = 0; col < GameBoard::COLS; col++) {
            if (tryMove(col, 'O')) return col;
        }
        
        // Block opponent's immediate win
        for (int col = 0; col < GameBoard::COLS; col++) {
            if (tryMove(col, 'X')) return col;
        }
        
        // Use Minimax for best move
        auto [score, bestCol] = minimax(board, MAX_DEPTH, -1000000, 1000000, true);
        if (bestCol != -1) return bestCol;
        
        // Random valid move as fallback
        vector<int> validMoves;
        for (int col = 0; col < GameBoard::COLS; col++) {
            if (board.isValidMove(col)) validMoves.push_back(col);
        }
        return validMoves.empty() ? -1 : validMoves[rand() % validMoves.size()];
    }

    bool tryMove(int col, char playerId) {
        if (!board.isValidMove(col)) return false;
        
        GameBoard temp = board;
        temp.makeMove(col, playerId);
        return temp.countFours(playerId) > board.countFours(playerId);
    }

    pair<int, int> minimax(GameBoard& board, int depth, int alpha, int beta, bool maximizing) {
        if (depth == 0 || board.isFull() || board.countFours('O') || board.countFours('X')) {
            return {evaluateBoard(board), -1};
        }
        
        int bestScore = maximizing ? -1000000 : 1000000;
        int bestCol = -1;
        
        for (int col = 0; col < GameBoard::COLS; col++) {
            if (board.isValidMove(col)) {
                GameBoard temp = board;
                temp.makeMove(col, maximizing ? 'O' : 'X');
                
                auto [score, _] = minimax(temp, depth-1, alpha, beta, !maximizing);
                
                if ((maximizing && score > bestScore) || (!maximizing && score < bestScore)) {
                    bestScore = score;
                    bestCol = col;
                }
                
                if (maximizing) alpha = max(alpha, bestScore);
                else beta = min(beta, bestScore);
                
                if (beta <= alpha) break;
            }
        }
        return {bestScore, bestCol};
    }

    int evaluateBoard(const GameBoard& board) {
        int score = 0;
        
        if (board.countFours('O')) return 100000;
        if (board.countFours('X')) return -100000;
        
        score += countThrees(board, 'O') * 100;
        score += countTwos(board, 'O') * 10;
        score -= countThrees(board, 'X') * 100;
        score -= countTwos(board, 'X') * 10;
        
        return score;
    }

    int countThrees(const GameBoard& board, char playerId) {
        int count = 0;
        for (int row = 0; row < GameBoard::ROWS; row++) {
            for (int col = 0; col < GameBoard::COLS; col++) {
                count += checkLine(board, playerId, row, col, 3);
            }
        }
        return count;
    }

    int countTwos(const GameBoard& board, char playerId) {
        int count = 0;
        for (int row = 0; row < GameBoard::ROWS; row++) {
            for (int col = 0; col < GameBoard::COLS; col++) {
                count += checkLine(board, playerId, row, col, 2);
            }
        }
        return count;
    }

    int checkLine(const GameBoard& board, char playerId, int row, int col, int needed) {
        int count = 0;
        int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};
        
        for (auto& dir : directions) {
            int player = 0, empty = 0;
            for (int i = 0; i < 4; i++) {
                int r = row + i*dir[0], c = col + i*dir[1];
                if (r < 0 || r >= GameBoard::ROWS || c < 0 || c >= GameBoard::COLS) break;
                
                if (board.getCell(r, c) == playerId) player++;
                else if (board.getCell(r, c) == '*') empty++;
            }
            if (player == needed && empty == (4-needed)) count++;
        }
        return count;
    }

    void resetToMainMenu() {
        gameState = MAIN_MENU;
        firstGame = true;
        board.reset();
    }

    void update() {
        if (gameState == PLAYING && currentPlayer->isComputer()) {
            makeComputerMove();
        }
    }

    void render() {
        window.clear();
        window.draw(background);
        
        switch (gameState) {
            case MAIN_MENU: renderMainMenu(); break;
            case PLAYER_SETUP: renderPlayerSetup(); break;
            case PLAYING: renderGame(); break;
            case GAME_OVER: renderGame(); renderGameOver(); break;
        }
        
        window.display();
    }

    void renderMainMenu() {
        window.draw(titleText);
        
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        for (auto& item : menuItems) {
            bool hover = item.getGlobalBounds().contains(mousePos);
            item.setFillColor(hover ? ColorPalette::AccentYellow : ColorPalette::LightGray);
            item.setStyle(hover ? Text::Bold : Text::Regular);
            window.draw(item);
        }
    }

    void renderPlayerSetup() {
        string promptText = (gettingPlayerName[0] ? "Enter Player 1 name:" : "Enter Player 2 name:");
        promptText += "\n" + inputText + "_";
        Text prompt(promptText, font, 30);
        prompt.setFillColor(ColorPalette::LightGray);
        prompt.setPosition(100, 300);
        window.draw(prompt);
    }

    void renderGame() {
        // Draw board
        RectangleShape boardBg(Vector2f(CELL_SIZE * GameBoard::COLS, CELL_SIZE * GameBoard::ROWS));
        boardBg.setPosition(50, 150);
        boardBg.setFillColor(ColorPalette::BoardColor);
        boardBg.setOutlineThickness(5);
        boardBg.setOutlineColor(ColorPalette::DarkGray);
        window.draw(boardBg);
        
        // Draw slots and tokens
        for (int i = 0; i < GameBoard::ROWS; i++) {
            for (int j = 0; j < GameBoard::COLS; j++) {
                CircleShape slot(RADIUS - 5);
                slot.setPosition(j * CELL_SIZE + 60, i * CELL_SIZE + 160);
                slot.setFillColor(ColorPalette::DarkBlue);
                window.draw(slot);
                
                char cell = board.getCell(i, j);
                if (cell == 'X' || cell == 'O') {
                    CircleShape token(RADIUS);
                    token.setPosition(j * CELL_SIZE + 60, i * CELL_SIZE + 160);
                    token.setFillColor(cell == 'X' ? players[0].getColor() : players[1].getColor());
                    window.draw(token);
                }
            }
        }
        
        // Draw UI text
        currentPlayerText.setString(currentPlayer->getName() + "'s Turn");
        currentPlayerText.setFont(font);
        currentPlayerText.setCharacterSize(28);
        currentPlayerText.setFillColor(currentPlayer->getColor());
        currentPlayerText.setPosition(50, 100);
        window.draw(currentPlayerText);
        
        string scoreStr = players[0].getName() + ": " + to_string(players[0].getScore()) + 
                         "   " + players[1].getName() + ": " + to_string(players[1].getScore());
        scoreText.setString(scoreStr);
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(ColorPalette::LightGray);
        scoreText.setPosition(50, 50);
        window.draw(scoreText);
        
        // Draw hover indicator
        if (gameState == PLAYING && !currentPlayer->isComputer()) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            int col = mousePos.x / CELL_SIZE;
            if (col >= 0 && col < GameBoard::COLS) {
                hoverIndicator.setPosition(col * CELL_SIZE + 100, 100);
                window.draw(hoverIndicator);
            }
        }
    }

    void renderGameOver() {
        RectangleShape overlay(Vector2f(window.getSize()));
        overlay.setFillColor(Color(0, 0, 0, 180));
        window.draw(overlay);
        
        string winnerMsg;
        if (players[0].getScore() > players[1].getScore()) {
            winnerMsg = players[0].getName() + " wins!";
        } else if (players[1].getScore() > players[0].getScore()) {
            winnerMsg = players[1].getName() + " wins!";
        } else {
            winnerMsg = "It's a tie!";
        }
        
        gameOverText.setString("Game Over!\n" + winnerMsg + "\nPress R to return to menu");
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(ColorPalette::AccentYellow);
        gameOverText.setPosition(
            window.getSize().x/2 - gameOverText.getLocalBounds().width/2,
            window.getSize().y/2 - 50
        );
        window.draw(gameOverText);
    }
};

int main() {
    ConnectFourGame game;
    game.run();
    return 0;
}