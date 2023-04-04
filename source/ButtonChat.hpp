#ifndef BUTTONCHAT_H
#define BUTTONCHAT_H

#include "Button.hpp"
#include "chat.hpp"

class ButtonChat : public Button{
    private:
        std::string userName;

    public:
        ButtonChat(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, std::string imagesPath, std::string name);
        ~ButtonChat();
        void draw();
        
};

#endif /* BUTTONCHAT_H */