#include "BoardGame_Classes.h"
#include <iostream>
using namespace std;
class TicTacToeBoard : public Board<char> {
public:
    TicTacToeBoard() {
        rows =3;
        columns = 3;
        board = new char*[rows];
        for (int i = 0; i < rows; i++) {
            board[i] = new char[columns];
            for (int j = 0; j < columns; j++) {
                board[i][j] = ' ';
            }
        }
    }
    bool update_board(int x, int y, char symbol) override {
        if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == ' ') {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }
    void display_board() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cout << (board[i][j] == ' ' ? '*' : board[i][j]);
                if (j < columns - 1) cout << " | ";
            }
            cout << endl;
            if (i < rows - 1) cout << "---------\n";
        }
    }
    bool is_win() override {
        for (int i = 0; i < rows; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
                return true;
        }
        for (int j = 0; j < columns; j++) {
            if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != ' ')
                return true;
        }
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
            return true;
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
            return true;
        return false;
    }
    bool is_draw() override {
        return n_moves == rows * columns && !is_win();
    }
    bool game_is_over() override {
        return is_win() || is_draw();
    }
    ~TicTacToeBoard() {
        for (int i = 0; i < rows; i++) {
            delete[] board[i];
        }
        delete[] board;
    }
};
class HumanPlayer : public Player<char> {
public:
    HumanPlayer(string name, char symbol) : Player<char>(name, symbol) {}
    void getmove(int &x, int &y) override {
        cout << getname() << " (" << getsymbol() << "), enter your move (row and column): ";
        cin >> x >> y;
        x--; y--;
    }
};
int main() {
    TicTacToeBoard board;
    Player<char>* players[2] = {
        new HumanPlayer("Player 1", 'X'),
        new HumanPlayer("Player 2", 'O')
    };
    players[0]->setBoard(&board);
    players[1]->setBoard(&board);
    GameManager<char> gameManager(&board, players);
    int x, y;
    board.display_board();
    while (!board.game_is_over()) {
        for (int i : {0, 1}) {
            players[i]->getmove(x, y);
            while (!board.update_board(x, y, players[i]->getsymbol())) {
                players[i]->getmove(x, y);
            }
            board.display_board();
            if (board.is_win()) {
                cout << players[1 - i]->getname() << " wins\n";
                return 0;
            }
            if (board.is_draw()) {
                cout << "Draw!\n";
                return 0;
            }
        }
    }
    delete players[0];
    delete players[1];
    return 0;
}
