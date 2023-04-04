# include "State.hpp"

State::State(){
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