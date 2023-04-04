#include <iostream>
#include "MainMenu.hpp"
#include "chat.hpp"
#include "NewReceiver.hpp"
#include "globals.hpp"

using namespace std;


int main(int argc, char** argv){
    std::vector<Button*> chats;
    MainMenu mainMenu = MainMenu(&chats);
    Chat chat = Chat(std::string("fonts/arial.ttf"), "Arthur");
    NewReceiver newReceiver = NewReceiver();

    chat.addMessage(Message({"Hello", "John","12","00"}));
    
    int currentState = global::MAINMENU;
    
    while (true) {
        switch (currentState) {
        case global::MAINMENU:
            mainMenu.confirmNewChat(newReceiver.getConfirmNewChat());
            mainMenu.setNameUser(newReceiver.getNameUser());
            currentState = mainMenu.execute();
            break;
        case global::CHAT:
            currentState = chat.execute();
            break;
        case global::NEWRECEIVER:
            currentState = newReceiver.execute();
            break;
        }
    }
    


    return 0;
}