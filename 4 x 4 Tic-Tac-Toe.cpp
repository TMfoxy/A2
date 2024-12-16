#include "BoardGame_Classes.h"
#include <iostream>
using namespace std;
class TicTacToeBoard : public Board<char> {
public:
    TicTacToeBoard() {
        rows = 4;
        columns = 4;
        board = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new char[columns];
            for (int j = 0; j < columns; ++j) {
                board[i][j] = '*';
            }
        }
        //this is raws and columns will be started in the game
        board[0][0] = 'O';
        board[0][1] = 'X';
        board[0][2] = 'O';
        board[0][3] = 'X';
        board[3][0] = 'X';
        board[3][1] = 'O';
        board[3][2] = 'X';
        board[3][3] = 'O';
    }
    //there is update the board when the user input his raws and columns
    bool update_board(int x, int y, char symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == '*') {
            board[x][y] = symbol;
            return true;
        }
        return false;
    }
    void display_board() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }
    bool is_win() override {
        // Check rows, columns, and diagonals for three in a row to define the winner
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] != '*' && (
                    (j <= columns - 3 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2]) ||
                    (i <= rows - 3 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j]) ||
                    (i <= rows - 3 && j <= columns - 3 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2]) ||
                    (i >= 2 && j <= columns - 3 && board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2])
                )) {
                    return true;
                }
            }
        }
        return false;
    }
    bool is_draw() override {
        if (is_win()) return false;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == '*') return false;
            }
        }
        return true;
    }
    bool game_is_over() override {
        return is_win() || is_draw();
    }
};
class player : public Player<char> {
public:
    player(string name, char symbol) : Player<char>(name, symbol) {}
    void getmove(int& x, int& y) override {
        cout << name << "'s turn (symbol: " << symbol << ")\n";
        cout << "Enter row and column: ";
        cin >> x >> y;
    }
};
int main() {
    TicTacToeBoard board;
    player player1("Player 1", 'X');
    player player2("Player 2", 'O');
    Player<char>* players[] = { &player1, &player2 };
    player1.setBoard(&board);
    player2.setBoard(&board);
    GameManager<char> gameManager(&board, players);
    gameManager.run();
    return 0;
}