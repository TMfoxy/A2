#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class PyramidBoard : public Board<char> {
private:
    vector<vector<char>> pyramid;

public:
    PyramidBoard() {
        pyramid = {{' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};
        rows = 3;
    }

    bool update_board(int x, int y, char symbol) override {
        if ((x == 0 && y == 2) || (x == 1 && y >= 1 && y <= 3) || (x == 2 && y >= 0 && y <= 4)) {
            if (x >= 0 && x < rows && y >= 0 && y < pyramid[x].size() && pyramid[x][y] == ' ') {
                pyramid[x][y] = symbol;
                n_moves++;
                return true;
            }
        }
        return false;
    }

    void display_board() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < pyramid[i].size(); ++j) {
                if ((i == 0 && j == 2) || (i == 1 && j >= 1 && j <= 3) || (i == 2)) {
                    cout << pyramid[i][j];
                } else {
                    cout << ' ';
                }
            }
            cout << "\n";
        }
    }

    bool is_win() override {
        // Check horizontal rows
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j <= 2; ++j) {
                if (pyramid[i][j] != ' ' && pyramid[i][j] == pyramid[i][j + 1] && pyramid[i][j] == pyramid[i][j + 2]) {
                    return true;
                }
            }
        }

        // Check vertical columns
        for (int j = 0; j < 5; ++j) {
            if (pyramid[0][j] != ' ' &&
                pyramid[0][j] == pyramid[1][j] &&
                pyramid[0][j] == pyramid[2][j]) {
                return true;
            }
        }

        // Check diagonals
        if ((pyramid[0][2] != ' ' && pyramid[0][2] == pyramid[1][1] && pyramid[0][2] == pyramid[2][0]) ||
            (pyramid[0][2] != ' ' && pyramid[0][2] == pyramid[1][3] && pyramid[0][2] == pyramid[2][4])) {
            return true;
        }

        return false;
    }

    bool is_draw() override {
        return n_moves == 9 && !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }
};

class PyramidPlayer : public Player<char> {
public:
    PyramidPlayer(string n, char symbol) : Player<char>(n, symbol) {}

    void getmove(int &x, int &y) override {
        cout << getname() << " (" << getsymbol() << "), enter your move (row and column): ";
        cin >> x >> y;
    }
};

int main() {
    PyramidBoard board;
    PyramidPlayer player1("Player 1", 'X');
    PyramidPlayer player2("Player 2", 'O');

    Player<char>* players[2] = {&player1, &player2};

    player1.setBoard(&board);
    player2.setBoard(&board);

    GameManager<char> gameManager(&board, players);
    gameManager.run();

    return 0;
}
