
#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"
#include "Screen.h"
#include "Character.h"
#include "Mob.h"
#include "Projectile.h"

using namespace std;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

const string WINDOW_TITLE = "Game SDL";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

GameObject gBackground;
game_map* MAP = new game_map;


bool load(int level)
{
    bool ret = MAP->loadMap("img", gRenderer, level);
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

int view;
Character _character[numCharacter];

std::vector<Mob>* vMob;
std::vector<Projectile> vProjectile;

SDL_Texture* pr[1];

int main(int argc, char* argv[]){

    initSDL(gWindow, gRenderer);

    Screen scr;
    scr.loadTexture(gRenderer);

    int isRunning = 1;
    /** Notes:
    1. Start Screen
    2. Level Selection
    3. In a level
    **/
    int currentLevel = 1;
    int currentCharacter = 0;

    if(!load(1))
        return -1;

    Mob mob;

    mob.setWeapon(1);

    if(!mob.loadMob("img/Level_1/0", gRenderer))
        return -1;

    for(int i = 0; i < numCharacter; i++){
        if(!_character[i].loadCharacter("Character/" + int2str(i), gRenderer, i))
            return -1;
    }

    for(int i = 0; i < 1; i++){
        std::string path = "img/Projectile/" + int2str(i) + ".png";

        SDL_Surface* sf = IMG_Load(path.c_str());
        if(sf == NULL) return -1;

        pr[i] = SDL_CreateTextureFromSurface(gRenderer, sf);

        SDL_FreeSurface(sf);
    }

    int lastTime = 0, currentTime = 0;


    while(isRunning)
    {

        currentTime = SDL_GetTicks();

        if(currentTime - lastTime >= 33){

            bool idle = true;
            while(SDL_PollEvent(&gEvent))
            {

                if(gEvent.type == SDL_QUIT)
                {
                    isRunning = 0;
                }
                if(isRunning == 1){
                    if(gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT){
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        scr.checkExit(x, y, isRunning, gEvent);
                        scr.checkStart(x, y, isRunning, gEvent);
                    }
                }
                else if(isRunning == 3){
                    _character[currentCharacter].handleInput(gEvent);
                    idle = false;

                }

            }

            if(idle)
                _character[currentCharacter].setStatus(0);
            if(!_character[currentCharacter].getAttack())
                _character[currentCharacter].setStatus(3);

            SDL_RenderClear(gRenderer);

            if(isRunning == 1){
                scr.startScreen(gRenderer);
            }
            else if(isRunning == 3){
                //scr.levelSelection(gRenderer);
                MAP->setNumBlock(4);

                _character[currentCharacter].tick(MAP);
                mob.tick(MAP, vProjectile, _character[currentCharacter].getRect());

                if(vProjectile.size())
                for(int i = vProjectile.size() - 1; i >= 0 ; i--){
                    vProjectile[i].tick();
                    if(vProjectile[i].done())
                    {
                        swap(vProjectile[i], vProjectile.back());
                        vProjectile.pop_back();
                    }
                }

                if(_character[currentCharacter].getX() >= SCREEN_WIDTH / 2)
                    view = _character[currentCharacter].getX() - (SCREEN_WIDTH / 2);
                else view = 0;



                MAP->render(gRenderer, view);

                mob.show(gRenderer, view);
                _character[currentCharacter].show(gRenderer, view);
                for(int i = 0; i < vProjectile.size(); i++){

                    SDL_Rect tRect = vProjectile[i].getRect();
                    tRect.x -= view;

                    SDL_Rect nRect = vProjectile[i].getHitBox(view);

                    SDL_RenderDrawRect(gRenderer, &nRect);

                    SDL_RenderCopyEx(gRenderer, pr[vProjectile[i].getId()], NULL, &tRect, vProjectile[i].getAngle(), NULL, SDL_FLIP_NONE);
                }
            }

            SDL_RenderPresent(gRenderer);

            lastTime = currentTime;
        }

    }
    close();

    return 0;
}

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

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
