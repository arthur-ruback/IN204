#include <iostream>
#include <thread>
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

void recvThread(std::vector<ButtonChat*> *chats){
    Socket inbound("127.0.0.1", SERVERPORT, 2, RECIEVER);
    MessageNet recvMsg;
    while(globalRun){
        recvMsg = inbound.recv();

        // failed receiving
        if (recvMsg.dest == 0)
            globalRun = false;

        bool foundChat = false;
        // look for chat with specified name
        for (auto i : *chats){
            // TODO: translate id to username
            // chat already exists and add msg
            if(i->getUserName() == to_string(recvMsg.sender)) {
                i->getChat()->addMessage(Message(recvMsg.content, to_string(recvMsg.sender)));
                foundChat = true;
                std::cout << "Adding to existing chat:" << i->getUserName() << std::endl;
                break;
            }
        }
        // chat doenst exist yet and needs to create it
        if(foundChat == false){
            Chat * newChat = new Chat(std::string(global::pathToFont), to_string(recvMsg.sender));
            ButtonChat *newButtonChat = new ButtonChat(NULL, NULL, 0, 0, global::pathToFont, std::string(global::pathToImgs+"profileChat.png"), to_string(recvMsg.sender), newChat);
            chats->push_back(newButtonChat);
            chats->back()->getChat()->addMessage(Message(recvMsg.content, to_string(recvMsg.sender)));
            std::cout << "Adding to new chat:" << chats->back()->getUserName() << std::endl;
        }
    }
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
            currentState = mainMenu.execute();
        } else if(currentState == global::CHAT){
            std::cout << "Entering chat" << std::endl;
            chat = mainMenu.getChatSelected();
            currentState = chat->execute();
        } else if(currentState == global::NEWRECEIVER){
            std::cout << "Entering newreceiver" << std::endl;
            currentState = newReceiver.execute();
            mainMenu.setReceiverName(newReceiver.getReceiverName());
        } else if(currentState == global::LOGIN){
            std::cout << "Entering login" << std::endl;
            currentState = login.execute();
            DEBUG("at the end of login");
            if(currentState == global::MAINMENU){
                DEBUG("Here");
                threadRecv = new std::thread(recvThread, &chats);
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