//
//  RemotePlayer.cpp
//  reversi
//
//  Created by Kurian Jacob on 23/09/16.
//  Copyright © 2016 Kurian Jacob. All rights reserved.
//

#include "RemotePlayer.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>

using std::string;

RemotePlayer::RemotePlayer(in_addr_t ipaddr, in_port_t port)
{
    this->ipaddr = ipaddr;
    this->port = port;
}

bool RemotePlayer::connect()
{
    sd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in rm_addr;
    
    if (sd < 0 ) {
        throw string("Can't establish connection to the remote player.");
    }
    
    bzero(&rm_addr, sizeof(rm_addr));
    rm_addr.sin_family = AF_INET;
    rm_addr.sin_port = this->port;
    rm_addr.sin_addr.s_addr = this->ipaddr;
    if(::connect(sd, reinterpret_cast<struct sockaddr *>(&rm_addr), sizeof(rm_addr)) < 0)
    {
        close(sd);
        throw string ("Can't connect to the remote server.");
    }
    
    return true;
}

bool RemotePlayer::send_string(const string &str)
{
    string s_str = str + "\n";
    return ::send(sd, s_str.c_str(), s_str.length(), 0) > 0;
}

bool RemotePlayer::send_rnum(const char *rnum)
{
    string str(rnum);
    
    return send_string(str);
}

string RemotePlayer::receive_string()
{
    char ch = '\0';
    string ret;
    
    while (ch != '\n') {
        
        if (::recv(sd, &ch, sizeof(ch), 0) <= 0)
        {
            throw string ("Connection error.");
        }
        ret = ret + ch;
    }
    
    return ret.substr(0, ret.length() - 1);
}

bool RemotePlayer::disconnect()
{
    return 0 != close(sd);
}

RemotePlayer::~RemotePlayer()
{
    /* Nothing to do. */
}
