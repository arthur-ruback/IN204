#include "NewReceiver.hpp"

const int MAIN_MENU = 1;
const int WIDTH_LIMIT = 492;
const int MAX_CHARACTER = 15;
const int NORMAL = 1;

NewReceiver::NewReceiver(std::string fontPath, std::string imagesPath){
    confirmNewChat = false;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        logSDLError("SDL_Init");
    if (TTF_Init() != 0)
        logSDLError("TTF_Init");

    gFontNormal = TTF_OpenFont(fontPath.c_str(), global::FONT_SIZE);
    if (gFontNormal == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    gFontSmall = TTF_OpenFont(fontPath.c_str(), global::FONT_SMALL_SIZE);
    if (gFontSmall == nullptr) {
        std::cerr << "Error in font initialization" << std::endl;
    }
    pathImages = imagesPath;
}

NewReceiver::~NewReceiver(){
    TTF_Quit();
    SDL_Quit();
}

int NewReceiver::execute(){
    SDL_Window * windowAdd = SDL_CreateWindow("New Chat",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            600, //window_width
                            350, //window height
                            SDL_WINDOW_SHOWN);

    SDL_Renderer * rendererAdd = SDL_CreateRenderer(windowAdd, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    inputText = "";
    bool flagRenderText = true;

    // BACKGROUND
    SDL_Rect backgroundRect = {0, 0, 600, 350};

    // TO WHO
    SDL_Texture * alterarTexture = IMG_LoadTexture(rendererAdd, std::string(pathImages+"to.png").c_str());
    SDL_Rect cabecalhoRect = {50, 70, 60, 30};

    // NAME TEXT
    SDL_Rect edgeRect = {55, 115, 500, 64};
    SDL_Rect nameTextRect = {59, 119, 492, 56};

    // CONFIRM BUTTON
    Button * confirmButton = new Button(windowAdd, rendererAdd, 350, 250, global::pathToFont, std::string(pathImages+"confirmButton.png").c_str());
    SDL_Rect confirmButtonRect = confirmButton->getButtonRect();

    // CANCEL BUTTON
    Button * cancelButton = new Button(windowAdd, rendererAdd, 90, 250, global::pathToFont, std::string(pathImages+"cancelButton.png").c_str());
    SDL_Rect cancelButtonRect = cancelButton->getButtonRect();

    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                confirmNewChat = false;
                running = false;
                break;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Verifique se o botão foi pressionado
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = {mouseX, mouseY};
                // Confirm Button
                if (SDL_PointInRect(&mousePoint, &confirmButtonRect)) {
                    if (inputText.size() != 0){
                        confirmNewChat = true;
                        running = false;
                    }
                // Cancel Button
                }else if (SDL_PointInRect(&mousePoint, &cancelButtonRect)) {
                    confirmNewChat = false;
                    running = false;
                }
                break;
            } else if (event.type == SDL_KEYDOWN) {
                //Handle backspace
                if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ) {
                    //lop off character
                    inputText.pop_back();
                    flagRenderText = true;
                }
                //Handle enter
                else if( event.key.keysym.sym == SDLK_RETURN) {
                    if(inputText.size() != 0) {
                        confirmNewChat = true;
                        running = false;
                        flagRenderText = true;
                    }
                }
                break;
            } else if (event.type == SDL_TEXTINPUT) {
                //Not copy or pasting
                if ( !( SDL_GetModState() & KMOD_CTRL && ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' || event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) ) ) {
                    if (inputText.size() < MAX_CHARACTER){
                        //Append character
                        inputText += event.text.text;
                        flagRenderText = true;
                    }
                }
                break;
            }
        }

        // NUM MAX CHARACTER
        std::string numCharacterText = "Number of characters: ";
        numCharacterText += std::to_string(inputText.size());
        numCharacterText += "/15";
        nameUser = inputText;
        
        if(flagRenderText){
            SDL_RenderClear(rendererAdd);

            // BACKGROUND
            SDL_SetRenderDrawColor(rendererAdd, 200, 200, 200, 255);
            SDL_RenderFillRect(rendererAdd, &backgroundRect);

            // TO WHO
            SDL_RenderCopy(rendererAdd, alterarTexture, NULL, &cabecalhoRect);

            // CONFIRM BUTTON
            confirmButton->draw();

            // CANCEL BUTTON
            cancelButton->draw();

            // NAME TEXT
            SDL_SetRenderDrawColor(rendererAdd, 0, 0, 0, 255);
            SDL_RenderFillRect(rendererAdd, &edgeRect);
            SDL_SetRenderDrawColor(rendererAdd, 128, 128, 128, 255);
            SDL_RenderFillRect(rendererAdd, &nameTextRect);

            // NUM MAX CHARACTER
            renderText(numCharacterText, 300, 180, gFontSmall, WIDTH_LIMIT, rendererAdd);

            // INPUT TEXT
            renderText(inputText, 65, 140, gFontNormal, WIDTH_LIMIT, rendererAdd);
            flagRenderText = false;

            SDL_RenderPresent(rendererAdd);
        }    
    }

    // Libere os recursos utilizados pela biblioteca SDL2
    SDL_DestroyRenderer(rendererAdd);
    SDL_DestroyWindow(windowAdd);
    SDL_Quit();

    return MAIN_MENU;

}

bool NewReceiver::getConfirmNewChat(){
    return confirmNewChat;
}
