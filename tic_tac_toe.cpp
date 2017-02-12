//
// Created by robin on 2/8/17.
//

#include <iostream>
#include <tgmath.h>
#include "tic_tac_toe.h"

TicTacToe::TicTacToe(int board_size) {
    this->board_size = board_size;
    board = new Marble[board_size];

    zobrist_table = new int[board_size * 3];
    init_zobrist_table();
    count = 0;
}

bool TicTacToe::do_move(TicTacToe::Marble marble, int position) {
    if (position > board_size - 1)
        return false;

    if (marble == EMPTY && board[position] != EMPTY)
        count--;
    else
        count++;

    board[position] = marble;

    return true;
}

bool TicTacToe::is_empty(int position) {
    return board[position] == EMPTY;
}

bool TicTacToe::is_winner(TicTacToe::Marble m) {
    return (marble_on_position(m, 0) && marble_on_position(m, 1) && marble_on_position(m, 2)) ||
           (marble_on_position(m, 3) && marble_on_position(m, 4) && marble_on_position(m, 5)) ||
           (marble_on_position(m, 6) && marble_on_position(m, 7) && marble_on_position(m, 8)) ||
           (marble_on_position(m, 0) && marble_on_position(m, 3) && marble_on_position(m, 6)) ||
           (marble_on_position(m, 1) && marble_on_position(m, 4) && marble_on_position(m, 7)) ||
           (marble_on_position(m, 2) && marble_on_position(m, 5) && marble_on_position(m, 8)) ||
           (marble_on_position(m, 0) && marble_on_position(m, 4) && marble_on_position(m, 8)) ||
           (marble_on_position(m, 2) && marble_on_position(m, 4) && marble_on_position(m, 6));

}

int TicTacToe::get_empty_count() {
    return board_size - count;
}

bool TicTacToe::marble_on_position(TicTacToe::Marble marble, int position) {
    return board[position] == marble;
}


int *TicTacToe::get_possible_moves(int &ref_size) {
    int *possibilities = new int[board_size - count];
    int current_cell = 0;
    for (int i = 0; i < board_size; i++) {
        if (is_empty(i)) {
            possibilities[current_cell] = i;
            current_cell++;
        }
    }
    ref_size = board_size - count;
    return possibilities;
}

const int lines[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {2, 4, 6},
        {0, 4, 8}
};

const int heuristic[4][4] = {
        {0,    -10, -100, -1000},
        {10,   0,   0,    0},
        {100,  0,   0,    0},
        {1000, 0,   0,    0}
};

/**
 * Static evaluation function, uses a transposition table to return moves that we already know.
 * Else it calculates the score, puts it in the transposition table and returns it.
 * @return
 */
int TicTacToe::eval() {

    if (is_winner(ZERO))
        return INF;
    else if (is_winner(CROSS))
        return NINF;

    int hash = get_hash();

    if (transposition_table[hash])
        return transposition_table[hash];

    int total = 0;
    for (int i = 0; i < 8; i++) {
        short me, other;
        me = other = 0;
        for (int j = 0; j < 3; j++) {
            Marble s = board[lines[i][j]];
            if (s == ZERO)
                me++;
            else if (s == CROSS)
                other++;
        }
        total += heuristic[me][other];
    }

    transposition_table[hash] = total;

    return total;
}

int TicTacToe::do_ai_move(TicTacToe::Marble m) {
    int possibility_size, *possibilities = get_possible_moves(possibility_size);
    int best_move = -1, score = NINF;
    for (int i = 0; i < possibility_size; i++) {
        do_move(m, possibilities[i]);
        // Do killer move
//        if (is_winner(m))
//            return do_move(m, possibilities[i]);

        // Search 10 plies deep (5 complete rounds)
        int temp_score = -mini_max((Marble) (3 - m), m, 5, NINF, INF, true);
        if (temp_score > score) {
            score = temp_score;
            best_move = possibilities[i];
        }
        do_move(TicTacToe::EMPTY, possibilities[i]);
    }
    return do_move(m, best_move);
}

/**
 * Minimax with alpha beta pruning and Zobrist hashing
 * @param me
 * @param other
 * @param depth
 * @param alpha
 * @param beta
 * @param maximize
 * @return
 */
int TicTacToe::mini_max(TicTacToe::Marble me, TicTacToe::Marble other, int depth, int alpha, int beta, bool maximize) {
    if (depth == 0 || is_winner(me) || is_winner(other) ||
        (!is_winner(me) && !is_winner(other) && count == board_size)) {
        return eval();
    }
    int possibility_size, *possibilities = get_possible_moves(possibility_size);
    int score = (maximize) ? NINF : INF;
    for (int i = 0; i < possibility_size; i++) {
        do_move(me, possibilities[i]);
        int temp_score;
        if (maximize) {
            temp_score = mini_max(other, me, depth - 1, alpha, beta, false);
            score = std::max(score, temp_score);
            alpha = std::max(alpha, score);
        } else {
            temp_score = mini_max(other, me, depth - 1, alpha, beta, true);
            score = std::min(score, temp_score);
            beta = std::min(beta, score);
        }
        do_move(TicTacToe::EMPTY, possibilities[i]);
        if (alpha >= beta)
            break;
    }
    return score;
}


void TicTacToe::init_zobrist_table() {
    for (int i = 0; i < board_size * 3; i++) {
        zobrist_table[i] = i + 1;
    }
}

int TicTacToe::get_zobrist_value(Marble piece, int cell) {
    return zobrist_table[cell * 3 + piece];
}

int TicTacToe::get_hash() {
    int result = 0;
    for (int i = 0; i < board_size; i++) {
        result ^= get_zobrist_value(board[i], i);
    }
    return result;
}

void TicTacToe::draw() {
    int col = (int) sqrt(board_size);
    int current = 0;
    for (int h = 0; h < col; h++) {
        for (int w = 0; w < col; w++) {
            switch (board[current]) {
                case TicTacToe::Marble::CROSS:
                    std::cout << 'X';
                    break;
                case TicTacToe::Marble::ZERO:
                    std::cout << 'O';
                    break;
                default:
                    std::cout << '_';
            }
            current++;
        }
        std::cout << std::endl;
    }
}



