#include "ButtonChat.hpp"

const int WIDTH_LIMIT = 492;

ButtonChat::ButtonChat(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, std::string imagesPath, std::string name): Button(windows, renderer, x, y, fontPath, imagesPath){
    userName = name;
}

ButtonChat::~ButtonChat(){

}

void ButtonChat::draw(){
    Button::draw();
    int x = buttonRect.x;
    int y = buttonRect.y;
    // NAME SHIFT
    x += 100;
    y += 35;
    renderText(userName, x, y, gFontNormal, WIDTH_LIMIT, ren);
}
