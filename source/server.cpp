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


    std::list<sf::TcpSocket*> clients;
    sf::SocketSelector selector;

    // to be notified in case of new connection
    selector.add(listener);

    while (1) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                // pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    // Add the new client to the clients list
                    clients.push_back(client);
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (auto it : clients)
                {
                    sf::TcpSocket& client = *it;
                    if (selector.isReady(client))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        Message toRead;
                        unsigned status = client.receive(packet);
                        if (status == sf::Socket::Done)
                        {
                            if(packet >> toRead){
                                std::cout << "SUCESS: " << toRead << std::endl;
                            }
                            else{
                                std::cerr << "ERROR EXTRACTING" << std::endl;
                                break;
                            }
                        }
                        else if (status == sf::Socket::Disconnected)
                        {
                            std::cout << "Disconnecting from " << client.getRemoteAddress() << std::endl;
                            std::cout << "Disconnecting from " << client.getRemoteAddress() << std::endl;
                            std::cout << "D" << std::endl;
                            clients.remove(&client);
                            std::cout << "D" << std::endl;
                            selector.remove(*it);
                            delete &client;
                            std::cout << "D" << std::endl;
                        }
                    }
                }
            } // if socket is ready
        }// if selector.wait
    }// while


}

    