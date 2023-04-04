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
#include "Button.hpp"

// WARNING, depends heavely on constants defined in globals.hpp
#include "globals.hpp"

class Chat : public State {
    private:
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