#ifndef MAINMENU_H
#define MAINMENU_H
#include <vector>
#include "State.hpp"
#include "Button.hpp"
#include "ButtonChat.hpp"
#include "chat.hpp"

class MainMenu : public State{
    private:
        bool buttonNewChat;
        bool createNewChat;
        int nbChats;
        std::vector<ButtonChat*> *chats;
        std::string pathImages;
        Chat * chatSelected;
    
    public:
        MainMenu(std::vector<ButtonChat*> *_chats,std::string fontPath, std::string imagesPath);
        virtual ~MainMenu();
        void addNewChat(SDL_Window * window, SDL_Renderer * ren);
        void updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture);
        int execute();
        void confirmNewChat(bool confirm);
        Chat * getChatSelected();
};

#endif /* MAINMENU_H */