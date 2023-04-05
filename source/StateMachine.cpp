#include <iostream>
#include <thread>
#include <map>
#include "MainMenu.hpp"
#include "chat.hpp"
#include "NewReceiver.hpp"
#include "ButtonChat.hpp"
#include "Login.hpp"
#include "globals.hpp"
#include "Socket.hpp"

using namespace std;

#define ENABLE_DEBUG

//TODO: mutex for variables
std::vector<ButtonChat*> chats;
bool globalRun;

Socket* outbound;

std::map<int,std::string> idToUsername;
std::map<std::string,int> usernameToId;

void recvThread(std::vector<ButtonChat*> *chats, std::string ip, std::string username){
    Socket inbound(ip, SERVERPORT, NOIDYET, RECIEVER, username);
    // link sockets
    outbound = new Socket(ip, SERVERPORT, NOIDYET, EMITTER, username, inbound.getID());
    std::cout << "socket created in thread" << std::endl;
    MessageNet recvMsg;
    while(globalRun){
        recvMsg = inbound.recv();

        // failed receiving
        if (recvMsg.dest == 0)
            globalRun = false;

        bool foundChat = false;
        // look for chat with specified name
        for (auto i : *chats){
            // chat already exists and add msg
            if(i->getUserName() == idToUsername[recvMsg.sender]) {
                i->getChat()->addMessage(Message(recvMsg.content, i->getUserName()));
                foundChat = true;
                std::cout << "Adding to existing chat:" << i->getUserName() << "\" number " << recvMsg.sender << std::endl;
                break;
            }
        }
        // chat doenst exist yet and needs to create it
        if(foundChat == false){
            // needs name of chatter, is gonna demand the server
            std::string name = inbound.getUsernameFromServer(recvMsg.sender);

            if(name != std::string()){
                // now that has it, create client
                Chat * newChat = new Chat(std::string(global::pathToFont), name);
                ButtonChat *newButtonChat = new ButtonChat(NULL, NULL, 0, 0, global::pathToFont, std::string(global::pathToImgs+"profileChat.png"), name, newChat);
                chats->push_back(newButtonChat);
                chats->back()->getChat()->addMessage(Message(recvMsg.content, name));
                std::cout << "Adding to new chat:" << chats->back()->getUserName() << std::endl;
                idToUsername[recvMsg.sender] = name;
                usernameToId[name] = recvMsg.sender;
            }else{
                std::cout << "Got bad reponse from server" << std::endl;
            }
        }
    }
    delete outbound;
    DEBUG("Thread receiving ended");
}

int main(int argc, char** argv){
    globalRun = true;
    std::thread *threadRecv;

    MainMenu mainMenu = MainMenu(&chats, global::pathToFont, global::pathToImgs);
    NewReceiver newReceiver = NewReceiver(global::pathToFont, global::pathToImgs);
    Login login = Login(global::pathToFont, global::pathToImgs);
    Chat *chat = NULL;
    
    int currentState = global::LOGIN;
    
    while (globalRun) {
        std::cout << "in switch " << currentState << std::endl;
        if(currentState == global::MAINMENU){
            std::cout << "Entering main menu" << std::endl;
            currentState = mainMenu.execute(outbound);
        } else if(currentState == global::CHAT){
            std::cout << "Entering chat" << std::endl;
            chat = mainMenu.getChatSelected();
            currentState = chat->execute(outbound, &usernameToId);
        } else if(currentState == global::NEWRECEIVER){
            std::cout << "Entering newreceiver" << std::endl;
            currentState = newReceiver.execute();
            mainMenu.setReceiverName(newReceiver.getReceiverName());
        } else if(currentState == global::LOGIN){
            std::cout << "Entering login" << std::endl;
            currentState = login.execute();
            DEBUG("at the end of login");
            if(currentState == global::MAINMENU){
                threadRecv = new std::thread(recvThread, &chats, std::string(argv[1]),login.getUserName());
            }
            mainMenu.setUserName(login.getUserName());
        } else if(currentState == global::DIE){
            std::cout << "dieing.." << std::endl;
            globalRun = false;
            std::cout << "dieing.." << std::endl;
        } else {
            globalRun = false;
        std::cout << "end of switch" << std::endl;
        }
    }
    
    if(!threadRecv){
        threadRecv->join();
        delete threadRecv;
    }
    std::cout << "bye" << std::endl;
    return 0;
}