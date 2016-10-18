#ifndef _STRATEGY_H

#define _STRATEGY_H

#include "GameEngine.hpp"

#define OPPONENT_COLOR(x) (x == CELL_WHITE ? CELL_BLACK : CELL_WHITE)
class Strategy
{
	private:
	
	/* No local vartiables. */
	int max_depth;
	
	/* Results of strategy. */
	int move_i, move_j;
	
	int color;
	
	public:
	Strategy (int max_depth, int color);
	
	
	int guess (GameEngine &engine, int depth, int color, int alpha, int beta);
	int minimum_points (GameEngine& engine, int depth, int color, int &optimal_i, int& optimal_j, int alpha, int beta);
	int maximum_points (GameEngine& engine, int depth, int color, int &optimal_i, int &optimal_j, int alpha, int beta);
	void get_optimal (int &i, int &j); 
		
	~Strategy ();
};

#endif
