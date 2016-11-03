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


	double guess (GameEngine &engine, int depth, int color, double alpha, double beta);
    double alpha_beta_guess (GameEngine &engine, int depth, double alpha, double beta, int color);
    void guess_self_move(GameEngine &engine);
	double minimum_points (GameEngine& engine, int depth, int color, int &optimal_i, int& optimal_j, double alpha, double beta);
	double maximum_points (GameEngine& engine, int depth, int color, int &optimal_i, int &optimal_j, double alpha, double beta);
	void get_optimal (int &i, int &j);
    void set_optimal_move (int i, int j);
    void set_color(int color);

	~Strategy ();
};

#endif
