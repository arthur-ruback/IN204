#include <iostream>
#include <SFML/Network.hpp>
#include <list>
#include <map>
#include "common.hpp"



int main(){
    sf::TcpListener listener;

    if (listener.listen(53000) != sf::Socket::Done)
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

                                    // TODO: Authenticate
                                    (*client).id = toRead.sender;
                                    int aux = 0;
                                    (*client).type = std::stoi(toRead.content);

                                    toSend.msgType = MSG_CONX_REP;
                                    toSend.sender = SERVER_ID;
                                    toSend.dest = (*client).id;
                                    toSend.content = std::string("Aprooved");
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

    