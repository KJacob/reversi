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

using std::cerr;
using std::endl;
using std::to_string;
using std::cout;

/* Forward declarations. */
void play_game (in_addr_t rem_ip, in_port_t rem_port);

int main(int argc, const char * argv[]) {
    in_addr_t rem_ip;
    in_port_t rem_port;
    
    std::cout << "Welcome to the reversi game.\n";
    
    if (3 != argc)
    {
        cerr << "Incorrect number of arguments." << endl;
        return EXIT_FAILURE;
    }
    
    inet_pton(AF_INET, argv[1], &rem_ip);
    rem_port = htons(atoi(argv[2]));
    try
    {
        play_game(rem_ip, rem_port);
    }
    catch (string &str_error)
    {
        cerr << "FATAL " << str_error << endl;
    }
    
    return EXIT_SUCCESS;
}

void play_game (in_addr_t rem_ip, in_port_t rem_port)
{
    RemotePlayer *rplayer = new RemotePlayer(rem_ip, rem_port);
    GameEngine *engine = new GameEngine();
    Strategy *plan = new Strategy (8, CELL_WHITE);
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
            
            engine->reset_board("WHITE" == color ? CELL_WHITE : CELL_BLACK);
            engine->print_board();

            if (color == "BLACK")
                continue;
        }
        else
        {
            sscanf(rem_msg.c_str(), "%d%d", &i, &j);
            if (!engine->make_enemy_move(i, j))
            {
                cerr << "Can't make this move.\n" << i << ' ' << j << endl;
            }
        }

        cout << "Current Status" << endl;
        engine->print_board();
        
        /*if (!engine->play_move(i, j))
        {
            cerr << "No moves to make." << endl;
            continue;
        }*/
        
        plan->guess (*engine, 0, CELL_WHITE, INT_MIN, INT_MAX); 
        plan->get_optimal(i, j);
        engine->make_move (i, j, CELL_WHITE);
        
        std::cout << "Our move: " << i << ' ' << j << endl;
        cout << "After move:" << endl;
        engine->print_board();

        rplayer->send_string(to_string(i) + " " + to_string(j));
    }
}

