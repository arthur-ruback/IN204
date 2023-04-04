#ifndef MAINMENU_H
#define MAINMENU_H
#include <vector>
#include "State.hpp"
#include "Button.hpp"
#include "ButtonChat.hpp"

class MainMenu : public State{
    private:
        int nextState;
        bool createNewChat;
        int nbChats;
        std::vector<ButtonChat*> *chats;
        std::string pathImages;
    
    public:
        MainMenu(std::vector<ButtonChat*> *_chats,std::string fontPath, std::string imagesPath);
        virtual ~MainMenu();
        void addNewChat(SDL_Window * window, SDL_Renderer * ren);
        void updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture);
        int execute();
        void confirmNewChat(bool confirm);
        void scrollMessages(int& yOffset, int heightMsgs, int amount);
};

#endif /* MAINMENU_H */