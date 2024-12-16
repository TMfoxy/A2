#include <iostream>
#include "BoardGame_Classes.h"
using namespace std;

// Define the smaller Tic Tac Toe board
class SmallBoard : public Board<char> {
public:
    SmallBoard() {
        rows = columns = 3;
        board = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new char[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = ' '; // Initialize empty cells
            }
        }
    }

    bool update_board(int x, int y, char symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == ' ') {
            board[x][y] = symbol;
            ++n_moves;
            return true;
        }
        return false;
    }

    void display_board() override {
        cout << "    0   1   2" << endl; // Column numbering
        for (int i = 0; i < rows; ++i) {
            cout << i << " | "; // Row numbering
            for (int j = 0; j < columns; ++j) {
                cout << board[i][j];
                if (j < columns - 1) cout << " | ";
            }
            cout << endl;
            if (i < rows - 1) cout << "  ---------" << endl;
        }
    }

    char get_cell(int x, int y) const {
        return board[x][y];
    }

    bool is_win() override {
        // Check rows, columns, and diagonals for a winner
        for (int i = 0; i < rows; ++i) {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return true;
        }
        for (int j = 0; j < columns; ++j) {
            if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j])
                return true;
        }
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return true;
        if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return true;
        return false;
    }

    bool is_draw() override {
        return n_moves == rows * columns && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

// Define the Ultimate Tic Tac Toe board
class UltimateBoard : public Board<char> {
private:
    SmallBoard* subBoards[3][3];
    char mainBoard[3][3];

public:
    UltimateBoard() {
        rows = columns = 3;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                subBoards[i][j] = new SmallBoard();
                mainBoard[i][j] = ' '; // Initialize main board as empty
            }
        }
    }

    bool update_board(int x, int y, char symbol) override {
        int subX = x / 3, subY = y / 3; // Determine which sub-board
        int localX = x % 3, localY = y % 3; // Local coordinates within the sub-board

        if (subBoards[subX][subY]->update_board(localX, localY, symbol)) {
            if (subBoards[subX][subY]->is_win() && mainBoard[subX][subY] == ' ') {
                mainBoard[subX][subY] = symbol;
            }
            return true;
        }
        return false;
    }

    void display_board() override {
        cout << "          0   1   2   3   4   5   6   7   8" << endl; // Global column numbering
        cout << "        -------------------------------------" << endl;
        for (int subX = 0; subX < rows; ++subX) {
            for (int localRow = 0; localRow < 3; ++localRow) {
                cout << subX * 3 + localRow << " | "; // Row numbering for ultimate board
                for (int subY = 0; subY < columns; ++subY) {
                    for (int localCol = 0; localCol < 3; ++localCol) {
                        cout << subBoards[subX][subY]->get_cell(localRow, localCol);
                        if (localCol < 2) cout << " | ";
                    }
                    cout << "   ";
                }
                cout << endl;
            }
            cout << "        -------------------------------------" << endl;
        }
    }

    bool is_win() override {
        // Check rows, columns, and diagonals for a winner
        for (int i = 0; i < rows; ++i) {
            if (mainBoard[i][0] != ' ' && mainBoard[i][0] == mainBoard[i][1] && mainBoard[i][1] == mainBoard[i][2])
                return true;
        }
        for (int j = 0; j < columns; ++j) {
            if (mainBoard[0][j] != ' ' && mainBoard[0][j] == mainBoard[1][j] && mainBoard[1][j] == mainBoard[2][j])
                return true;
        }
        if (mainBoard[0][0] != ' ' && mainBoard[0][0] == mainBoard[1][1] && mainBoard[1][1] == mainBoard[2][2])
            return true;
        if (mainBoard[0][2] != ' ' && mainBoard[0][2] == mainBoard[1][1] && mainBoard[1][1] == mainBoard[2][0])
            return true;
        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (mainBoard[i][j] == ' ') return false;
            }
        }
        return !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

// Define a concrete player class
class HumanPlayer : public Player<char> {
public:
    HumanPlayer(string n, char s) : Player<char>(n, s) {}

    void getmove(int& x, int& y) override {
        cout << getname() << ", enter your move (row and column): ";
        cin >> x >> y;
    }
};

// Main function
int main() {
    UltimateBoard board;
    Player<char>* players[2] = { new HumanPlayer("Player 1", 'X'), new HumanPlayer("Player 2", 'O') };
    GameManager<char> gameManager(&board, players);
    gameManager.run();

    return 0;
}
