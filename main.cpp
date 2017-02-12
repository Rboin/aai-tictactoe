#include <iostream>
#include "tic_tac_toe.h"

using namespace std;

const int board_size = 9;

int main() {
    TicTacToe game(board_size);
    int x;
    for (int i = 0; i < 9; i++) {
        cout << "Insert index to place O on: ";
        cin >> x;
        game.do_move(TicTacToe::ZERO, x);
        // board is full?
        if (game.get_empty_count() != 0) {
            game.do_ai_move(TicTacToe::CROSS);
        }
        game.draw();
        if (game.get_empty_count() == 0) {
            cout << "DRAW" << endl;
            return 0;
        } else if (game.is_winner(TicTacToe::CROSS)) {
            cout << "CROSS WON!";
            break;
        } else if (game.is_winner(TicTacToe::ZERO)) {
            cout << "ZERO WON";
            break;
        }
        cout << endl << endl;
    }
    return 0;
}