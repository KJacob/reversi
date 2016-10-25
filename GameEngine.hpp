//
//  GameEngine.hpp
//  reversi
//
//  Created by Kurian Jacob, Sandeep S et al. on 23/09/16.
//  Copyright © 2016 Kurian Jacob. All rights reserved.
//

#ifndef GameEngine_hpp
#define GameEngine_hpp

#include <stdio.h>
#include <vector>

#define ROW_VALID(x) ((x) < 8 && (x) >= 0)
#define COL_VALID(x) ((x) < 8 && (x) >= 0)

using std::vector;

enum cell_type{
    CELL_BLANK,
    CELL_WHITE,
    CELL_BLACK
};

class GameEngine {
private:
    vector<vector<int> > gameboard;
    int color;
    static const int pos_weight_matrix[8][8];

public:
    GameEngine ();
    GameEngine (const GameEngine &other);

    void reset_board(int color);

    bool play_move(int &i, int &j);
    bool make_enemy_move(int i, int j);

    bool is_move_valid (int i, int j, int color);

    bool search_color(int color, int i, int j, int &ci, int &cj, int dir_r, int dir_c);
    void apply_color(int i, int j, int color, int dir_r, int dir_c);
    void make_move(int i, int j, int color);
    int num_valid_moves(int color);
    bool make_self_move(int &i, int &j);
    void print_board ();

    double evaluate(int color);

    ~GameEngine();
};

#endif /* GameEngine_hpp */
