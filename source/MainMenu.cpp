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
}

MainMenu::~MainMenu() {}

int MainMenu::execute(){

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
    ButtonChat * button = new ButtonChat(window, ren, 0, 70, global::pathToFont, std::string(pathImages+"profileChat.png"), "Arthur");
    SDL_Rect buttonRect = button->getButtonRect();
    updateChats(window, ren, button->getTexture());
    if (createNewChat){
        nbChats += 1;
        std::cout << nameUser << std::endl;
        addNewChat(window, ren);
        createNewChat = false;
    }

    //MONTAR ESPACO BRANCO SEM CHAT
    SDL_Rect conversasRect = {0, 162+nbChats*92, 563, 774-nbChats*92};

    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    exit(0);
                case SDL_MOUSEBUTTONDOWN:
                    // Verifique se o botão foi pressionado
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    SDL_Point mousePoint = {mouseX, mouseY};
                    if (SDL_PointInRect(&mousePoint, &buttonRect)) {
                        buttonNewChat = false;
                        //printf("Botão 0 pressionado!\n");
                        running = false;
                    }
                    for (auto i : *chats){
                        SDL_Rect chatRect = i->getButtonRect();
                        if (SDL_PointInRect(&mousePoint, &chatRect)) {
                            buttonNewChat = false;
                            //std::cout << "Botão " << i+1 << "pressionado!\n" << std::endl;
                            running = false;
                        }
                    }
                    if (SDL_PointInRect(&mousePoint, &buttonHeaderRect)) {
                        buttonNewChat = true;
                        //createNewChat = true;
                        //printf("Header pressionado!\n");
                        running = false;
                    }
                    break;
            }

        }

        //CABECALHO
        SDL_RenderCopy(ren, textureHeader, NULL, &cabecalhoRect);
        buttonHeader->draw();

        //BUTTON
        button->draw();
        for (auto i : *chats){
            i->draw();
        }

        //ESPACO EM BRANCO
        // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        // SDL_RenderFillRect(ren, &conversasRect);

        SDL_RenderPresent(ren);
    }
    // Libere os recursos utilizados pela biblioteca SDL2
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (buttonNewChat){
        return NEWRECEIVER;
    } else{
        std::cout << "vou pro chat" << std::endl;
        return CHAT;
    }

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
