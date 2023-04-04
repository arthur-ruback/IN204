# include "State.hpp"

State::State(std::string fontPath){
    Window = nullptr;
    renderer = nullptr;
    fontNormal = nullptr;
    fontSmall = nullptr;
    fontBig = nullptr;
    textColor = { 0, 0, 0 };

    // initialization SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        logSDLError("SDL_Init");

    if (TTF_Init() != 0)
        logSDLError("TTF_Init");

    fontNormal = TTF_OpenFont(fontPath.c_str(), global::FONT_SIZE);
    if (fontNormal == nullptr)
        logSDLError("TTF_OpenFont");

    fontSmall = TTF_OpenFont(fontPath.c_str(), (int)global::FONT_SIZE/2);
    if (fontSmall == nullptr)
        logSDLError("TTF_OpenFont");

    fontBig = TTF_OpenFont(fontPath.c_str(), (int)global::FONT_SIZE*1.5);
    if (fontBig == nullptr)
        logSDLError("TTF_OpenFont");
    nameUser = "";
}

State::~State(){
    TTF_Quit();
    SDL_Quit();
}

std::string State::getNameUser(){
    return nameUser;
}

void State::setNameUser(std::string newName){
    nameUser = newName;
}