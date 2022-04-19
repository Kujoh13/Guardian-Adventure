
#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"
#include "Screen.h"
#include "Character.h"
#include "Mob.h"
#include "Projectile.h"
#include "lp.h"

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
    TTF_Quit();
}

int view;
Character _character[numCharacter];

std::vector<Projectile> vProjectile;
std::vector<Mob> vMob;
std::vector<Item> vItem, vItem_temp;
std::vector<lp> v_lp;
SDL_Texture* pr[numProjectile];
SDL_Texture* itemDrop[numItem];
SDL_Texture* lpTexture;
SDL_Texture* lp_Animation[3];
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

    fileMob >> MAP->numCoin;
    fileMob >> MAP->numGem;

    fileMob.close();

    for(int i = 0; i < vMob.size(); i++){
        if(!vMob[i].loadMob(map_path + int2str(vMob[i].getId()), gRenderer))
            return 0;
    }
    lp new_lp;
    new_lp.setType(1);
    new_lp.setW(70);
    new_lp.setH(70);
    new_lp.setX(MAP->get_lp_pos().back());
    MAP->pop();
    new_lp.loadVar({4, 14}, {4, 11}, {4, 17});
    v_lp.push_back(new_lp);
    return 1;
}

int main(int argc, char* argv[]){

    initSDL(gWindow, gRenderer);
    TTF_Init();

    Screen scr;
    scr.loadTexture(gRenderer);

    int isRunning = 3;
    /** Notes:
    1. Start Screen
    2. Level Selection
    3. In a level
    **/
    int currentLevel = 1;
    int currentCharacter = 1;

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
        pr_w[i] = sf->w;
        pr_h[i] = sf->h;
        pr[i] = SDL_CreateTextureFromSurface(gRenderer, sf);

        SDL_FreeSurface(sf);
    }

    //////////////

    for(int i = 0; i < numItem; i++){
        std::string path = "img/item" + int2str(i) + ".png";

        SDL_Surface* sf = IMG_Load(path.c_str());
        if(sf == NULL)
        {
            std :: cout << "Missing item image!!!";
            return -1;
        }
        itemDrop[i] = SDL_CreateTextureFromSurface(gRenderer, sf);
        SDL_FreeSurface(sf);
    }

    //////////////

    SDL_Surface* loadSurface = IMG_Load("img/lp.png");

    lpTexture = SDL_CreateTextureFromSurface(gRenderer, loadSurface);

    for(int i = 0; i < 3; i++)
    {
        std::string path = "img/lp" + int2str(i) + ".png";
        loadSurface = IMG_Load(path.c_str());
        lp_Animation[i] = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
    }

    SDL_FreeSurface(loadSurface);

    ///////////////

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

                for(int i = 0; i < vMob.size(); i++)
                {
                    rectMob.push_back({vMob[i].getRect(), vMob[i].getHp()});
                }

                _character[currentCharacter].tick(MAP, rectMob, vProjectile);

                if(vItem.size())
                for(int i = vItem.size() - 1; i >= 0; i--)
                {
                    vItem[i].tick(_character[currentCharacter].getRect(), MAP);
                    if(collision(vItem[i].getRect(), _character[currentCharacter].getRect()) && vItem[i].getFell())
                    {
                        if(vItem[i].getId() == 3)
                            vItem[i].dropItem(vItem_temp, Rand(1, MAP->numCoin), Rand(1, MAP->numGem));
                        swap(vItem[i], vItem.back());
                        vItem.pop_back();
                    }
                }

                while(vItem_temp.size())
                {
                    vItem.push_back(vItem_temp.back());
                    vItem_temp.pop_back();
                }

                if(MAP->get_lp_pos().size() && MAP->get_lp_pos().back() <= _character[currentCharacter].getX())
                {
                    lp new_lp;
                    new_lp.setX(SCREEN_WIDTH);
                    new_lp.setVelX(-5);
                    new_lp.setType(0);
                    MAP->pop();
                    v_lp.push_back(new_lp);
                }

                if(vMob.size())
                for(int i = vMob.size() - 1; i >= 0; i--)
                {
                    vMob[i].setHp(rectMob[i].second);
                    if(rectMob[i].second == 0)
                    {
                       vMob[i].spawnItem(vItem);
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
                for(int i = vProjectile.size() - 1; i >= 0 ; i--)
                {
                    vProjectile[i].tick();
                    if(vProjectile[i].done())
                    {
                        swap(vProjectile[i], vProjectile.back());
                        vProjectile.pop_back();
                    }
                    else if(vProjectile[i].getHostile())
                    {
                        if(collision(vProjectile[i].getHitBox(), _character[currentCharacter].getRect()))
                        {
                            int curHp = _character[currentCharacter].getHp() - vProjectile[i].getDmg();
                            _character[currentCharacter].setHp(curHp);
                            swap(vProjectile[i], vProjectile.back());
                            vProjectile.pop_back();
                        }
                    }
                    else if(!vProjectile[i].getHostile())
                    {
                        if(vMob.size())
                        for(int j = vMob.size() - 1; j >= 0; j--)
                        {
                            if(collision(rectMob[j].first, vProjectile[i].getHitBox()))
                            {
                                rectMob[j].second = std::max(0, rectMob[j].second - vProjectile[i].getDmg());
                                vMob[j].setHp(rectMob[j].second);
                                swap(vProjectile[i], vProjectile.back());
                                vProjectile.pop_back();
                                break;
                            }
                        }
                    }
                }

                if(v_lp.size()){
                    for(int i = v_lp.size() - 1; i >= 0; i--)
                    {
                        v_lp[i].tick(_character[currentCharacter].getRect(), MAP, vItem);
                        if(v_lp[i].get_done())
                        {
                            std::swap(v_lp[i], v_lp.back());
                            v_lp.pop_back();
                        }
                    }
                }

                MAP->render(gRenderer, view);

                for(int i = 0; i < vItem.size(); i++){
                    SDL_Rect nRect = vItem[i].getRect();
                    nRect.x -= view;
                    SDL_RenderCopy(gRenderer, itemDrop[vItem[i].getId()], NULL, &nRect);
                }

                for(int i = 0; i < vMob.size(); i++){
                    vMob[i].show(gRenderer, view);

                    /// h / mh = x / 40 x = h * 40

                    SDL_Rect tRect = {vMob[i].getX() - view, vMob[i].getY() - 5, 42, 7};

                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

                    SDL_RenderFillRect(gRenderer, &tRect);

                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);

                    int w = vMob[i].getHp() * 40 / vMob[i].getMaxHp();

                    tRect = {vMob[i].getX() + 1 - view, vMob[i].getY() - 4, w, 5};

                    SDL_RenderFillRect(gRenderer, &tRect);

                }

                scr.ingame(gRenderer, _character[currentCharacter].getHp(), _character[currentCharacter].getDmg());

                _character[currentCharacter].show(gRenderer, view);
                if(_character[currentCharacter].getStatus() != 5){

                    SDL_Rect rect = {_character[currentCharacter].getX() - view, _character[currentCharacter].getY() - 5, 42, 7};

                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

                    SDL_RenderFillRect(gRenderer, &rect);

                    int w = _character[currentCharacter].getHp() * 40 / _character[currentCharacter].getMaxHp();

                    rect = {_character[currentCharacter].getX() + 1 - view, _character[currentCharacter].getY() - 4, w, 5};

                    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);

                    SDL_RenderFillRect(gRenderer, &rect);

                }

                for(int i = 0; i < vProjectile.size(); i++){

                    vProjectile[i].setW(pr_w[vProjectile[i].getId()]);
                    vProjectile[i].setH(pr_h[vProjectile[i].getId()]);

                    SDL_Rect tRect = vProjectile[i].getRect();
                    tRect.x -= view;

                    SDL_RenderCopyEx(gRenderer, pr[vProjectile[i].getId()], NULL, &tRect, vProjectile[i].getAngle(), NULL, SDL_FLIP_NONE);
                }

                for(int i = 0; i < v_lp.size(); i++)
                {
                    if(v_lp[i].getType() == 1)
                        v_lp[i].show(gRenderer, view, lp_Animation[v_lp[i].getStatus()]);
                    else
                        v_lp[i].show(gRenderer, view, lpTexture);
                }

                //SDL_RenderCopy(gRenderer, itemDrop[3], NULL, NULL);

            }

            SDL_RenderPresent(gRenderer);

            lastTime = currentTime;
        }

    }

    for(int i = 0; i < numProjectile; i++)
        SDL_DestroyTexture(pr[i]);

    SDL_DestroyTexture(lpTexture);

    for(int i = 0; i < 3; i++)
        SDL_DestroyTexture(lp_Animation[i]);

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
