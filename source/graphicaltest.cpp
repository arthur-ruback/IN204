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
#include "globals.hpp"
#include "common.hpp"
#include "chat.hpp"

#define ENABLE_DEBUG

int main(int argc, char* argv[]) {

    std::string whoImTalkingTo("Bob L'éponge");
    Chat chat(std::string("source/fonts/arial.ttf"), whoImTalkingTo);

    // Simulates messages from server
    chat.addMessage(Message({"Hello", "John","12","00"}));
    chat.addMessage(Message({"Hi there", "Me","12","02"}));
    chat.addMessage(Message({"How are you?", "John","12","03"}));
    chat.addMessage(Message({"I'm good, thanks. How about you? Isso ainda é um teste :)", "Mary","13","29"}));
    chat.addMessage(Message({"Isso ainda é um teste? Eu acredito plenamente que sim!", "Me","13","35"}));
    chat.addMessage(Message({"Great to hear that", "Mary","13","49"}));

    for(int i = 0; i < 2; i++){
        chat.execute();
        chat.addMessage(Message({"Uai, funciona mesmo?", "John","04","09"}));
    }
}