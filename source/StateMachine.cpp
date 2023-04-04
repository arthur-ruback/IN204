#include <iostream>
#include "MainMenu.hpp"
#include "chat.hpp"
#include "NewReceiver.hpp"
#include "globals.hpp"

using namespace std;


int main(int argc, char** argv){
    std::vector<Button*> chats;
    MainMenu mainMenu = MainMenu(&chats, global::pathToFont, global::pathToImgs);
    DEBUG("Done creating main menu");
    Chat chat = Chat(std::string(global::pathToFont), "Arthur");
    DEBUG("Done creating chat");
    NewReceiver newReceiver = NewReceiver(global::pathToFont, global::pathToImgs);
    DEBUG("Done creating newRecheiver");

    chat.addMessage(Message({"Hello", "John","12","00"}));
    
    int currentState = global::MAINMENU;
    
    while (true) {
        switch (currentState) {
        case global::MAINMENU:
            DEBUG("executing mainmenu");
            mainMenu.confirmNewChat(newReceiver.getConfirmNewChat());
            mainMenu.setNameUser(newReceiver.getNameUser());
            currentState = mainMenu.execute();
            break;
        case global::CHAT:
            DEBUG("executing chat");
            currentState = chat.execute();
            break;
        case global::NEWRECEIVER:
            DEBUG("executing newreceiver");
            currentState = newReceiver.execute();
            break;
        }
    }
    


    return 0;
}