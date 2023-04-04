#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common.hpp"
#include "globals.hpp"

class Button{
    private:
        TTF_Font* gFontNormal;
        SDL_Window * win;
        SDL_Renderer * ren;
        SDL_Surface* buttonSurface;
        SDL_Texture* buttonTexture;
        SDL_Rect buttonRect;
        //std::string userName;
    
    public:
        std::string userName;
        Button();
        Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, const char* file);
        Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y);
        virtual ~Button();
        void draw();
        void drawUserName();
        void setWindowRenderer(SDL_Window * newWindow, SDL_Renderer * newRen, SDL_Texture* newTexture);
        SDL_Rect getButtonRect();
        SDL_Texture* getTexture();
        void insertUserName(std::string userName);
};

#endif /* BUTTON_H */