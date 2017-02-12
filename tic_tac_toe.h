//
// Created by robin on 2/8/17.
//

#ifndef LECTURE_3_TIC_TAC_TOE_H
#define LECTURE_3_TIC_TAC_TOE_H

#include <map>

#define INF 0x7FFFFFFF
#define NINF ~INF+1

class TicTacToe {
public:
    typedef enum {
        EMPTY, CROSS, ZERO
    } Marble;

    TicTacToe(int);

    Marble *get_board();

    bool is_winner(Marble);

    int get_empty_count();

    bool do_move(Marble, int);

    int do_ai_move(Marble);

    int mini_max(Marble, Marble, int, int, int, bool);

    int eval();

    void draw();

private:
    int count;
    int board_size;
    int *zobrist_table;
    std::map<int, int> transposition_table;
    Marble *board;


    void init_zobrist_table();

    int get_zobrist_value(Marble, int);

    int get_hash();

    bool is_empty(int);

    bool marble_on_position(Marble, int);

    int *get_possible_moves(int &);

};


#endif //LECTURE_3_TIC_TAC_TOE_H
