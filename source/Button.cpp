#include <iostream>
#include "Button.hpp"

const int WIDTH_LIMIT = 492;

Button::Button(std::string fontPath, std::string imagesPath){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        logSDLError("SDL_Init");
    if (TTF_Init() != 0)
        logSDLError("TTF_Init");

    gFontNormal = TTF_OpenFont(fontPath.c_str(), global::FONT_SIZE);
    if (gFontNormal == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    pathImages = imagesPath;
}

Button::Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, const std::string imagePath) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        logSDLError("SDL_Init");
    if (TTF_Init() != 0)
        logSDLError("TTF_Init");

    gFontNormal = TTF_OpenFont(fontPath.c_str(), global::FONT_SIZE);
    if (gFontNormal == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    win = windows;
    ren = renderer;
    buttonSurface = IMG_Load(imagePath.c_str());
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