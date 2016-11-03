//

#include "RemotePlayer.hpp"
#include "GameEngine.hpp"
#include "Strategy.hpp"

//  main.cpp
//  reversi
//
//  Created by Kurian Jacob, Sandeep S et al. on 23/09/16.
//  Copyright © 2016 Kurian Jacob. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <string>
#include <climits>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_SEARCH_DEPTH 7

using std::cerr;
using std::endl;
using std::to_string;
using std::cout;

/* Forward declarations. */
void play_game (in_addr_t rem_ip, in_port_t rem_port, int search_depth);

int main(int argc, const char * argv[]) {
    in_addr_t rem_ip;
    in_port_t rem_port;
    int search_depth = DEFAULT_SEARCH_DEPTH;

    std::cout << "Welcome to the reversi game.\n";

    if (3 > argc)
    {
        cerr << "Incorrect number of arguments." << endl;
        return EXIT_FAILURE;
    }
    
    if (4 == argc)
    {
    	search_depth = atoi(argv[3]);
    }

    cout << "Selected difficulty level is " << search_depth << endl;

    inet_pton(AF_INET, argv[1], &rem_ip);
    rem_port = htons(atoi(argv[2]));
    try
    {
        play_game(rem_ip, rem_port, search_depth);
    }
    catch (string &str_error)
    {
        cerr << "FATAL " << str_error << endl;
    }

    return EXIT_SUCCESS;
}

void play_game (in_addr_t rem_ip, in_port_t rem_port, int search_depth)
{
    RemotePlayer *rplayer = new RemotePlayer(rem_ip, rem_port);
    GameEngine *engine = new GameEngine();
    Strategy *plan = new Strategy (search_depth, CELL_WHITE);
    string rem_msg;

    if (!rplayer->connect())
    {
        ::perror("remote");
        return;
    }

    rplayer->send_rnum("163050036");

    while (true)
    {
        rem_msg = rplayer->receive_string();
        int msg_break = rem_msg.find_first_of (' ');
        int i, j;
        std::cout<<rem_msg<<std::endl;
        string command = rem_msg.substr(0, msg_break);
        if ("START" == command)
        {
            string color = rem_msg.substr(msg_break + 1);
            int self_color = "WHITE" == color ? CELL_WHITE : CELL_BLACK;
            engine->reset_board(self_color);
            engine->print_board();
            plan->set_color(self_color);

            if (color == "BLACK")
                continue;
        }
        else
        {
            sscanf(rem_msg.c_str(), "%d%d", &i, &j);
            if (!engine->make_enemy_move(i, j))
            {
                cerr << "Can't make opponent move: " << i << " " << j << endl;
            }
        }

        cout << "Current Status" << endl;
        engine->print_board();

        /*if (!engine->play_move(i, j))
        {
            cerr << "No moves to make." << endl;
            continue;
        }*/

        plan->guess_self_move(*engine);
        plan->get_optimal(i, j);
        if(!engine->make_self_move(i, j))
        {
            cerr << "Can't make self move: " << i << " " << j << endl;
        }

        std::cout << "Our move: " << i << ' ' << j << endl;
        cout << "After move:" << endl;
        engine->print_board();
        engine->print_points();

        rplayer->send_string(to_string(i) + " " + to_string(j));
    }
}

