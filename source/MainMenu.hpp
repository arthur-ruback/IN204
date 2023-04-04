#ifndef MAINMENU_H
#define MAINMENU_H
#include <vector>
#include "State.hpp"
#include "Button.hpp"

class MainMenu : public State{
    private:
        bool buttonNewChat;
        bool createNewChat;
        int nbChats;
        std::vector<Button*> *chats;
        std::string pathImages;
    
    public:
        MainMenu(std::vector<Button*> *_chats,std::string fontPath, std::string imagesPath);
        virtual ~MainMenu();
        void addNewChat(SDL_Window * window, SDL_Renderer * ren);
        void updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture);
        int execute();
        void confirmNewChat(bool confirm);
};

#endif /* MAINMENU_H */