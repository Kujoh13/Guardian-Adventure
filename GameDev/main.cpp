
#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"
#include "Screen.h"

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
static std::vector<GameObject> object;

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
game_map MAP;
bool load(int level)
{
    bool ret = MAP.loadMap("img/", gRenderer, level);
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

void startScreen(){

}

int main(int argc, char* argv[]){


    initSDL(gWindow, gRenderer);

    Screen scr;
    scr.loadTexture(gRenderer);

    int isRunning = 1;

    load(1);

    while(isRunning)
    {

        while(SDL_PollEvent(&gEvent))
        {
            if(gEvent.type == SDL_QUIT)
            {
                isRunning = 0;
            }
        }

        int lastTime = 0;

        int currentTime = SDL_GetTicks();

        if(currentTime - lastTime >= 1){

            SDL_RenderClear(gRenderer);

            if(isRunning == 1){
                scr.startScreen(gRenderer);
            }

            //MAP.render(gRenderer);

            SDL_RenderPresent(gRenderer);

            lastTime = currentTime;
        }

    }
    close();

    return 0;
}
