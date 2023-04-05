#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"
#include "Socket.hpp"

int main()
{
    Socket socket("127.0.0.1", 53001, 1, EMITTER);
    int aux;
    while (true)
    {
        std::cin >> aux;
        socket.send(2, std::string("Essa eh pro 2"));
        /* code */
    }
    
    return 0;
}