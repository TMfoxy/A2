#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class NumericalBoard : public Board<int> {
private:
    vector<vector<int>> board;
    vector<int> usedNumbers;

public:
    NumericalBoard() {
        rows = 3;
        columns = 3;
        board.resize(rows, vector<int>(columns, '*'));
    }

    bool update_board(int x, int y, int symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == '*' &&
            find(usedNumbers.begin(), usedNumbers.end(), symbol) == usedNumbers.end()) {
            board[x][y] = symbol;
            usedNumbers.push_back(symbol);
            return true;
        }
        return false;
    }

    void display_board() override {
        cout << "  0 1 2\n";
        for (int i = 0; i < rows; i++) {
            cout << i << " ";
            for (int j = 0; j < columns; j++) {
                if (board[i][j] == '*') {
                    cout << "* ";
                } else {
                    cout << board[i][j] << " ";
                }
            }
            cout << "\n";
        }
    }

    bool is_win() override {
        for (int i = 0; i < rows; i++) {
            if (board[i][0] + board[i][1] + board[i][2] == 15) return true;
        }
        for (int j = 0; j < columns; j++) {
            if (board[0][j] + board[1][j] + board[2][j] == 15) return true;
        }
        if (board[0][0] + board[1][1] + board[2][2] == 15 || board[0][2] + board[1][1] + board[2][0] == 15) return true;
        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (board[i][j] == '*') return false;
            }
        }
        return !is_win();
    }

    bool game_is_over() override {
        return is_win() || is_draw();
    }

    vector<int> get_used_numbers() {
        return usedNumbers;
    }
};

class NumericalPlayer : public Player<int> {
public:
    NumericalPlayer(string n, int symbol) : Player<int>(n, symbol) {}

    void getmove(int& x, int& y) override {
        cout << name << ", enter your move (row column): ";
        cin >> x >> y;
    }

    int choose_number(const vector<int>& usedNumbers) {
        int number;
        while (true) {
            cout << name << ", choose your number: ";
            cin >> number;
            if (find(usedNumbers.begin(), usedNumbers.end(), number) == usedNumbers.end() &&
                ((symbol == 1 && number % 2 != 0) || (symbol == 2 && number % 2 == 0))) {
                return number;
            }
            cout << "Invalid number. Try again.\n";
        }
    }
};

int main() {
    NumericalBoard board;
    NumericalPlayer player1("Player 1 (Odd)", 1);
    NumericalPlayer player2("Player 2 (Even)", 2);
    Player<int>* players[2] = {&player1, &player2};

    int x, y, chosenNumber;
    board.display_board();
    while (!board.game_is_over()) {
        for (int i = 0; i < 2; i++) {
            auto usedNumbers = board.get_used_numbers();
            chosenNumber = static_cast<NumericalPlayer*>(players[i])->choose_number(usedNumbers);
            players[i]->getmove(x, y);
            while (!board.update_board(x, y, chosenNumber)) {
                cout << "Invalid move. Try again.\n";
                players[i]->getmove(x, y);
            }
            board.display_board();
            if (board.is_win()) {
                cout << players[i]->getname() << " wins\n";
                return 0;
            }
            if (board.is_draw()) {
                cout << "Draw!\n";
                return 0;
            }
        }
    }
    return 0;
}
