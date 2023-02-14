#include <iostream>
#include <SFML/Network.hpp>
#include <list>
#include <map>
#include "common.hpp"

bool existsClientId(std::list<ClientData> &clients, int id){
    for (auto client: clients){
        if(client.id == id)
            return 1;
    }
    return 0;
}

ClientData* findClientById(std::list<ClientData> &clients, int id){
    for (auto & client: clients){
        if(client.id == id)
            return &client;
    }
    return NULL;
}

int main(){
    sf::TcpListener listener;

    if (listener.listen(53001) != sf::Socket::Done)
    {
        std::cerr << "Failed in binding to port" << std::endl;
        return -1;
    }


    std::list<ClientData> clients;
    sf::SocketSelector selector;

    Message toRead, toSend;

    // to be notified in case of new connection
    selector.add(listener);

    while (1) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                // pending connection
                sf::TcpSocket* sock = new sf::TcpSocket;
                if (listener.accept(*sock) == sf::Socket::Done)
                {
                    ClientData aux = {NULL, 0, 0, 0};
                    aux.socket = sock;
                    // Add the new client to the clients list
                    clients.push_back(aux);
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*sock);
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete sock;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (auto client = clients.begin(); client != clients.end(); client++)
                {
                    //sf::TcpSocket* client = it;
                    if (selector.isReady(*((*client).socket)))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        
                        unsigned status = ((*client).socket)->receive(packet);
                        if (status == sf::Socket::Done)
                        {
                            if(packet >> toRead){
                                std::cout << "SUCESS: " << toRead << std::endl;

                                // Demanding connexion
                                if(toRead.msgType == MSG_CONX_REQ){

                                    if(existsClientId(clients,toRead.sender)){
                                        // client claiming to be someone else
                                        std::cout << "Disconnecting from repeated id client: " << ((*client).socket)->getRemoteAddress() << std::endl;
                                        selector.remove(*((*client).socket));
                                        delete (*client).socket;
                                        client = clients.erase(client);
                                        client--;
                                    }else{

                                        // TODO: Authenticate
                                        (*client).id = toRead.sender;
                                        int aux = 0;
                                        (*client).type = std::stoi(toRead.content);

                                    

                                        toSend.msgType = MSG_CONX_REP;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Aproved");
                                        packet.clear();
                                        packet << toSend;

                                        ((*client).socket)->send(packet);
                                        std::cout << "Admited " << (*client).id << " as " << (((*client).type)? "reciever" : "emitter") << std::endl;
                                    }
                                }else if(toRead.msgType == MSG_STD){
                                    ClientData *clientData = findClientById(clients,toRead.dest);
                                    if(clientData == NULL){
                                        // destination not registered, return error to sender
                                        toSend.msgType = MSG_ERROR;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Destination not registered in database");
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                        std::cout << "Client " << toRead.dest << " not registered in database" << std::endl;
                                    }else if(((*clientData).type == EMITTER) && (*clientData).link == 0){
                                        // destination not adequate to recv data, return error to sender
                                        toSend.msgType = MSG_ERROR;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Destination is a emitter and is not linked to any receiver");
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                        std::cout << "Client " << toRead.dest << " is not a reciever nor is linked to one" << std::endl;
                                    }else{
                                        // redirecting through link or normal relay
                                        if ((*clientData).link != 0)
                                            clientData = findClientById(clients,(*clientData).link);
                                        (*clientData).socket->send(packet);
                                        std::cout << "Relaying msg to " << toRead.dest << std::endl;
                                        toSend.msgType = MSG_OK;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string();
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                    }
                                // try to link to another client
                                }else if(toRead.msgType == MSG_LINK_REQ){
                                    (*client).link = std::stoi(toRead.content);
                                    std::cout << "Linked " << (*client).id << " and " << (*client).link << std::endl;
                                    toSend.msgType = MSG_OK;
                                    toSend.sender = SERVER_ID;
                                    toSend.dest = (*client).id;
                                    toSend.content = std::string();
                                    packet.clear();
                                    packet << toSend;
                                    ((*client).socket)->send(packet);
                            
                                }
                            }
                            else{
                                std::cerr << "ERROR EXTRACTING" << std::endl;
                                break;
                            }
                        }
                        else if (status == sf::Socket::Disconnected)
                        {
                            std::cout << "Disconnecting from " << ((*client).socket)->getRemoteAddress() << std::endl;
                            selector.remove(*((*client).socket));
                            delete (*client).socket;
                            client = clients.erase(client);
                            client--;   // since erase returns next element and loop also does this, needs to go back 1
                        }
                    }
                }
            } // if socket is ready
        }// if selector.wait
    }// while


}

    