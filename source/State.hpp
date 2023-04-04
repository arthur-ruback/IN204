#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class State{
    public:
        State();
        virtual ~State();
        virtual int execute() = 0;

    protected:
        // TODO: unificar coisas comuns de janelas para todos
};


#endif /* STATE_H */