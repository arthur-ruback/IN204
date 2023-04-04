#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>  
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Message.hpp"
#include "MsgContainer.hpp"
#include "common.hpp"
#include "State.hpp"

// WARNING, depends heavely on constants defined in globals.hpp
#include "globals.hpp"

// TODO: rever função render msgs para comprimir

class Chat : public State {
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        int TEXT_BOX_HEIGHT;
        int MAX_MSG_WIDTH;
        SDL_Window* Window;
        SDL_Renderer* renderer;
        TTF_Font* fontNormal;
        TTF_Font* fontSmall;
        TTF_Font* fontBig;
        SDL_Color textColor;
        MsgContainer* msgContainer;
        std::string whoImTalkingTo;

    public:
        Chat(std::string, std::string);
        ~Chat();
        void addMessage(Message);
        int execute();
    private:
        void renderMessages(int);
        void scrollMessages(int&, int, int amount=0);

};