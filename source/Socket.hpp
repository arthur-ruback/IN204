# pragma once

#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <chrono>

#include "common.hpp"

class Socket {
    private:
        sf::Packet toSend, toRecv;
        MessageNet sendMsg, recvMsg;
        sf::TcpSocket socket;
        bool validSocket;
        unsigned myID;
        int myType;
    public:
        Socket(std::string ip, unsigned port, unsigned myID, int type, unsigned link = 0);
        ~Socket();
        int send(unsigned destID, std::string msg);
        MessageNet recv();
};