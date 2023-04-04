#include "ButtonChat.hpp"

const int WIDTH_LIMIT = 492;

ButtonChat::ButtonChat(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, std::string imagesPath, std::string name): Button(windows, renderer, x, y, fontPath, imagesPath){
    userName = name;
}

ButtonChat::~ButtonChat(){

}

void ButtonChat::draw(){
    Button::draw();
    SDL_Rect userNameRect = {buttonRect.x+50, buttonRect.y+10, buttonRect.w, buttonRect.h};
    renderText(userName, userNameRect.x, userNameRect.y+userNameRect.h, gFontNormal, WIDTH_LIMIT, ren);
}
