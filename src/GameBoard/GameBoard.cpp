#include "GameBoard.h"
#include <set>

GameBoard::GameBoard() {
    reset();
}

void GameBoard::reset() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '*';
        }
    }
}

bool GameBoard::isValidMove(int col) const {
    // A move is valid if the column is within bounds and the top cell is empty.
    return (col >= 0 && col < COLS && board[0][col] == '*');
}

bool GameBoard::makeMove(int col, char playerId) {
    if (!isValidMove(col)) {
        return false;
    }

    // Find the first empty row from the bottom.
    for (int row = ROWS - 1; row >= 0; row--) {
        if (board[row][col] == '*') {
            board[row][col] = playerId;
            return true;
        }
    }
    return false; // Should not happen if isValidMove is checked
}

bool GameBoard::isFull() const {
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == '*') {
            return false; // Found an empty slot, board is not full.
        }
    }
    return true;
}

std::vector<std::pair<int, int>> GameBoard::findFourInARows(char playerId) const {
    std::set<std::pair<int, int>> cell_set;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            // Check Horizontal
            if (checkDirection(row, col, 0, 1, playerId)) {
                for (int i = 0; i < 4; i++) cell_set.insert({row, col + i});
            }
            // Check Vertical
            if (checkDirection(row, col, 1, 0, playerId)) {
                for (int i = 0; i < 4; i++) cell_set.insert({row + i, col});
            }
            // Check Diagonal (down-right)
            if (checkDirection(row, col, 1, 1, playerId)) {
                for (int i = 0; i < 4; i++) cell_set.insert({row + i, col + i});
            }
            // Check Diagonal (down-left)
            if (checkDirection(row, col, 1, -1, playerId)) {
                for (int i = 0; i < 4; i++) cell_set.insert({row + i, col - i});
            }
        }
    }

    // Convert set to vector to return
    std::vector<std::pair<int, int>> winning_cells;
    for(const auto& cell : cell_set) {
        winning_cells.push_back(cell);
    }
    return winning_cells;
}

int GameBoard::countFours(char playerId) const {
    int count = 0;
    // Iterate through each cell as a potential start of a four-in-a-row
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            // Check all four directions from each cell
            if (checkDirection(row, col, 0, 1, playerId)) count++;  // Horizontal
            if (checkDirection(row, col, 1, 0, playerId)) count++;  // Vertical
            if (checkDirection(row, col, 1, 1, playerId)) count++;  // Diagonal (down-right)
            if (checkDirection(row, col, 1, -1, playerId)) count++; // Diagonal (down-left)
        }
    }
    return count;
}

char GameBoard::getCell(int row, int col) const {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return board[row][col];
    }
    return '\0'; // Return null character for out-of-bounds access
}

bool GameBoard::checkDirection(int row, int col, int rowDir, int colDir, char playerId) const {
    for (int i = 0; i < 4; i++) {
        int r = row + i * rowDir;
        int c = col + i * colDir;
        // Check if the cell is within bounds and belongs to the player
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != playerId) {
            return false;
        }
    }
    return true;
}
