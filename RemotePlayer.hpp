//
//  RemotePlayer.hpp
//  reversi
//
//  Created by Kurian Jacob on 23/09/16.
//  Copyright © 2016 Kurian Jacob. All rights reserved.
//

#ifndef RemotePlayer_hpp
#define RemotePlayer_hpp

#include <stdio.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using std::string;

class RemotePlayer
{
private:
    in_addr_t ipaddr;
    in_port_t port;
    
    int sd;
    
public:
    RemotePlayer (in_addr_t ipaddr, in_port_t port);
    
    bool connect ();
    bool send_rnum(const char *rnum);
    
    bool send_string (const string &str);
    string receive_string();
    bool disconnect();
    
    ~RemotePlayer();
};

#endif /* RemotePlayer_hpp */
