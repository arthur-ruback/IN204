#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"

int main()
{
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 53000);
    if (status != sf::Socket::Done)
    {
         std::cerr << "Falha na connexão" << std::endl;
    }

    unsigned myID = 0;
    std::cout << "Who am I? (My ID plz)" << std::endl;
    std::cin >> myID;

    while (1)
    {
        Message teste;
        teste.sender = myID;
        
        std::cout << "to who?" << std::endl;
        std::cin >> teste.dest;

        std::cout << "type to send:" << std::endl;
        std::cin >> teste.content;

        sf::Packet toSend;
        toSend << teste;
        std::cout << teste << std::endl;
        socket.send(toSend);
    }
    
    std::cout << "Done Sending" << std::endl;



    // sf::RenderWindow window(sf::VideoMode(1200, 1200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);

    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     window.clear();
    //     window.draw(shape);
    //     window.display();
    // }

    return 0;
}