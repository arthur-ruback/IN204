#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"

int main()
{
    sf::Packet toSend, toRecv;
    Message sendMsg, recvMsg;
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 53000);
    if (status != sf::Socket::Done)
    {
         std::cerr << "Falha na connexão" << std::endl;
    }

    unsigned myID = SERVER_ID;
    while(myID == SERVER_ID){
        std::cout << "Who am I? (My ID plz)" << std::endl;
        std::cin >> myID;
        if (myID == SERVER_ID){
            std::cout << "Sorry, it cannot be 0 :(" << std::endl;
        }
    }

    // Server handshake
    Message authMsg = {MSG_CONX_REQ, myID, SERVER_ID, std::string("0")};
    toSend << authMsg;
    std::cout << "Sending: " << authMsg << std::endl;
    socket.send(toSend);

    socket.receive(toRecv);
    toRecv >> recvMsg;
    std::cout << "Recieved: " << recvMsg << std::endl;
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_CONX_REP || recvMsg.dest != myID || recvMsg.content.compare("Aprooved")){
        std::cout << "Connection refused by server" << std::endl;
        return 0;
    }   

    while (1)
    {
        sendMsg.msgType = MSG_STD;
        sendMsg.sender = myID;
        
        std::cout << "to who?" << std::endl;
        std::cin >> sendMsg.dest;

        std::cout << "type to send:" << std::endl;
        std::cin >> sendMsg.content;
        
        toSend.clear();
        toSend << sendMsg;
        std::cout << sendMsg << std::endl;
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