
#include "BasicFunction.h"
#include "GameObject.h"

using namespace std;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

const string WINDOW_TITLE = "Game SDL";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

GameObject gBackground;
bool loadBackground()
{
    bool ret = gBackground.loadImage("img/flag.png", gRenderer);
    return ret;
}

void close()
{
    gBackground.Free();

    SDL_DestroyRenderer (gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]){


    initSDL(gWindow, gRenderer);

    if(loadBackground() == false)
        return -1;

    bool isRunning = true;

    while(isRunning)
    {
        while(SDL_PollEvent(&gEvent))
        {
            if(gEvent.type == SDL_QUIT)
            {
                isRunning = 0;
            }
        }

        SDL_RenderClear(gRenderer);

        gBackground.render(gRenderer, NULL);

        SDL_RenderPresent(gRenderer);
    }
    close();

    return 0;
}
