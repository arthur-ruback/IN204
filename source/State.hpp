#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "globals.hpp"
#include "common.hpp"

using namespace std;

class State{
    public:
        State();
        virtual ~State();
        virtual int execute() = 0;
        std::string getNameUser();
        void setNameUser(std::string newName);

    protected:
        TTF_Font* gFontNormal;
        TTF_Font* gFontSmall;
        SDL_Color gTextColor = {0, 0, 0};
        SDL_Renderer* gMainRenderer;
        std::string nameUser;
        // TODO: unificar coisas comuns de janelas para todos
};


#endif /* STATE_H */