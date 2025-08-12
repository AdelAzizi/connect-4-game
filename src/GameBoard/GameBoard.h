#pragma once

#include "../Constants.h"
#include <vector>
#include <utility> // For std::pair

class GameBoard {
public:
    // The game board grid. Kept public to allow direct access from game logic
    // and AI, mirroring the original design.
    char board[ROWS][COLS];

    // Constructor
    GameBoard();

    void reset();
    bool isValidMove(int col) const;
    bool makeMove(int col, char playerId);
    bool isFull() const;
    int countFours(char playerId) const;
    char getCell(int row, int col) const;

    // New function to find all cells that are part of any four-in-a-row
    std::vector<std::pair<int, int>> findFourInARows(char playerId) const;

private:
    // Helper function to check for four-in-a-row in a specific direction
    bool checkDirection(int row, int col, int rowDir, int colDir, char playerId) const;
};
