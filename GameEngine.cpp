//
//  GameEngine.cpp
//  reversi
//
//  Created by Kurian Jacob on 23/09/16.
//  Copyright © 2016 Kurian Jacob. All rights reserved.
//

#include "GameEngine.hpp"

#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

const int GameEngine::pos_weight_matrix[8][8] =
{
    {666,  -8,  8,  6,  6,   8,  -8,  666},
    { -8, -24, -4, -3, -3,  -4, -24,   -8},
    {  8,  -4,  7,  4,  4,   7,  -4,    8},
    {  6,  -3,  4,  0,  0,   4,  -3,    6},
    {  6,  -3,  4,  0,  0,   4,  -3,    6},
    {  8,  -4,  7,  4,  4,   7,  -4,    8},
    { -8, -24, -4, -3, -3,  -4,  -24,  -8},
    {666,  -8,  8,  6,  6,   8,  -8,  666}
};

GameEngine::GameEngine()
{
}

void GameEngine::reset_board(int color)
{
    gameboard = vector<vector<int> >(8, vector<int>(8, CELL_BLANK));

    this->gameboard[3][3] = CELL_WHITE;
    this->gameboard[4][4] = CELL_WHITE;
    this->gameboard[3][4] = CELL_BLACK;
    this->gameboard[4][3] = CELL_BLACK;

    this->color = color;
}

/* Copy constructor. */
GameEngine::GameEngine (const GameEngine &other)
{
	this->gameboard = other.gameboard;
	this->color = other.color;
}

bool GameEngine::is_move_valid(int i, int j, int color)
{
    /* Move right, left, bottom, top */

    int c, r;
    int n_enemy_color;

    if ( i < 0 || i > 7 || j < 0 || j > 7)
    {
        return false;
    }

    if (gameboard[i][j] != CELL_BLANK) {
        return false;
    }

    c = j + 1;

    n_enemy_color = 0;
    while (c < 8 && gameboard[i][c] != color &&gameboard[i][c] != CELL_BLANK) {
        n_enemy_color ++;
        c++;
    }

    if (c < 8 && gameboard[i][c] == color && n_enemy_color) {
        return true;
    }

    c = j - 1;
    n_enemy_color = 0;
    while (c >= 0 && gameboard[i][c] != color && gameboard[i][c] != CELL_BLANK) {
        n_enemy_color++;
        c--;
    }

    if (c >= 0 && gameboard[i][c] == color && n_enemy_color) {
        return true;
    }

    r = i + 1;
    n_enemy_color = 0;
    while (r < 8 && gameboard[r][j] != color && gameboard[r][j] != CELL_BLANK)
    {
        n_enemy_color++;
        r++;
    }

    if (r < 8 && gameboard[r][j] == color && n_enemy_color) {
        return true;
    }

    r = i - 1;
    n_enemy_color = 0;
    while (r >= 0 && gameboard[r][j] != color && gameboard[r][j] != CELL_BLANK) {
        n_enemy_color++;
        r--;
    }

    if (r >= 0 && gameboard[r][j] == color && n_enemy_color) {
        return true;
    }

    r = i - 1;
    c = j - 1;
    n_enemy_color = 0;

    while(r >= 0 && c >= 0 && gameboard[r][c] != color && gameboard[r][c] != CELL_BLANK)
    {
    	n_enemy_color++;
    	r--;
    	c--;
    }

    if (r >= 0 && c >= 0 && gameboard[r][c] == color && n_enemy_color){
	    return true;
	}

    r = i - 1;
    c = j + 1;
    n_enemy_color = 0;

    while(r >= 0 && c < 8 && gameboard[r][c] != color && gameboard[r][c] != CELL_BLANK)
    {
    	n_enemy_color++;
    	r--;
    	c++;
    }

    if (r >= 0 && c < 8 && gameboard[r][c] == color && n_enemy_color){
	    return true;
    }

    r = i + 1;
    c = j - 1;
    n_enemy_color = 0;

    while(r < 8 && c >= 0 && gameboard[r][c] != color && gameboard[r][c] != CELL_BLANK)
    {
    	n_enemy_color++;
    	r++;
    	c--;
    }

    if (r < 8 && c >= 0 && gameboard[r][c] == color && n_enemy_color){
	    return true;
    }

    r = i + 1;
    c = j + 1;
    n_enemy_color = 0;

    while(r < 8 && c < 8 && gameboard[r][c] != color && gameboard[r][c] != CELL_BLANK)
    {
    	n_enemy_color++;
    	r++;
    	c++;
    }

    if (r < 8 && c < 8 && gameboard[r][c] == color && n_enemy_color){
	    return true;
    }

    return false;
}

int GameEngine::num_valid_moves(int color)
{
    int num_moves = 0;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (is_move_valid(r, c, color))
            {
                num_moves++;
            }
        }
    }
    return num_moves;
}


bool GameEngine::play_move(int &i, int &j)
{
    int r, c;

    for (r = 0; r < 8; r++)
    {
        for (c = 0; c < 8; c++)
        {
            if (is_move_valid(r, c, this->color))
            {
                i = r;
                j = c;

                make_move(i, j, color);
                return true;
            }
        }
    }

    /* No moves to play. */
    return false;
}

void GameEngine::make_move(int i, int j, int color)
{
    int dir_r, dir_c;
    int ci, cj;

    gameboard[i][j] = color;

    for (dir_r = -1; dir_r <= +1; dir_r++)
    {
        for (dir_c = -1; dir_c <= +1; dir_c++)
        {

            if (!dir_c && !dir_r)
                continue;

            if (search_color(color, i, j, ci, cj, dir_r, dir_c))
            {
                apply_color(i, j, color, dir_r, dir_c);
            }
        }
    }
}

bool GameEngine::search_color(int color, int i, int j, int &ci, int &cj, int dir_r, int dir_c)
{
    int r, c;
    int n_enemy_color;

    r = i + dir_r;
    c = j + dir_c;
    n_enemy_color = 0;

    while (ROW_VALID(r) && COL_VALID(c) && gameboard[r][c] != color && gameboard[r][c] != CELL_BLANK)
    {
        r = r + dir_r;
        c = c + dir_c;
        n_enemy_color ++;
    }

    if (ROW_VALID(r) && COL_VALID(c) && gameboard[r][c] == color && n_enemy_color)
    {
        ci = r;
        cj = c;
        return true;
    }

    return false;
}

void GameEngine::apply_color(int i, int j, int color, int dir_r, int dir_c)
{
    int r, c;

    r = i + dir_r;
    c = j + dir_c;

    while (gameboard[r][c] != color)
    {
        gameboard[r][c] = color;
        r = r + dir_r;
        c = c + dir_c;
    }
}

bool GameEngine::make_self_move(int &i, int &j)
{
    if(i >= 0 && i <= 7 && j >= 0 && j <= 7)
    {
        if(is_move_valid(i, j, color))
        {
            make_move(i, j, color);
            return true;
        }
    } else
    {
        cerr<< "Attempting to play random move.";
        return play_move(i, j);
    }
    return false;
}

bool GameEngine::make_enemy_move(int i, int j)
{
    int enemy_color = color == CELL_WHITE ? CELL_BLACK : CELL_WHITE;
    if(is_move_valid(i, j, enemy_color))
    {
        make_move(i, j, enemy_color);
        return true;
    }

    return false;
}

void GameEngine::print_board ()
{
	char ch;

	cout << "\t";
	for (int j = 0; j < 8; ++j)
		cout << j;

	cout << endl;
	for (int i = 0; i < 8; ++i)
	{
		cout << i << '\t';
		for(int j = 0; j < 8; ++j)
		{
			ch = gameboard[i][j] == CELL_BLANK ? '.' : gameboard[i][j] == CELL_WHITE ? 'W' : 'B';
			cout << ch;
		}

		cout << std::endl;
	}
}

void GameEngine::print_points ()
{
	int black, white;
	black = white = 0;

	int i, j;

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (CELL_WHITE == gameboard[i][j])
			{
				white++;
			}
			else if (CELL_BLACK == gameboard[i][j])
			{
				black++;
			}
			else ;
		}
	}

	cout << "White: " << white << " Black: " << black << endl;
	return;
}

double GameEngine::evaluate (int color)
{
    int i = 0, j = 0;
    int opp_color = color == CELL_WHITE ? CELL_BLACK : CELL_WHITE;
    int self_cell_score = 0, opp_cell_score = 0; // Weighted score of cell positons
    int self_cell_count = 0, opp_cell_count = 0; // Num of cells occupied by each player
    int self_frontier_count = 0, opp_frontier_count = 0; // Num of frontier cells
    int self_corner_count = 0, opp_corner_count = 0; // Num of corner cells captured
    int self_corner_close_count = 0, opp_corner_close_count = 0; // Num of close to corner cells
    int self_mobility_count = 0, opp_mobility_count = 0; // Num of possible moves for each player

    double cell_count_score = 0, frontier_score = 0, cell_score = 0, corner_score = 0, corner_close_score = 0, mobility_score = 0, net_score = 0;

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if(gameboard[i][j] == color)
            {
                self_cell_score += pos_weight_matrix[i][j];
                self_cell_count++;
            } else if(gameboard[i][j] == opp_color)
            {
                opp_cell_score += pos_weight_matrix[i][j];
                opp_cell_count++;
            }
            if(gameboard[i][j] != CELL_BLANK)
            {
                for(int k = -1; k <= 1; k++)
                {
                    for(int l = -1; l <= 1; l++)
                    {
                        if( i + k >= 0 && i + k <=7 && j + l >= 0 && j + l <= 7 && gameboard[i + k][j + l] == CELL_BLANK)
                        {
                            if(gameboard[i][j] == color)
                            {
                                self_frontier_count++;
                            } else
                            {
                                opp_frontier_count++;
                            }
                        }
                    }
                }
            }
        }
    }

    cell_score = self_cell_score - opp_cell_score;

    if(self_cell_count > opp_cell_count)
    {
        cell_count_score = (100.0 * self_cell_count) / (self_cell_count + opp_cell_count);
    } else if(opp_cell_count > self_cell_count)
    {
        cell_count_score = -1 * (100.0 * opp_cell_count) / (self_cell_count + opp_cell_count);
    }

    if(self_frontier_count > opp_frontier_count)
    {
        frontier_score = -1 * (100.0 * self_frontier_count) / (self_frontier_count + opp_frontier_count);
    } else if(opp_frontier_count > self_frontier_count)
    {
        frontier_score = (100.0 * opp_frontier_count) / (self_frontier_count + opp_frontier_count);
    }


    if(gameboard[0][0] == color) self_corner_count++;
    else if(gameboard[0][0] == opp_color) opp_corner_count++;
    if(gameboard[0][7] == color) self_corner_count++;
    else if(gameboard[0][7] == opp_color) opp_corner_count++;
    if(gameboard[7][0] == color) self_corner_count++;
    else if(gameboard[7][0] == opp_color) opp_corner_count++;
    if(gameboard[7][7] == color) self_corner_count++;
    else if(gameboard[7][7] == opp_color) opp_corner_count++;

    corner_score = 25 * (self_corner_count - opp_corner_count);


	if(gameboard[0][0] == CELL_BLANK)   {
		if(gameboard[0][1] == color) self_corner_close_count++;
		else if(gameboard[0][1] == opp_color) opp_corner_close_count++;
		if(gameboard[1][1] == color) self_corner_close_count++;
		else if(gameboard[1][1] == opp_color) opp_corner_close_count++;
		if(gameboard[1][0] == color) self_corner_close_count++;
		else if(gameboard[1][0] == opp_color) opp_corner_close_count++;
	}
	if(gameboard[0][7] == CELL_BLANK)   {
		if(gameboard[0][6] == color) self_corner_close_count++;
		else if(gameboard[0][6] == opp_color) opp_corner_close_count++;
		if(gameboard[1][6] == color) self_corner_close_count++;
		else if(gameboard[1][6] == opp_color) opp_corner_close_count++;
		if(gameboard[1][7] == color) self_corner_close_count++;
		else if(gameboard[1][7] == opp_color) opp_corner_close_count++;
	}
	if(gameboard[7][0] == CELL_BLANK)   {
		if(gameboard[7][1] == color) self_corner_close_count++;
		else if(gameboard[7][1] == opp_color) opp_corner_close_count++;
		if(gameboard[6][1] == color) self_corner_close_count++;
		else if(gameboard[6][1] == opp_color) opp_corner_close_count++;
		if(gameboard[6][0] == color) self_corner_close_count++;
		else if(gameboard[6][0] == opp_color) opp_corner_close_count++;
	}
	if(gameboard[7][7] == CELL_BLANK)   {
		if(gameboard[6][7] == color) self_corner_close_count++;
		else if(gameboard[6][7] == opp_color) opp_corner_close_count++;
		if(gameboard[6][6] == color) self_corner_close_count++;
		else if(gameboard[6][6] == opp_color) opp_corner_close_count++;
		if(gameboard[7][6] == color) self_corner_close_count++;
		else if(gameboard[7][6] == opp_color) opp_corner_close_count++;
	}
	corner_score = -12.5 * (self_corner_close_count - opp_corner_close_count);

    self_mobility_count = num_valid_moves(color);
    opp_mobility_count = num_valid_moves(opp_color);

    if(self_mobility_count > opp_mobility_count)
    {
        mobility_score = (100 * self_mobility_count) / (self_mobility_count + opp_mobility_count);
    } else if(opp_mobility_count > self_mobility_count)
    {
        mobility_score = -1 * (100 * opp_mobility_count) / (self_mobility_count + opp_mobility_count);
    }


    net_score = 10 * cell_count_score + 800 * corner_score + 380 * corner_close_score + 70 * mobility_score + 70 * frontier_score + 10 * cell_score;

    return net_score;
}

GameEngine::~GameEngine()
{
    /* Nothing */
}
