#include <iostream>
#include <cfloat>
#include "Strategy.hpp"

Strategy::Strategy (int max_depth, int color)
{
	this->max_depth = max_depth;
	this->color = color;
}

double Strategy::guess (GameEngine &engine, int depth, int our_color, double alpha, double beta)
{
	int i, j;
	double optimal_point;
	int optimal_i, optimal_j;

	if (depth == max_depth)
		return engine.evaluate (our_color);

	/* this->color: Color of player of interest. */
	if (our_color != this->color)
	{
		optimal_point = minimum_points (engine, depth, our_color, optimal_i, optimal_j, alpha, beta);
	}
	else {
		optimal_point = maximum_points (engine, depth, our_color, optimal_i, optimal_j, alpha, beta);
	}

	if (depth == 0)
	{
		move_i = optimal_i;
		move_j = optimal_j;
	}

	return optimal_point;
}

void Strategy::get_optimal (int &i, int &j)
{
	i = move_i;
	j = move_j;
}

double Strategy::minimum_points (GameEngine& engine, int depth, int our_color, int &optimal_i, int &optimal_j, double alpha, double beta)
{
	int i, j;
	int opponent_color = OPPONENT_COLOR(our_color);
	double min_points = DBL_MAX;
	double points;
    bool has_valid_move = false;

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			GameEngine opponent = engine;

			if (opponent.is_move_valid(i, j, our_color))
			{
                has_valid_move = true;
				opponent.make_move(i, j, our_color);
				points = guess(opponent, depth + 1, opponent_color, alpha, min_points);

				if (points <= min_points)
				{
					min_points = points;
					optimal_i = i;
					optimal_j = j;
				}

				if (min_points <= alpha)
				{
				    return min_points;
				}
			}
		}
	}
    if(!has_valid_move)
    {
        return engine.evaluate(our_color);
    }

	return min_points;
}

double Strategy::maximum_points (GameEngine& engine, int depth, int our_color, int &optimal_i, int &optimal_j, double alpha, double beta)
{
	int i, j;
	int opponent_color = OPPONENT_COLOR(our_color);
	double max_points = -DBL_MAX;
	double points;
    bool has_valid_move = false;

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			GameEngine opponent = engine;

			if (opponent.is_move_valid(i, j, our_color))
			{
                has_valid_move = true;
				opponent.make_move(i, j, our_color);

				points = guess(opponent, depth + 1, opponent_color, max_points, beta);

				if (points >= max_points)
				{
					/* Optimize here */
					max_points = points;
					optimal_i = i;
					optimal_j = j;
				}
			}

			if (max_points >= beta)
			{
			    return max_points;
			}
		}
	}

    if(!has_valid_move)
    {
        return engine.evaluate(our_color);
    }

	return max_points;
}

void Strategy::set_color(int new_color)
{
    this->color = new_color;
}

void Strategy::guess_self_move(GameEngine &engine)
{
    this->guess(engine, 0, this->color, -DBL_MAX, DBL_MAX);
}
