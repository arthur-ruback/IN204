#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"

int main()
{
    sf::Packet toSend, toRecv;
    MessageNet sendMsg, recvMsg;
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 53001);
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
    MessageNet authMsg = {MSG_CONX_REQ, myID, SERVER_ID, std::string("1")};
    toSend << authMsg;
    std::cout << "Sending: " << authMsg << std::endl;
    socket.send(toSend);

    socket.receive(toRecv);
    toRecv >> recvMsg;
    std::cout << "Recieved: " << recvMsg << std::endl;
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_CONX_REP || recvMsg.dest != myID || recvMsg.content.compare("Aproved")){
        std::cout << "Connection refused by server" << std::endl;
        return 0;
    }   

    while (1)
    {
        socket.receive(toRecv);
        toRecv >> recvMsg;
        std::cout << "Recieved: " << recvMsg << std::endl;
    }
    
    std::cout << "Done Sending" << std::endl;

    return 0;
}