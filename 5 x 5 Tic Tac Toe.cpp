#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
using namespace std;
class TicTacToeBoard : public Board<char> {
public:
    TicTacToeBoard() : Board() {
        rows =5;
        columns = 5;
        board = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new char[columns]{' '};
        }
    }
    ~TicTacToeBoard() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }
    bool update_board(int x, int y, char symbol) override {
        if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != ' ') {
            cout << "Invalid move! Try again.\n";
            return false;
        }
        board[x][y] = symbol;
        n_moves++;
        return true;
    }
    void display_board() override {
        cout << "  ";
        for (int j = 0; j < columns; ++j) cout << j << " ";
        cout << endl;
        for (int i = 0; i < rows; ++i) {
            cout << i << " ";
            for (int j = 0; j < columns; ++j) {
                cout << (board[i][j] == ' ' ? '*' : board[i][j]) << " ";
            }
            cout << endl;
        }
    }
    bool is_win() override { return false; }
    bool is_draw() override { return n_moves == rows * columns; }
    bool game_is_over() override { return is_win() || is_draw(); }
    int count_threes(char symbol) {
        int count = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol) count++;
                if (board[j][i] == symbol && board[j + 1][i] == symbol && board[j + 2][i] == symbol) count++;
            }
        }
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) count++;
                if (board[i][j + 2] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j] == symbol) count++;
            }
        }
        return count;
    }
    void reset_board() {
        for (int i = 0; i < rows; ++i) {
            fill(board[i], board[i] + columns, ' ');
        }
        n_moves = 0;
    }
};
class HumanPlayer : public Player<char> {
public:
    HumanPlayer(string n, char symbol) : Player<char>(n, symbol) {}
    void getmove(int& x, int& y) override {
        cout <<
            name << " (" << symbol << "), enter your move (row and column): ";
        cin >> x >> y;
    }
};
int main() {
    TicTacToeBoard board;
    HumanPlayer player1("Player 1", 'X');
    HumanPlayer player2("Player 2", 'O');
    Player<char>* players[2] = { &player1, &player2 };
    player1.setBoard(&board);
    player2.setBoard(&board);
    int rounds = 0, p1_wins = 0, p2_wins = 0;
    char play_again = 'y';
    while (play_again == 'y') {
        cout << "Starting round " << ++rounds << "!\n";
        board.reset_board();
        GameManager<char> gameManager(&board, players);
        gameManager.run();
        int p1_score = board.count_threes('X');
        int p2_score = board.count_threes('O');
        cout << "Round " << rounds << " Scores:\nPlayer 1 (X): " << p1_score << "\nPlayer 2 (O): " << p2_score << "\n";
        if (p1_score > p2_score) p1_wins++;
        else if (p2_score > p1_score) p2_wins++;
        else cout << "This round is a draw!\n";
        cout << "Play another round? (y/n): ";
        cin >> play_again;
    }
    cout << "\nGame Summary:\nTotal Rounds Played: " << rounds
         << "\nPlayer 1 Total Wins: " << p1_wins
         << "\nPlayer 2 Total Wins: " << p2_wins
         << "\nOverall Winner: " << (p1_wins > p2_wins ? "Player 1!" : p2_wins > p1_wins ? "Player 2!" : "Draw!") << endl;
    return 0;
}