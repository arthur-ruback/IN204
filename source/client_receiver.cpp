#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"
#include "Socket.hpp"

int main()
{
    Socket socket("127.0.0.1", 53001, 2, RECIEVER);
    int aux;
    while (1)
    {
        std::cin >> aux;
        MessageNet recv = socket.recv();
        std::cout << "Recieved: " << recv << std::endl;
    }
    
    return 0;
}