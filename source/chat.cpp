#include "chat.hpp"

Chat::Chat(std::string fontPath, std::string _whoImTalkingTo){
    SCREEN_WIDTH = global::INIT_SCREEN_WIDTH;
    SCREEN_HEIGHT = global::INIT_SCREEN_HEIGHT;
    TEXT_BOX_HEIGHT = global::INIT_TEXT_BOX_HEIGHT;
    MAX_MSG_WIDTH = SCREEN_WIDTH * 0.8 - 2 * global::SIDE_SPACCING;
    Window = nullptr;
    renderer = nullptr;
    fontNormal = nullptr;
    fontSmall = nullptr;
    fontBig = nullptr;
    textColor = { 0, 0, 0 };
    whoImTalkingTo = _whoImTalkingTo;

    // initialization SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        logSDLError("SDL_Init");

    if (TTF_Init() != 0)
        logSDLError("TTF_Init");

    Window = SDL_CreateWindow("Chat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (Window == nullptr)
        logSDLError("SDL_CreateWindow (main)");

    renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) 
        logSDLError("SDL_CreateRenderer (main)");

    fontNormal = TTF_OpenFont(fontPath.c_str(), global::FONT_SIZE);
    if (fontNormal == nullptr)
        logSDLError("TTF_OpenFont");

    fontSmall = TTF_OpenFont(fontPath.c_str(), (int)global::FONT_SIZE/2);
    if (fontSmall == nullptr)
        logSDLError("TTF_OpenFont");

    fontBig = TTF_OpenFont(fontPath.c_str(), (int)global::FONT_SIZE*1.5);
    if (fontBig == nullptr)
        logSDLError("TTF_OpenFont");

    msgContainer = new MsgContainer(&getHeightText, fontNormal, fontSmall, renderer, MAX_MSG_WIDTH);
}

Chat::~Chat(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(Window);
    TTF_Quit();
    SDL_Quit();
    delete msgContainer;
}

void Chat::addMessage(Message msg){
    msgContainer->append(msg);
}

void Chat::renderMessages(int y) {
    int msgHeight = 0;
    int maxMsgHeight = global::FONT_SIZE;
    int widthText = 0;
    int heightText = 0;
    int widthTime = 0;
    int heightTime = 0;
    int widthTotal = 0;
    int heightTotal = 0;

    for (auto it = msgContainer->getReadBeginIterator(); it != msgContainer->getReadEndIterator(); it++){
        // sufficently inside screen to be plotted
        if(y > - maxMsgHeight - global::V_SPACING && y < SCREEN_HEIGHT - TEXT_BOX_HEIGHT){
            getSizeText(it->text, MAX_MSG_WIDTH, fontNormal, renderer, heightText, widthText);
            getSizeText(it->hour+":"+it->minute, MAX_MSG_WIDTH, fontSmall, renderer, heightTime, widthTime);
            widthTotal = std::max(widthText, widthTime);
            heightTotal = heightText + heightTime;
            // render to the left, not sent by me
            if(it->sender != "Me"){
                // render rectangle which contains message
                SDL_Rect rect = {global::SIDE_SPACCING/2, y, widthTotal+global::SIDE_SPACCING, heightTotal};
                SDL_SetRenderDrawColor( renderer, 224, 224, 224, 255 );
                SDL_RenderFillRect( renderer, &rect );

                // render normal size
                msgHeight = renderText(it->text, global::SIDE_SPACCING, y, fontNormal, MAX_MSG_WIDTH, renderer);
                y += msgHeight;
                if (msgHeight > maxMsgHeight)
                    maxMsgHeight = msgHeight;

                // render small
                msgHeight = renderText(it->hour+":"+it->minute, global::SIDE_SPACCING, y, fontSmall, MAX_MSG_WIDTH, renderer);
                y += msgHeight + global::V_SPACING;

            // render to the right, sent by me
            }else{
                // render rectangle which contains message
                SDL_Rect rect = {(int)(SCREEN_WIDTH - global::SIDE_SPACCING*1.5 - widthTotal), y, widthTotal+global::SIDE_SPACCING, heightTotal};
                SDL_SetRenderDrawColor( renderer, 224, 224, 224, 255 );
                SDL_RenderFillRect( renderer, &rect );
                // render normal size
                msgHeight = renderText(it->text, SCREEN_WIDTH - global::SIDE_SPACCING - widthTotal, y, fontNormal, MAX_MSG_WIDTH, renderer);
                y += msgHeight;
                if (msgHeight > maxMsgHeight)
                    maxMsgHeight = msgHeight;
                // render small
                msgHeight = renderText(it->hour+":"+it->minute, SCREEN_WIDTH - global::SIDE_SPACCING*1.5 - global::FONT_SIZE, y, fontSmall, MAX_MSG_WIDTH, renderer);
                y += msgHeight + global::V_SPACING;
            }
        }else{
            y += getHeightText(it->text, MAX_MSG_WIDTH, fontNormal, renderer) + global::V_SPACING + getHeightText(it->hour+":"+it->minute, MAX_MSG_WIDTH, fontSmall, renderer);
        }
    } 
}

void Chat::scrollMessages(int& yOffset, int heightMsgs, int amount) {
    yOffset += amount * global::SCROLL_SPEED;
    // not enough messages to fill the screen
    if (heightMsgs < SCREEN_HEIGHT - (TEXT_BOX_HEIGHT + global::HEADER_HEIGHT + global::V_SPACING)){
        yOffset = global::HEADER_HEIGHT + global::V_SPACING;
    // enough messages to fill the screen
    } else {
        // first message always on the top of the screen or hidden (y < HEADER_HEIGHT)
        if (yOffset > global::HEADER_HEIGHT + global::V_SPACING)
            yOffset = global::HEADER_HEIGHT + global::V_SPACING;
        // cannot have empty pace on the bottom if there are sufficint messages 
        else if (yOffset < (SCREEN_HEIGHT - TEXT_BOX_HEIGHT - heightMsgs))
            yOffset = (SCREEN_HEIGHT - TEXT_BOX_HEIGHT - heightMsgs);
    }
}

int Chat::execute(){
    bool quit = false;
    SDL_Event event;
    int yOffset = 0;
    std::string inputText = "> ";

    bool flagRenderText = true;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            else if (event.type == SDL_MOUSEWHEEL) {
                scrollMessages(yOffset, msgContainer->getTotalHeight(), event.wheel.y);
                flagRenderText = true;
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_ENTER) {
                    DEBUG("cursor joined window ",(int)event.window.windowID);
                } else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
                    DEBUG("cursor left window ",(int)event.window.windowID);
                } else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SCREEN_WIDTH = event.window.data1;
                    SCREEN_HEIGHT = event.window.data2;
                    MAX_MSG_WIDTH = SCREEN_WIDTH * 0.8 - 2 * global::SIDE_SPACCING;
                    msgContainer->updateWidthMax(MAX_MSG_WIDTH);
                    scrollMessages(yOffset, msgContainer->getTotalHeight());
                    DEBUG("Resizing window");
                    flagRenderText = true;
                }
            }
            //Special key input
            else if( event.type == SDL_KEYDOWN )
            {
                //Handle backspace
                if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                {
                    //lop off character
                    inputText.pop_back();
                    flagRenderText = true;
                }
                //Handle copy
                else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                {
                    SDL_SetClipboardText( inputText.c_str() );
                }
                //Handle paste
                else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                {
                    inputText = SDL_GetClipboardText();
                    flagRenderText = true;
                }
                //Handle enter
                else if( event.key.keysym.sym == SDLK_RETURN)
                {
                    DEBUG("got enter");
                    if(inputText.size() > 2){
                        // erase "> " from string
                        inputText.erase(0,2);
                        // get current time
                        std::time_t now = std::time(nullptr);
                        std::tm* timeinfo = std::localtime(&now);
                        std::ostringstream hour;
                        hour << std::setw(2) << std::setfill('0') << timeinfo->tm_hour;
                        std::ostringstream min;
                        min << std::setw(2) << std::setfill('0') << timeinfo->tm_min;
                        // add message to list
                        int offset = msgContainer->append(Message(inputText,std::string("Me"),hour.str(),min.str())) + global::V_SPACING;
                        inputText.clear();
                        inputText += "> ";
                        // screen filled with messages
                        if (msgContainer->getTotalHeight() >= SCREEN_HEIGHT - TEXT_BOX_HEIGHT - global::HEADER_HEIGHT - global::V_SPACING)
                            yOffset -= offset;
                        flagRenderText = true;
                    }

                }
            }//Special text input event
            else if( event.type == SDL_TEXTINPUT )
            {
                DEBUG("Got some text");
                //Not copy or pasting
                if( !( SDL_GetModState() & KMOD_CTRL && ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' || event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) ) )
                {
                    //Append character
                    inputText += event.text.text;
                    flagRenderText = true;
                }
            }
        }

        if(flagRenderText){
            // update scroll
            scrollMessages(yOffset, msgContainer->getTotalHeight(),0);
            //update textbox size
            TEXT_BOX_HEIGHT = getHeightText(inputText, SCREEN_WIDTH - 2 * global::SIDE_SPACCING, fontNormal, renderer) + 2 * global::V_SPACING;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            // messages
            renderMessages(yOffset);

            // clear text before drawing input box
            SDL_Rect rect = {0,SCREEN_HEIGHT - TEXT_BOX_HEIGHT,SCREEN_WIDTH,TEXT_BOX_HEIGHT};
            SDL_SetRenderDrawColor( renderer, 192, 192, 192, 255 );
            SDL_RenderFillRect( renderer, &rect );

            // input text
            renderText(inputText, global::SIDE_SPACCING, SCREEN_HEIGHT - TEXT_BOX_HEIGHT, fontNormal, MAX_MSG_WIDTH, renderer);

            // clear text before drawing Contact name
            SDL_Rect rect2 = {0,0,SCREEN_WIDTH,global::HEADER_HEIGHT};
            SDL_SetRenderDrawColor( renderer, 192, 192, 192, 255 );
            SDL_RenderFillRect( renderer, &rect2 );

            renderText(whoImTalkingTo, global::SIDE_SPACCING, global::V_SPACING, fontBig, MAX_MSG_WIDTH, renderer);
            
            // show
            SDL_RenderPresent(renderer);
            flagRenderText = false;
        }
        SDL_Delay( 50 );
    }
    return 1;
}