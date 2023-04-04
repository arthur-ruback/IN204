#include <iostream>
#include "MainMenu.hpp"
#include "chat.hpp"
#include "NewReceiver.hpp"
#include "ButtonChat.hpp"
#include "globals.hpp"

using namespace std;


int main(int argc, char** argv){
    std::vector<ButtonChat*> chats;
    MainMenu mainMenu = MainMenu(&chats, global::pathToFont, global::pathToImgs);
    Chat *chat = new Chat(std::string(global::pathToFont), "Arthur");
    NewReceiver newReceiver = NewReceiver(global::pathToFont, global::pathToImgs);

    chat->addMessage(Message({"Hello", "John","12","00"}));
    
    int currentState = global::MAINMENU;
    
    while (true) {
        switch (currentState) {
        case global::MAINMENU:
            mainMenu.confirmNewChat(newReceiver.getConfirmNewChat());
            mainMenu.setNameUser(newReceiver.getNameUser());
            currentState = mainMenu.execute();
            break;
        case global::CHAT:
            chat = mainMenu.getChatSelected();
            currentState = chat->execute();
            break;
        case global::NEWRECEIVER:
            currentState = newReceiver.execute();
            break;
        //case global::MAKENEWCHAT:

        }
    }
    


    return 0;
}