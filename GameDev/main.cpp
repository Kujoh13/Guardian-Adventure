
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

void close()
{
    SDL_DestroyRenderer (gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int view;
Character _character[numCharacter];

std::vector<Projectile> vProjectile;
std::vector<Mob> vMob;
SDL_Texture* pr[numProjectile];
int pr_w[numProjectile];
int pr_h[numProjectile];
game_map* MAP = new game_map;

bool loadLevel(int level)
{
    bool ret = MAP->loadMap("img", gRenderer, level);
    if(!ret)
        return false;

    std::string map_path = "img/Level_" + int2str(level) + "/";
    ifstream fileMob((map_path + "level_info.txt").c_str());
    {
        if(fileMob.is_open() == false){
            std :: cout << "Missing mob file!!!";
        }
        int numMob = 0, typeMob = 0;
        fileMob >> typeMob;
        for(int i = 0; i < typeMob; i++)
        {
            fileMob >> numMob;
            for(int j = 0; j < numMob; j++)
            {
                Mob mob;
                int x, y, w;
                fileMob >> x >> y >> w;
                mob.setWeapon(w);
                mob.setX(x);
                mob.setY(y);
                mob.setId(i);
                int l, r;
                fileMob >> l >> r;
                mob.setRange(l, r);

                vMob.push_back(mob);

            }
        }
    }


    fileMob.close();

    for(int i = 0; i < vMob.size(); i++){
        if(!vMob[i].loadMob(map_path + int2str(vMob[i].getId()), gRenderer))
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[]){

    initSDL(gWindow, gRenderer);

    Screen scr;
    scr.loadTexture(gRenderer);

    int isRunning = 3;
    /** Notes:
    1. Start Screen
    2. Level Selection
    3. In a level
    **/
    int currentLevel = 1;
    int currentCharacter = 0;

    if(!loadLevel(1))
        return -1;

    //////////////

    for(int i = 0; i < numCharacter; i++)
        if(!_character[i].loadCharacter("Character/" + int2str(i), gRenderer, i))
        {
            std::cout << "Can't load character " << i << "!!!";
            return -1;
        }

    /////////////

    for(int i = 0; i < numProjectile; i++)
    {
        std::string path = "img/Projectile/" + int2str(i) + ".png";

        SDL_Surface* sf = IMG_Load(path.c_str());
        if(sf == NULL)
        {
            std :: cout << "Missing projectile image!!!";
            return -1;
        }

        pr[i] = SDL_CreateTextureFromSurface(gRenderer, sf);

        SDL_FreeSurface(sf);
    }
    std::string path = "img/Projectile/info.txt";

    ifstream file(path);

    for(int i = 0; i < numProjectile; i++){
        file >> pr_w[i] >> pr_h[i];
    }
    file.close();

    //////////////

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
                scr.levelSelection(gRenderer);
                MAP->setNumBlock(4);

                vector<std::pair<SDL_Rect, int> > rectMob;

                for(int i = 0; i < vMob.size(); i++){
                    rectMob.push_back({vMob[i].getRect(), vMob[i].getHp()});
                }

                _character[currentCharacter].tick(MAP, rectMob);

                if(vMob.size())
                for(int i = vMob.size() - 1; i >= 0; i--){
                    vMob[i].setHp(rectMob[i].second);
                    if(rectMob[i].second == 0){
                       swap(rectMob[i], rectMob.back());
                       swap(vMob[i], vMob.back());
                       rectMob.pop_back();
                       vMob.pop_back();
                    }
                    else
                        vMob[i].tick(MAP, vProjectile, &_character[currentCharacter]);
                }

                rectMob.clear();

                if(_character[currentCharacter].getX() >= SCREEN_WIDTH / 2)
                    view = _character[currentCharacter].getX() - (SCREEN_WIDTH / 2);
                else view = 0;

                if(vProjectile.size())
                for(int i = vProjectile.size() - 1; i >= 0 ; i--){
                    vProjectile[i].tick();
                    if(vProjectile[i].done() || vProjectile[i].collide(&_character[currentCharacter]))
                    {
                        swap(vProjectile[i], vProjectile.back());
                        vProjectile.pop_back();
                    }
                }

                MAP->render(gRenderer, view);

                for(int i = 0; i < vMob.size(); i++){
                    vMob[i].show(gRenderer, view);
                }

                //mob.show(gRenderer, view);


                _character[currentCharacter].show(gRenderer, view);
                for(int i = 0; i < vProjectile.size(); i++){

                    vProjectile[i].setW(pr_w[vProjectile[i].getId()]);
                    vProjectile[i].setH(pr_h[vProjectile[i].getId()]);

                    SDL_Rect tRect = vProjectile[i].getRect();
                    tRect.x -= view;

                    SDL_Rect nRect = vProjectile[i].getHitBox();
                    nRect.x -= view;
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(gRenderer, &nRect);

                    SDL_RenderCopyEx(gRenderer, pr[vProjectile[i].getId()], NULL, &tRect, vProjectile[i].getAngle(), NULL, SDL_FLIP_NONE);
                }
            }

            SDL_RenderPresent(gRenderer);

            lastTime = currentTime;
        }

    }

    for(int i = 0; i < numProjectile; i++)
        SDL_DestroyTexture(pr[i]);
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
