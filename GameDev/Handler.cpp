#include "Handler.h"

Handler::Handler()
{
    isRunning = prev = 1;
    current_character = 0;
    current_level = 1;
    for(int i = 0; i < numItem; i++)
        itemDrop[i] = NULL;

    for(int i = 0; i < numProjectile; i++)
        pr[i] = NULL;

    lpTexture = NULL;

    for(int i = 0; i < 3; i++)
        lp_Animation[i] = NULL;

    explosion = NULL;

    MAP = new game_map;

    numGem = 0, numCoin = 0;

    paused = false;
    frame_back = 0;
    frame_char3 = 1000;

    id = 0;

    prev_char_level = NULL;

    char_growth = NULL;

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

    delete(char_growth);

}

void Handler::tick(SDL_Renderer* renderer)
{
    prev_map = current_level;
    prev_char = current_character;
    for(int i = 0; i < numCharacter; i++)
        prev_char_level[i] = character_level[i];
    prev = isRunning;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            isRunning = 0;
            std::ofstream player_info("player_info.txt");
            player_info << numCoin << ' ' << numGem << '\n';
            player_info << lastLevel << '\n';
            for(int i = 0; i < numCharacter; i++)
                player_info << character_level[i] << ' ' << char_growth[i][0] << ' ' << char_growth[i][1] << '\n';

            player_info.close();
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){

            int x, y;
            SDL_GetMouseState(&x, &y);
            scr.handleMouseInput(x, y, isRunning, current_level, current_character, character_level, numCoin, numGem, lastLevel, paused);

            bool click = true;

            if(current_level != prev_map)
            {
                Audio_Player.changeMap();
                click = false;
            }

            if(prev_char != current_character)
            {
                Audio_Player.selectCharacter();
                click = false;
            }

            if(prev_char_level[current_character] != character_level[current_character])
            {
                if(prev_char_level[current_character]) Audio_Player.upgradeCharacter();
                else Audio_Player.unlockCharacter();
                click = false;
            }

            if(prev == 2 && isRunning == 3)
            {
                Audio_Player.playButton();
                click = false;
                _character[current_character].setX(0);
                _character[current_character].setY(0);
                vMob.clear();
                rectMob.clear();
                vItem.clear();
                vItem_temp.clear();
                vProjectile.clear();
                vExplosion.clear();
                v_lp.clear();
                _character[current_character].setLevel(character_level[current_character]);

                int newDmg = _character[current_character].getBaseDmg() ;
                if(character_level[current_character] > 0)
                    newDmg += (character_level[current_character] - 1) * char_growth[current_character][0];
                _character[current_character].setDmg(newDmg);

                int newHp = _character[current_character].getBaseHp();
                if(character_level[current_character] > 0)
                    newHp += (character_level[current_character] - 1) * char_growth[current_character][1];
                _character[current_character].setMaxHp(newHp);

                _character[current_character].setHp(_character[current_character].getMaxHp());
                _character[current_character].setStatus(0);
                _character[current_character].setFrame(0);
                frame_back = 0;
                delete(MAP);
                MAP = new game_map;
                boss.reset();
                loadLevel(current_level, renderer);
            }
            if(click) Audio_Player.mouse_click();
        }
        else if(event.type == SDL_KEYDOWN)
        {
            scr.handleKeyInput(event, paused, isRunning);
        }
        if(isRunning == 3){
            _character[current_character].handleInput(event);
        }

    }

    SDL_RenderClear(renderer);

    if(prev != isRunning) Audio_Player.stopAudio();

    if(isRunning == 1){
        scr.startScreen(renderer);
        Audio_Player.playBackgroundMusic(isRunning, current_level);
    }
    else if(isRunning == 2){
        int baseHp = _character[current_character].getBaseHp();
        int baseDmg = _character[current_character].getBaseDmg();
        scr.levelSelection(renderer, current_level, current_character, character_level, numCoin, numGem, lastLevel, baseHp, baseDmg, char_growth);
        Audio_Player.playBackgroundMusic(isRunning, current_level);
    }
    else if(isRunning == 3){
        if(paused) return;



        for(int i = 0; i < vMob.size(); i++)
            rectMob.push_back({vMob[i].getRect(), vMob[i].getHp()});

        frame_char3++;

        if(frame_char3 <= 3 * 30)
            _character[current_character].setVelX(MAX_RUN_SPEED + 10);
        else
            _character[current_character].setVelX(MAX_RUN_SPEED);

//        if(_character[current_character].jumped())
//            Audio_Player.character_jump();

        if(_character[current_character].getMove() && _character[current_character].getStatus() < 4 && _character[current_character].onGround())
        {
            if(current_character == 1 || current_character == 2)
                Audio_Player.character_move(1);
            else
                Audio_Player.character_move(0);
        }
        else Audio_Player.setMove();

        if(current_level == numLevel)
            boss.tick(&_character[current_character], vItem);

        _character[current_character].tick(MAP, rectMob, vProjectile);


        if(_character[current_character].startAttack() && _character[current_character].getStatus() == 3)
            Audio_Player.character_attack(current_character);

        _character[current_character].setObjectId(++id, 0);

        if(_character[current_character].getStatus() == 4 || _character[current_character].getStatus() == 5)
        {
            if(frame_back == 0)
            {
                Audio_Player.stopAudio();
                if(_character[current_character].getStatus() == 4)
                    Audio_Player.loseGame();
                else
                    Audio_Player.winGame();
            }
            frame_back++;
            if(frame_back == 5 * 30){
                isRunning = 2;
                if(_character[current_character].getStatus() == 5 && lastLevel == current_level)
                    lastLevel++;
                _character[current_character].setStatus(0);
                Audio_Player.stopAudio();
            }
        }
        else
        {
            Audio_Player.playBackgroundMusic(isRunning, current_level);
        }

        if(vItem.size())
        for(int i = vItem.size() - 1; i >= 0; i--)
        {
            vItem[i].tick(_character[current_character].getRect(), MAP);

            vItem[i].setObjectId(++id, 0);

            if(collision(vItem[i].getRect(), _character[current_character].getRect()) && vItem[i].getFell())
            {
                if(vItem[i].getId() == 3)
                    vItem[i].dropItem(vItem_temp, Rand(1, MAP->getNumCoin()), Rand(1, MAP->getNumGem()), _character[current_character].getMaxHp());
                else if(vItem[i].getId() == 2)
                {
                    if(_character[current_character].getStatus() < 4){
                        int nHp = std::min(_character[current_character].getMaxHp(), _character[current_character].getHp() + vItem[i].getVal());
                        _character[current_character].setHp(nHp);
                        Audio_Player.character_heal();
                    }
                }
                else if(vItem[i].getId() == 1)
                {
                    numGem += vItem[i].getVal();
                    Audio_Player.collectItem();
                }
                else
                {
                    numCoin += vItem[i].getVal();
                    Audio_Player.collectItem();
                }
                std::swap(vItem[i], vItem.back());
                vItem.pop_back();
            }
            else if(vItem[i].getY() + vItem[i].getH() >= SCREEN_HEIGHT - 1)
            {
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
            vMob[i].setObjectId(++id, 0);
            if(rectMob[i].second == 0)
            {
               Audio_Player.mobDie();
               vMob[i].spawnItem(vItem);
               std::swap(rectMob[i], rectMob.back());
               std::swap(vMob[i], vMob.back());
               rectMob.pop_back();
               vMob.pop_back();
            }
            else
                vMob[i].tick(MAP, vProjectile, &_character[current_character], vExplosion);
        }
        if(vProjectile.size())
        {
            if(current_character == 1)
                character1();
            else
            {
                for(int i = vProjectile.size() - 1; i >= 0; i--)
                {
                    vProjectile[i].tick(MAP);
                    vProjectile[i].setObjectId(++id, 0);
                    if(vProjectile[i].done())
                    {
                        if(vProjectile[i].getThrew())
                        {
                            vExplosion.push_back({vProjectile[i].getHitBox(), vProjectile[i].getRadius(), vProjectile[i].getDmg(), 15});
                            Audio_Player.bomb_explosion();
                        }
                        std::swap(vProjectile[i], vProjectile.back());
                        vProjectile.pop_back();
                    }
                    else if(vProjectile[i].getHostile())
                    {
                        if(collision(vProjectile[i].getHitBox(), _character[current_character].getRect()))
                        {
                            int chance = Rand(1, 2);
                            if(current_character != 2 || chance == 1)
                            {
                                int curHp = _character[current_character].getHp() - vProjectile[i].getDmg();
                                _character[current_character].setHp(curHp);
                            }
                            if(vProjectile[i].getThrew())
                            {
                                vExplosion.push_back({vProjectile[i].getHitBox(), vProjectile[i].getRadius(), vProjectile[i].getDmg(), 15});
                                Audio_Player.bomb_explosion();
                            }
                            std::swap(vProjectile[i], vProjectile.back());
                            vProjectile.pop_back();
                        }
                    }
                    else if(!vProjectile[i].getHostile())
                    {
                        if(current_level == numLevel)
                        {
                            if(collision(vProjectile[i].getHitBox(), boss.getBossHitbox()) && boss.vulnerable())
                            {
                                if(current_character == 3)
                                    frame_char3 = 0;
                                if(current_character == 4)
                                {
                                    int chance = Rand(1, 4);
                                    if(chance == 3)
                                    {
                                        int newHp = _character[current_character].getHp() + _character[current_character].getMaxHp() / 10;
                                        _character[current_character].setHp(newHp);
                                        Audio_Player.character_heal();
                                    }
                                }
                                boss.takeDamage(vProjectile[i].getDmg());
                                std::swap(vProjectile[i], vProjectile.back());
                                vProjectile.pop_back();
                            }
                        }
                        if(vMob.size())
                        for(int j = vMob.size() - 1; j >= 0; j--)
                        {

                            if(collision(rectMob[j].first, vProjectile[i].getRect()))
                            {
                                if(current_character == 3)
                                    frame_char3 = 0;
                                if(current_character == 4)
                                {
                                    int chance = Rand(1, 4);
                                    if(chance == 3)
                                    {
                                        int newHp = _character[current_character].getHp() + _character[current_character].getMaxHp() / 10;
                                        _character[current_character].setHp(newHp);
                                        Audio_Player.character_heal();
                                    }
                                }
                                rectMob[j].second = std::max(0, rectMob[j].second - vProjectile[i].getDmg());
                                vMob[j].setHp(rectMob[j].second);
                                std::swap(vProjectile[i], vProjectile.back());
                                vProjectile.pop_back();
                                break;
                            }
                        }
                    }
                }
            }
        }

        if(v_lp.size())
        {
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
        for(int i = 0; i < vExplosion.size(); i++)
        {
            if(distance(vExplosion[i].rect, _character[current_character].getRect()) <= vExplosion[i].radius)
            {
                int curHp = _character[current_character].getHp() - vExplosion[i].dmg;
                _character[current_character].setHp(curHp);
            }
            vExplosion[i].frame--;
        }
    }

}

void Handler::show(SDL_Renderer* renderer)
{
    if(isRunning == 3){

        if(_character[current_character].getX() >= SCREEN_WIDTH / 2)
            view = _character[current_character].getX() - (SCREEN_WIDTH / 2);
        else view = 0;
        if(current_level == numLevel) view = 0;

        MAP->render(renderer, view, vMob.empty(), current_character == numLevel);



        for(int i = 0; i < vItem.size(); i++){
            SDL_Rect nRect = vItem[i].getRect();
            nRect.x -= view;
            SDL_RenderCopy(renderer, itemDrop[vItem[i].getId()], NULL, &nRect);
        }

        for(int i = 0; i < vMob.size(); i++){
            vMob[i].show(renderer, view);

            /// h / mh = x / 40 x = h * 40

            SDL_Rect tRect = {vMob[i].getX() - view, vMob[i].getY() - 20, 42, 7};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderFillRect(renderer, &tRect);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

            int w = vMob[i].getHp() * 40 / vMob[i].getMaxHp();

            tRect = {vMob[i].getX() + 1 - view, vMob[i].getY() - 19, w, 5};

            SDL_RenderFillRect(renderer, &tRect);

            tRect.x--;
            tRect.y += 6;
            tRect.h = 7;
            tRect.w = 42;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &tRect);

            std::pair<int, int> temp = vMob[i].getAttackBar();
            temp.first = std::min(temp.first, temp.second);

            tRect.x++;
            tRect.y++;
            tRect.h = 5;
            tRect.w = temp.first * 40 / temp.second;

            SDL_SetRenderDrawColor(renderer, 107, 107, 107, 255);
            if(temp.first == temp.second)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &tRect);

        }

        int level_end = 0;
        if (_character[current_character].getStatus() == 4) level_end = 1;
        else if(_character[current_character].getStatus() == 5) level_end = 2;
        scr.ingame(renderer, _character[current_character].getHp(), _character[current_character].getDmg(), numCoin, numGem, paused, level_end);

        _character[current_character].show(renderer, view);

        if(current_level == numLevel)
            boss.show(renderer, &Audio_Player);

        {

            SDL_Rect rect = {_character[current_character].getX() - view, _character[current_character].getY() - 20, 42, 7};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderFillRect(renderer, &rect);

            int w = _character[current_character].getHp() * 40 / _character[current_character].getMaxHp();

            rect = {_character[current_character].getX() + 1 - view, _character[current_character].getY() - 19, w, 5};

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

            SDL_RenderFillRect(renderer, &rect);

            rect.x--;
            rect.y += 6;
            rect.h = 7;
            rect.w = 42;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);

            std::pair<int, int> temp = _character[current_character].getAttackBar();
            temp.first = std::min(temp.first, temp.second);

            rect.x++;
            rect.y++;
            rect.h = 5;
            rect.w = temp.first * 40 / temp.second;

            SDL_SetRenderDrawColor(renderer, 107, 107, 107, 255);
            if(temp.first == temp.second)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);

        }


        for(int i = 0; i < vProjectile.size(); i++){

            vProjectile[i].setW(pr_w[vProjectile[i].getId()]);
            vProjectile[i].setH(pr_h[vProjectile[i].getId()]);

            SDL_Rect tRect = vProjectile[i].getRect();
            tRect.x -= view;
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
            SDL_Rect tRect = vExplosion[i].rect;
            int r = vExplosion[i].radius;
            tRect.x = (2 * vExplosion[i].rect.x + vExplosion[i].rect.w) / 2 - sqrt(2) / 2 * r - view;
            tRect.y = (2 * vExplosion[i].rect.y + vExplosion[i].rect.h) / 2 - sqrt(2) / 2 * r;
            tRect.w = sqrt(2) * r;
            tRect.h = sqrt(2) * r;
            SDL_RenderCopy(renderer, explosion, NULL, &tRect);
            if(vExplosion[i].frame == 0)
            {
                std::swap(vExplosion[i], vExplosion.back());
                vExplosion.pop_back();
            }
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

    if(level < numLevel)
    {
        lp new_lp;
        new_lp.setType(1);
        new_lp.setW(70);
        new_lp.setH(70);
        new_lp.setX(MAP->get_lp_pos().back());
        MAP->pop();
        new_lp.loadVar({4, 14}, {4, 11}, {4, 17});
        v_lp.push_back(new_lp);
    }

    return 1;
}

void Handler::load(SDL_Renderer* renderer)
{
    character_level = new int[numCharacter];
    prev_char_level = new int[numCharacter];

    char_growth = new int* [numCharacter];
    for(int i = 0; i < numCharacter; i++)
        char_growth[i] = new int [2];

    std::ifstream player_info("player_info.txt");
    player_info >> numCoin >> numGem;
    player_info >> lastLevel;
    for(int i = 0; i < numCharacter; i++){
        player_info >> character_level[i] >> char_growth[i][0] >> char_growth[i][1];
    }

    player_info.close();

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

    Audio_Player.loadAudioFiles();

    boss.loadBoss(renderer);

    SDL_FreeSurface(loadSurface);

    ///////////////
}

void Handler::character1()
{
    for(int i = vProjectile.size() - 1; i >= 0; i--)
    {
        vProjectile[i].tick(MAP);
        vProjectile[i].setObjectId(++id, 0);
        if(vProjectile[i].done())
        {
            if(vProjectile[i].getThrew()){
                if(distance(vProjectile[i].getHitBox(), _character[current_character].getRect()) <= vProjectile[i].getRadius())
                {
                    int curHp = _character[current_character].getHp() - vProjectile[i].getDmg();
                    _character[current_character].setHp(curHp);
                }
                vExplosion.push_back({vProjectile[i].getHitBox(), vProjectile[i].getRadius(), vProjectile[i].getDmg(), 15});
                Audio_Player.bomb_explosion();
            }
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
                    vExplosion.push_back({vProjectile[i].getHitBox(), vProjectile[i].getRadius(), vProjectile[i].getDmg(), 15});
                    Audio_Player.bomb_explosion();
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

                if(collision(rectMob[j].first, vProjectile[i].getHitBox()) && vMob[j].getObjectId() != vProjectile[i].getObjectId())
                {
                    vMob[j].setObjectId(vProjectile[i].getObjectId(), 1);
                    rectMob[j].second = std::max(0, rectMob[j].second - vProjectile[i].getDmg());
                    vMob[j].setHp(rectMob[j].second);
                }
            }
            if(current_level == numLevel)
            {
                if(collision(vProjectile[i].getHitBox(), boss.getBossHitbox()) && boss.vulnerable())
                {
                    boss.takeDamage(vProjectile[i].getDmg());
                }
            }
        }

    }
}
