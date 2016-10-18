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

const int GameEngine::pos_weight_matrix[8][8] =
  {
  {99, -8, 8, 6, 6, 8, -8, 99},
  {-8, -24, -4, -3, -3, -4, -24, -8},
  {8, -4, 7, 4, 4, 7, -4, 8},
  {6, -3, 4, 0, 0, 4, -3, 6},
    {6, -3, 4, 0, 0, 4, -3, 6},
    {8, -4, 7, 4, 4, 7, -4, 8},
    {-8, -24, -4, -3, -3, -4, -24, -8},
  {99, -8, 8, 6, 6, 8, -8, 99}
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

int GameEngine::evaluate (int color)
{
	int pos_score;
	
	int i, j;
	
	pos_score = 0;
	
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (gameboard[i][j] == color)
			{
				pos_score = pos_score + pos_weight_matrix[i][j];
			}
		}
	}
	
	return pos_score;
}

GameEngine::~GameEngine()
{
    /* Nothing */
}
