#include <iostream>
#include "Button.hpp"

const int WIDTH_LIMIT = 492;

Button::Button(){
    
}

Button::Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, const char* file) {
    gFontNormal = TTF_OpenFont("fonts/arial.ttf", global::FONT_SIZE);
    if (gFontNormal == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    win = windows;
    ren = renderer;
    buttonSurface = IMG_Load(file);
    buttonTexture = SDL_CreateTextureFromSurface(ren, buttonSurface);
    int buttonWidth = buttonSurface->w;
    int buttonHeight = buttonSurface->h;
    buttonRect = { x, y, buttonWidth, buttonHeight };
    SDL_FreeSurface(buttonSurface);
}

Button::Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y) {
    gFontNormal = TTF_OpenFont("fonts/arial.ttf", global::FONT_SIZE);
    if (gFontNormal == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    win = windows;
    ren = renderer;
    buttonSurface = IMG_Load("profileChat.png");
    buttonTexture = SDL_CreateTextureFromSurface(ren, buttonSurface);
    int buttonWidth = buttonSurface->w;
    int buttonHeight = buttonSurface->h;
    buttonRect = { x, y, buttonWidth, buttonHeight };
    SDL_FreeSurface(buttonSurface);
}

Button::~Button(){
    
}

void Button::draw(){
    SDL_RenderCopy(ren, buttonTexture, NULL, &buttonRect);
}

void Button::drawUserName(){
    SDL_Rect userNameRect = {buttonRect.x+50, buttonRect.y+10, buttonRect.w, buttonRect.h};
    renderText(userName, userNameRect.x, userNameRect.y+userNameRect.h, gFontNormal, WIDTH_LIMIT, ren);
}

void Button::setWindowRenderer(SDL_Window * newWindow, SDL_Renderer * newRen, SDL_Texture* newTexture){
    win = newWindow;
    ren = newRen;
    buttonTexture = newTexture;
}

void Button::insertUserName(std::string name){
    userName = name;
}

SDL_Rect Button::getButtonRect(){
    return buttonRect;
}

SDL_Texture* Button::getTexture(){
    return buttonTexture;
}