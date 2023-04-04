#ifndef NEWRECEIVER_H
#define NEWRECEIVER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "State.hpp"
#include "Button.hpp"

class NewReceiver : public State {
    private:
        bool confirmNewChat;
        std::string inputText;
        std::string pathImages;

    public:
        NewReceiver(std::string,std::string);
        virtual ~NewReceiver();
        int execute();
        bool getConfirmNewChat();
};


#endif /* NEWRECEIVER_H */