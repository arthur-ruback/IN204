#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "MainMenu.hpp"
#include "Button.hpp"

const int CHAT = 2;
const int NEWRECEIVER = 3;

MainMenu::MainMenu(std::vector<ButtonChat*> *_chats, std::string fontPath, std::string imagesPath) : chats(_chats), State(fontPath) {
    pathImages = imagesPath;
    nbChats = 0;
    nextState = global::MAINMENU;
}

MainMenu::~MainMenu() {}

int MainMenu::execute(){

    nextState = global::MAINMENU;

    SDL_Window * window = SDL_CreateWindow("Artheus Web",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            563, //window_width
                            844, //window height
                            SDL_WINDOW_SHOWN);

    SDL_Renderer * ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    bool running = true;

    //MONTAR HEADER
    SDL_Texture * textureHeader = IMG_LoadTexture(ren, std::string(pathImages+"header.png").c_str());
    SDL_Rect cabecalhoRect = {0, 0, 373, 70};
    Button * buttonHeader = new Button(window, ren, 373, 0, global::pathToFont, std::string(pathImages+"newChat.png"));
    SDL_Rect buttonHeaderRect = buttonHeader->getButtonRect();

    //MONTAR CHATS (BOTOES)
    if (createNewChat){
        std::cout << nameUser << std::endl;
        addNewChat(window, ren);
        updateChats(window, ren, chats->back()->getTexture());
        nbChats += 1;
        createNewChat = false;
    }

    //MONTAR ESPACO BRANCO SEM CHAT
    SDL_Rect conversasRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Event event;
    bool flagRenderText = true;
    while (running){
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    nextState = global::DIE;
                    break;
                case SDL_MOUSEWHEEL:
                    //TODO: scroll mgs
                    flagRenderText = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // Verifique se o botão foi pressionado
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    SDL_Point mousePoint = {mouseX, mouseY};
                    /*if (SDL_PointInRect(&mousePoint, &buttonRect)) {
                        buttonNewChat = false;
                        //printf("Botão 0 pressionado!\n");
                        running = false;
                    }*/
                    for (auto i = 0; i < chats->size(); i++){
                        SDL_Rect chatRect = (*chats)[i]->getButtonRect();
                        if (SDL_PointInRect(&mousePoint, &chatRect)) {
                            std::cout << "Botão " << i+1 << "pressionado!\n" << std::endl;
                            running = false;
                        }
                    }
                    if (SDL_PointInRect(&mousePoint, &buttonHeaderRect)) {
                        nextState = global::NEWRECEIVER;
                        running = false;
                    }
                    break;
            }

        }

        if(flagRenderText){

            //CABECALHO
            SDL_RenderCopy(ren, textureHeader, NULL, &cabecalhoRect);
            buttonHeader->draw();

            //ESPACO EM BRANCO
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderFillRect(ren, &conversasRect);

            //BUTTON
            for (auto i : *chats){
                i->draw();
            }

            SDL_RenderPresent(ren);
            flagRenderText = false;
        }
    }
    // Libere os recursos utilizados pela biblioteca SDL2
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    DEBUG("next state is ", nextState);
    return nextState;

}

void MainMenu::addNewChat(SDL_Window * window, SDL_Renderer * ren){
    ButtonChat *newChat = new ButtonChat(window, ren, 0, 70+nbChats*92, global::pathToFont, std::string(pathImages+"profileChat.png"), nameUser);
    chats->push_back(newChat);
}

void MainMenu::updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture){
    for (auto i : *chats){
        i->setWindowRenderer(window, ren, buttonTexture);
    }
}

void MainMenu::confirmNewChat(bool confirm){
    if (confirm){
        createNewChat = true;
    } else{
        createNewChat = false;
    }
}

void MainMenu::scrollMessages(int& yOffset, int heightMsgs, int amount) {
    yOffset += amount * global::SCROLL_SPEED;
    // not enough chats to fill the screen
    if (heightMsgs < SCREEN_HEIGHT - (global::HEADER_HEIGHT + global::V_SPACING)){
        yOffset = global::HEADER_HEIGHT + global::V_SPACING;
    // enough messages to fill the screen
    } else {
        // first message always on the top of the screen or hidden (y < HEADER_HEIGHT)
        if (yOffset > global::HEADER_HEIGHT + global::V_SPACING)
            yOffset = global::HEADER_HEIGHT + global::V_SPACING;
        // cannot have empty pace on the bottom if there are sufficint messages 
        else if (yOffset < (SCREEN_HEIGHT - heightMsgs))
            yOffset = (SCREEN_HEIGHT - heightMsgs);
    }
}