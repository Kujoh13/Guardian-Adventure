#include "Handler.h"

Handler::Handler()
{
    isRunning = 2;
    current_character = 0;
    current_level = 2;
    for(int i = 0; i < numItem; i++)
        itemDrop[i] = NULL;

    for(int i = 0; i < numProjectile; i++)
        pr[i] = NULL;

    lpTexture = NULL;

    for(int i = 0; i < 3; i++)
        lp_Animation[i] = NULL;

    explosion = NULL;

    MAP = new game_map;
}

Handler::~Handler()
{
    while(vProjectile.size()) vProjectile.pop_back();
    while(vMob.size()) vMob.pop_back();

    for(int i = 0; i < numItem; i++)
        SDL_DestroyTexture(itemDrop[i]);

    for(int i = 0; i < numProjectile; i++)
        SDL_DestroyTexture(pr[i]);

    SDL_DestroyTexture(lpTexture);

    for(int i = 0; i < 3; i++)
        SDL_DestroyTexture(lp_Animation[i]);

    SDL_DestroyTexture(explosion);
}

void Handler::tick(SDL_Renderer* renderer)
{
    bool idle = true;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            isRunning = 0;
        }
        if(isRunning == 1){
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                int x, y;
                SDL_GetMouseState(&x, &y);
                scr.checkExit(x, y, isRunning, event);
                scr.checkStart(x, y, isRunning, event);
            }
        }
        else if(isRunning == 2){
            loadLevel(current_level, renderer);
            isRunning = 3;
        }
        else if(isRunning == 3){
            if(_character[current_character].handleInput(event))
                idle = false;
        }

    }

    if(idle)
        _character[current_character].setStatus(0);

    if(!_character[current_character].getAttack())
        _character[current_character].setStatus(3);

    SDL_RenderClear(renderer);

    if(isRunning == 1){
        scr.startScreen(renderer);
    }
    else if(isRunning == 3){

        for(int i = 0; i < vMob.size(); i++)
            rectMob.push_back({vMob[i].getRect(), vMob[i].getHp()});

        _character[current_character].tick(MAP, rectMob, vProjectile);

        if(vItem.size())
        for(int i = vItem.size() - 1; i >= 0; i--)
        {
            vItem[i].tick(_character[current_character].getRect(), MAP);
            if(collision(vItem[i].getRect(), _character[current_character].getRect()) && vItem[i].getFell())
            {
                if(vItem[i].getId() == 3)
                    vItem[i].dropItem(vItem_temp, Rand(1, MAP->numCoin), Rand(1, MAP->numGem));
                else if(vItem[i].getId() == 2)
                {
                    int nHp = std::min(_character[current_character].getMaxHp(), _character[current_character].getHp() + vItem[i].getVal());
                    _character[current_character].setHp(nHp);
                }
                std::swap(vItem[i], vItem.back());
                vItem.pop_back();
            }
        }

        while(vItem_temp.size())
        {
            vItem.push_back(vItem_temp.back());
            vItem_temp.pop_back();
        }

        if(MAP->get_lp_pos().size() && MAP->get_lp_pos().back() <= _character[current_character].getX())
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
               std::swap(rectMob[i], rectMob.back());
               std::swap(vMob[i], vMob.back());
               rectMob.pop_back();
               vMob.pop_back();
            }
            else
                vMob[i].tick(MAP, vProjectile, &_character[current_character]);
        }
        if(vProjectile.size())
        for(int i = vProjectile.size() - 1; i >= 0 ; i--)
        {
            vProjectile[i].tick(MAP);
            if(vProjectile[i].done())
            {
                if(distance(vProjectile[i].getHitBox(), _character[current_character].getRect()) <= vProjectile[i].getRadius())
                {
                    int curHp = _character[current_character].getHp() - vProjectile[i].getDmg();
                    _character[current_character].setHp(curHp);
                }
                vExplosion.push_back({vProjectile[i].getHitBox(), {vProjectile[i].getRadius(), 15}});
                std::swap(vProjectile[i], vProjectile.back());
                vProjectile.pop_back();
            }
            else if(vProjectile[i].getHostile())
            {
                if(collision(vProjectile[i].getHitBox(), _character[current_character].getRect()))
                {
                    int curHp = _character[current_character].getHp() - vProjectile[i].getDmg();
                    _character[current_character].setHp(curHp);
                    if(vProjectile[i].getThrew())
                    {
                        vExplosion.push_back({vProjectile[i].getHitBox(), {vProjectile[i].getRadius(), 15}});
                    }
                    std::swap(vProjectile[i], vProjectile.back());
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
                        std::swap(vProjectile[i], vProjectile.back());
                        vProjectile.pop_back();
                        break;
                    }
                }
            }
        }

        if(v_lp.size()){
            for(int i = v_lp.size() - 1; i >= 0; i--)
            {
                v_lp[i].tick(_character[current_character].getRect(), MAP, vItem);
                if(v_lp[i].get_done())
                {
                    std::swap(v_lp[i], v_lp.back());
                    v_lp.pop_back();
                }
            }
        }
        rectMob.clear();
    }

}

void Handler::show(SDL_Renderer* renderer)
{
    if(_character[current_character].getX() >= SCREEN_WIDTH / 2)
        view = _character[current_character].getX() - (SCREEN_WIDTH / 2);
    else view = 0;

    MAP->render(renderer, view);

    for(int i = 0; i < vItem.size(); i++){
        SDL_Rect nRect = vItem[i].getRect();
        nRect.x -= view;
        SDL_RenderCopy(renderer, itemDrop[vItem[i].getId()], NULL, &nRect);
    }

    for(int i = 0; i < vMob.size(); i++){
        vMob[i].show(renderer, view);

        /// h / mh = x / 40 x = h * 40

        SDL_Rect tRect = {vMob[i].getX() - view, vMob[i].getY() - 5, 42, 7};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderFillRect(renderer, &tRect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        int w = vMob[i].getHp() * 40 / vMob[i].getMaxHp();

        tRect = {vMob[i].getX() + 1 - view, vMob[i].getY() - 4, w, 5};

        SDL_RenderFillRect(renderer, &tRect);

    }

    scr.ingame(renderer, _character[current_character].getHp(), _character[current_character].getDmg());

    _character[current_character].show(renderer, view);
    if(_character[current_character].getStatus() != 5){

        SDL_Rect rect = {_character[current_character].getX() - view, _character[current_character].getY() - 5, 42, 7};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderFillRect(renderer, &rect);

        int w = _character[current_character].getHp() * 40 / _character[current_character].getMaxHp();

        rect = {_character[current_character].getX() + 1 - view, _character[current_character].getY() - 4, w, 5};

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_RenderFillRect(renderer, &rect);

    }

    for(int i = 0; i < vProjectile.size(); i++){

        vProjectile[i].setW(pr_w[vProjectile[i].getId()]);
        vProjectile[i].setH(pr_h[vProjectile[i].getId()]);

        SDL_Rect tRect = vProjectile[i].getRect();
        tRect.x -= view;

        SDL_RenderDrawRect(renderer, &tRect);

        SDL_RenderCopyEx(renderer, pr[vProjectile[i].getId()], NULL, &tRect, vProjectile[i].getAngle(), NULL, SDL_FLIP_NONE);
    }

    for(int i = 0; i < v_lp.size(); i++)
    {
        if(v_lp[i].getType() == 1)
            v_lp[i].show(renderer, view, lp_Animation[v_lp[i].getStatus()]);
        else
            v_lp[i].show(renderer, view, lpTexture);
    }
    if(vExplosion.size())
    for(int i = vExplosion.size() - 1; i >= 0; i--){
        SDL_Rect tRect = vExplosion[i].first;
        int r = vExplosion[i].second.first;
        tRect.x = (2 * vExplosion[i].first.x + vExplosion[i].first.w) / 2 - sqrt(2) / 2 * r - view;
        tRect.y = (2 * vExplosion[i].first.y + vExplosion[i].first.h) / 2 - sqrt(2) / 2 * r;
        tRect.w = sqrt(2) * r;
        tRect.h = sqrt(2) * r;
        SDL_RenderCopy(renderer, explosion, NULL, &tRect);
        vExplosion[i].second.second--;
        if(vExplosion[i].second.second == 0)
        {
            std::swap(vExplosion[i], vExplosion.back());
            vExplosion.pop_back();
        }
    }

    SDL_RenderPresent(renderer);

}

bool Handler::loadLevel(int level, SDL_Renderer* renderer)
{
    if(!MAP->loadMap("img", renderer, level))
        return false;

    std::string map_path = "img/Level_" + int2str(level) + "/";
    std::ifstream fileMob((map_path + "level_info.txt").c_str());
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
                int x, y;
                fileMob >> x >> y;
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

    int nCoin, nGem;

    fileMob >> nCoin;
    fileMob >> nGem;
    MAP->setNumCoin(nCoin);
    MAP->setNumGem(nGem);

    fileMob.close();

    for(int i = 0; i < vMob.size(); i++){
        if(!vMob[i].loadMob(map_path + int2str(vMob[i].getId()), renderer))
        {
            return false;
        }
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

void Handler::load(SDL_Renderer* renderer)
{
    scr.loadTexture(renderer);

    //////////////

    for(int i = 0; i < numCharacter; i++)
        if(!_character[i].loadCharacter("Character/" + int2str(i), renderer, i))
        {
            std::cout << "Can't load character " << i << "!!!";
            exit(-1);
        }

    /////////////

    for(int i = 0; i < numProjectile; i++)
    {
        std::string path = "img/Projectile/" + int2str(i) + ".png";

        SDL_Surface* sf = IMG_Load(path.c_str());
        if(sf == NULL)
        {
            std :: cout << "Missing projectile image!!!";
            exit(-1);
        }
        pr_w[i] = sf->w;
        pr_h[i] = sf->h;
        pr[i] = SDL_CreateTextureFromSurface(renderer, sf);

        SDL_FreeSurface(sf);
    }

    std::string path = "img/explosion.png";
    SDL_Surface* sf = IMG_Load(path.c_str());

    explosion = SDL_CreateTextureFromSurface(renderer, sf);

    SDL_FreeSurface(sf);

    //////////////

    for(int i = 0; i < numItem; i++){
        std::string path = "img/item" + int2str(i) + ".png";

        SDL_Surface* sf = IMG_Load(path.c_str());
        if(sf == NULL)
        {
            std :: cout << "Missing item image!!!";
            exit(-1);
        }
        itemDrop[i] = SDL_CreateTextureFromSurface(renderer, sf);
        SDL_FreeSurface(sf);
    }

    //////////////

    SDL_Surface* loadSurface = IMG_Load("img/lp.png");

    lpTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);

    for(int i = 0; i < 3; i++)
    {
        std::string path = "img/lp" + int2str(i) + ".png";
        loadSurface = IMG_Load(path.c_str());
        lp_Animation[i] = SDL_CreateTextureFromSurface(renderer, loadSurface);
    }

    SDL_FreeSurface(loadSurface);

    ///////////////
}
