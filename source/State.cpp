# include "State.hpp"

State::State(){
    nameUser = "batatinha";
}

State::~State(){}

std::string State::getNameUser(){
    return nameUser;
}

void State::setNameUser(std::string newName){
    nameUser = newName;
}