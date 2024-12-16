#include <iostream>
#include "BoardGame_Classes.h"
using namespace std;

class FourInARowBoard : public Board<char> {
private:
    int rows = 6, cols = 7;
    char** grid;

public:
    FourInARowBoard() {
        grid = new char*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new char[cols];
            for (int j = 0; j < cols; j++) {
                grid[i][j] = '.';
            }
        }
    }

    bool update_board(int x, int y, char symbol) override {
        if (y < 0 || y >= cols || grid[0][y] != '.') return false;
        for (int i = rows - 1; i >= 0; i--) {
            if (grid[i][y] == '.') {
                grid[i][y] = symbol;
                return true;
            }
        }
        return false;
    }
    void display_board() override {
        cout << "  0 1 2 3 4 5 6\n";
        for (int i = 0; i < rows; i++) {
            cout << i << " ";
            for (int j = 0; j < cols; j++) {
                cout << grid[i][j] << " ";
            }
            cout << "\n";
        }
    }

    bool is_win() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char s = grid[i][j];
                if (s == '.') continue;
                if (j + 3 < cols && s == grid[i][j+1] && s == grid[i][j+2] && s == grid[i][j+3]) return true;
                if (i + 3 < rows && s == grid[i+1][j] && s == grid[i+2][j] && s == grid[i+3][j]) return true;
                if (i + 3 < rows && j + 3 < cols && s == grid[i+1][j+1] && s == grid[i+2][j+2] && s == grid[i+3][j+3]) return true;
                if (i + 3 < rows && j - 3 >= 0 && s == grid[i+1][j-1] && s == grid[i+2][j-2] && s == grid[i+3][j-3]) return true;
            }
        }
        return false;
    }

    bool is_draw() override {
        for (int j = 0; j < cols; j++) {
            if (grid[0][j] == '.') return false;
        }
        return true;
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }

    ~FourInARowBoard() {
        for (int i = 0; i < rows; i++) delete[] grid[i];
        delete[] grid;
    }
};

class HumanPlayer : public Player<char> {
public:
    HumanPlayer(string n, char s) : Player<char>(n, s) {}

    void getmove(int& x, int& y) override {
        cout << name << " (" << symbol << ") - Enter column (0-6): ";
        cin >> y;
        x = -1;
    }
};

int main() {
    FourInARowBoard board;
    HumanPlayer player1("Player 1", 'X');
    HumanPlayer player2("Player 2", 'O');

    player1.setBoard(&board);
    player2.setBoard(&board);

    Player<char>* players[2] = {&player1, &player2};
    GameManager<char> game(&board, players);

    game.run();

    return 0;
}
