#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"
#include "Socket.hpp"

int main()
{
    Socket socket("127.0.0.1", SERVERPORT, NOIDYET, EMITTER,"EmissorTeste");
    int aux;
    while (true)
    {
        std::cin >> aux;
        socket.send(1, std::string("Essa eh pro 1"));
        /* code */
    }
    
    return 0;
}