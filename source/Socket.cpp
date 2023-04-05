#include "Socket.hpp"

Socket::Socket(std::string ip, unsigned port, unsigned myID, int type, unsigned link) : myID(myID), myType(type){

    // socket creation
    sf::Socket::Status status = socket.connect(ip, port);
    if (status != sf::Socket::Done)
    {
        std::cerr << "Falha na connexão" << std::endl;
        validSocket = false;
    } else {
        validSocket = true;
    }

    if(validSocket){
        // Server handshake
        MessageNet authMsg = {MSG_CONX_REQ, myID, SERVER_ID, std::to_string(myType)};
        toSend << authMsg;
        std::cout << "Sending: " << authMsg << std::endl;
        socket.send(toSend);

        socket.receive(toRecv);
        toRecv >> recvMsg;
        std::cout << "Recieved: " << recvMsg << std::endl;
        if (recvMsg.sender != 0 || recvMsg.msgType != MSG_CONX_REP || recvMsg.dest != myID || recvMsg.content.compare("Aproved")){
            std::cout << "Connection refused by server" << std::endl;
            validSocket = false;
        }   

        // Link to another client?
        if(link && myType == EMITTER){
            MessageNet linkMsg = {MSG_LINK_REQ, myID, SERVER_ID, std::to_string(link)};
            toSend.clear();
            toSend << linkMsg;
            std::cout << "Sending: " << linkMsg << std::endl;
            socket.send(toSend);
        }
    } else {
        std::cout << "Invalid socket, no operation will be made" << std::endl;
    }
}

Socket::~Socket(){

}

int Socket::send(unsigned destID, std::string msg){

    if(!validSocket){
        std::cout << "Invalid socket, no operation will be made" << std::endl;
        return -1;
    }
    if(myType != EMITTER){
        std::cout << "Operation forbidden in this type os socket" << std::endl;
        return -1;
    }

    sendMsg.msgType = MSG_STD;
    sendMsg.sender = myID;
    
    sendMsg.dest = destID;
    sendMsg.content = msg;
    
    toSend.clear();
    toSend << sendMsg;
    std::cout << sendMsg << std::endl;
    socket.send(toSend);

    socket.receive(toRecv);
    toRecv >> recvMsg;
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_OK || recvMsg.dest != myID){
        std::cout << "Error: " << recvMsg.content << std::endl;
    }
    return 0;
}

MessageNet Socket::recv(){
    if(myType != RECIEVER){
        std::cout << "Operation forbidden in this type os socket" << std::endl;
        return MessageNet();
    }
    socket.receive(toRecv);
    toRecv >> recvMsg;
    return recvMsg;
}