#include "Character.h"


Character::Character()
{
    frame = 0;
    status = nStatus = 0;
    id = 0;
    isFalling = true;
    isJumping = false;
    finishAttack = true;
    nextAttack = 0;
    facing = 0;
    type = 1;

    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};
    _died = {0, 0};
    _victory = {0, 0};

    for(int i = 0; i <= 1; i++){
        jumpAnimation[i] = NULL;
        idleAnimation[i] = NULL;
        moveAnimation[i] = NULL;
        diedAnimation[i] = NULL;
        attackAnimation[i] = NULL;
    }
    velX = MAX_RUN_SPEED;
}

Character::~Character()
{
    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};
    _died = {0, 0};
    _victory = {0, 0};

    frame = 0;
    for(int i = 0; i <= 1; i++){
        SDL_DestroyTexture(jumpAnimation[i]);
        SDL_DestroyTexture(idleAnimation[i]);
        SDL_DestroyTexture(moveAnimation[i]);
        SDL_DestroyTexture(diedAnimation[i]);
        SDL_DestroyTexture(attackAnimation[i]);
    }
    SDL_DestroyTexture(victoryAnimation);
    Free();
}

bool Character::loadCharacter(std::string path, SDL_Renderer* renderer, int _id)
{
    SDL_Surface* loadedSurface;

    for(int i = 0; i <= 1; i++){
        std::string s = int2str(i) + ".png";
        loadedSurface = IMG_Load((path + "/jump" + s).c_str());
        if(loadedSurface == NULL) return 0;

        jumpAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        loadedSurface = IMG_Load((path + "/idle" + s).c_str());
        if(loadedSurface == NULL) return 0;

        idleAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        loadedSurface = IMG_Load((path + "/move" + s).c_str());
        if(loadedSurface == NULL) return 0;

        moveAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        loadedSurface = IMG_Load((path + "/died" + s).c_str());
        if(loadedSurface == NULL) return 0;

        diedAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        loadedSurface = IMG_Load((path + "/attack" + s).c_str());
        if(loadedSurface == NULL) return 0;

        attackAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    }
    loadedSurface = IMG_Load((path + "/victory.png").c_str());

    if(loadedSurface == NULL) return 0;

    victoryAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/portrait.png").c_str());

    if(loadedSurface == NULL) return 0;

    portrait = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    SDL_FreeSurface(loadedSurface);

    id = _id;

    {
        std::ifstream file(path + "/char_info.txt");
        file >> rect.w >> rect.h;
        file >> type;
        file >> _idle.first >> _idle.second;
        file >> _move.first >> _move.second;
        file >> _attack.first >> _attack.second;
        file >> _died.first >> _died.second;
        file >> _victory.first >> _victory.second;
        file >> framePerAttack;
        file >> baseHp >> baseDmg;
        maxHp = baseHp;
        if(type == TYPE::MELEE){
            file >> melee.x >> melee.y >> melee.w >> melee.h;
            file >> frameAttack;
        }
        else{
            file >> idProjectile >> prSpeed >> prLastTime;
        }
        file.close();
    }

    return 1;
}

void Character::drawIdle(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    if(nStatus != status)
        frame = 0;

    status = STATUS::IDLE;

    nRect.x = (frame % _idle.first) * charSize;
    nRect.y = (frame / _idle.first) * charSize;
    SDL_RenderCopy(renderer, idleAnimation[facing], &nRect, &tRect);
    frame++;
    if(frame == _idle.second) frame -= _idle.second;
}

void Character::drawMove(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    if(nStatus != status)
        frame = 0;

    status = STATUS::MOVE;

    nRect.x = (frame % _move.first) * charSize;
    nRect.y = (frame / _move.first) * charSize;
    SDL_RenderCopy(renderer, moveAnimation[facing], &nRect, &tRect);
    frame++;
    if(frame == _move.second) frame -= _move.second;
}

void Character::drawDied(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    if(nStatus != status)
        frame = 0;

    if(frame == _died.second) frame--;

    status = STATUS::DIED;

    nRect.x = (frame % _died.first) * charSize;
    nRect.y = (frame / _died.first) * charSize;
    SDL_RenderCopy(renderer, diedAnimation[facing], &nRect, &tRect);
    frame++;
}

void Character::drawVictory(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    if(nStatus != status)
        frame = 0;

    if(frame == _victory.second) frame -= _victory.second;

    status = STATUS::VICTORY;

    nRect.x = (frame % _victory.first) * charSize;
    nRect.y = (frame / _victory.first) * charSize;
    SDL_RenderCopy(renderer, victoryAnimation, &nRect, &tRect);
    frame++;
}

void Character::drawAttack(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    if(nStatus != status && finishAttack) frame = 0;

    if(frame == _attack.second - 1)
    {
        finishAttack = true;
    }
    status = STATUS::ATTACK;

    nRect.x = (frame % _attack.first) * charSize;
    nRect.y = (frame / _attack.first) * charSize;
    SDL_RenderCopy(renderer, attackAnimation[facing], &nRect, &tRect);
    frame++;
}

void Character::show(SDL_Renderer* renderer, int view)
{

    SDL_Rect pRect = {10, 10, portraitSize, portraitSize};

    SDL_RenderCopy(renderer, portrait, NULL, &pRect);

    //std::cout << view << '\n';
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect tRect = rect;
    tRect.x -= view;

    SDL_RenderDrawRect(renderer, &tRect);

    if(type == TYPE::MELEE)
    {
        SDL_Rect tempRect = rect;
        tempRect.x += melee.x;
        tempRect.y += melee.y;
        tempRect.w = melee.w;
        tempRect.h = melee.h;

        if(facing) tempRect.x -= melee.w - charWidth;

        tempRect.x -= view;
        SDL_RenderDrawRect(renderer, &tempRect);
    }

    if(nStatus != status && (finishAttack || nextAttack == 0)) frame = 0;

    if(nStatus == STATUS::DIED)
    {
        drawDied(renderer, view);
        return;
    }
    if(nStatus == STATUS::VICTORY)
    {
        drawVictory(renderer, view);
        return;
    }

    nextAttack++;

    if(nStatus == STATUS::ATTACK || !finishAttack)
    {
        drawAttack(renderer, view);
    }
    else if(isJumping || isFalling)
    {
        SDL_Rect nRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};
        SDL_RenderCopy(renderer, jumpAnimation[facing], NULL, &nRect);
        status = STATUS::JUMP;
        return;
    }

    else if(nStatus == STATUS::IDLE) drawIdle(renderer, view);
    else if(nStatus == STATUS::MOVE) drawMove(renderer, view);
}

bool Character::handleInput(SDL_Event event)
{
    bool flag = true;
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_a:
            {
                pressed['a'] = 1;
                pressed['d'] = 0;
            }
            break;
        case SDLK_d:
            {
                pressed['a'] = 0;
                pressed['d'] = 1;
            }
            break;
        case SDLK_SPACE:
            {
                if(isFalling == false){
                    pressed[' '] = 1;
                }
            }
            break;
        case SDLK_k:
            {
                if(nextAttack >= framePerAttack){
                    pressed['k'] = 1;
                }
            }
            break;
        }
    }

    else if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_a:
            {
                pressed['a'] = 0;
            }
            break;
        case SDLK_d:
            {
                pressed['d'] = 0;
            }
            break;
        case SDLK_SPACE:
            {
                pressed[' '] = 0;
            }
        case SDLK_k:
            {
                pressed['k'] = 0;
            }
            break;
        }


    }
    if(pressed['a']) flag = false;
    if(pressed['d']) flag = false;
    if(pressed[' ']) flag = false;
    if(pressed['k']) flag = false;
    if(!flag)
        return true;
}

void Character::tick(game_map* MAP, std::vector<std::pair<SDL_Rect, int> >& rectMob, std::vector<Projectile>& vProjectile)
{

    if(pressed[' '] && isFalling == false && status != 4) {
        isJumping = isFalling = true;
        nStatus = STATUS::JUMP;
        velY = -40;
    }

    pressed[' '] = false;

    if(isJumping || isFalling){
        velY += gravity;

        if(velY > MAX_FALL_SPEED)
            velY = MAX_FALL_SPEED;

    }

    rect.y += velY;

    if(rect.y < 0) rect.y = 0;

    if(rect.y + rect.h >= MAP->getMapHeight() * TILE_SIZE - 2) rect.y = MAP->getMapHeight() * TILE_SIZE - rect.h - 2;

    collisionY(MAP);

    if(rect.y + rect.h >= MAP->getMapHeight() * TILE_SIZE - 2 || hp == 0)
    {
        nStatus = STATUS::DIED;
        return;
    }

    if(rect.x >= MAP->getVictory())
    {
        nStatus = STATUS::VICTORY;
        return;
    }

    if(pressed['a'])
    {
        facing = 1;
        nStatus = STATUS::MOVE;
        rect.x -= velX;
    }
    else if(pressed['d'])
    {
        nStatus = STATUS::MOVE;
        facing = 0;
        rect.x += velX;
    }

    if(rect.x < 0) rect.x = 0;
    if(rect.x > MAP->getMapWidth() * TILE_SIZE) rect.x = MAP->getMapWidth() * TILE_SIZE;

    collisionX(MAP);

    if(pressed['k'] && nextAttack >= framePerAttack)
    {
        nStatus = STATUS::ATTACK;
        finishAttack = false;
        nextAttack = 0;
    }

    if(!finishAttack && frame == frameAttack && type == TYPE::MELEE){

        SDL_Rect tempRect = rect;
        tempRect.x += melee.x;
        tempRect.y += melee.y;
        tempRect.w = melee.w;
        tempRect.h = melee.h;

        if(facing) tempRect.x -= melee.w - charWidth;

        for(int i = 0; i < rectMob.size(); i++){
            if(collision(tempRect, rectMob[i].first)){
                rectMob[i].second -= dmg;
                if(id == 0)
                {
                    int chance = Rand(1, 5);
                    if(chance == 1)
                        rectMob[i].second -= dmg / 2;
                }
            }
        }

        if(vProjectile.size())
        for(int i = vProjectile.size() - 1; i >= 0; i--)
        {
            if(collision(tempRect, vProjectile[i].getRect()) && vProjectile[i].getThrew() == false)
            {
                std::swap(vProjectile[i], vProjectile.back());
                vProjectile.pop_back();
            }
        }

    }

    if(nStatus == STATUS::ATTACK && nextAttack == 0 && type == TYPE::RANGED){
        Projectile pr;
        SDL_Rect tRect = rect;
        if(facing) tRect.x -= SCREEN_WIDTH;
        else tRect.x += SCREEN_WIDTH;
        pr.setHostile(false);
        pr.setSpeed(double(prSpeed));
        pr.setLastTime(prLastTime);
        pr.shoot(tRect, rect, idProjectile, dmg, prSpeed);
        vProjectile.push_back(pr);
    }
}

void Character::collisionX(game_map* MAP)
{

    int pos_x1 = (rect.x - 1) / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = rect.y / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_y1; i <= pos_y2; i++)
        if(MAP->getInfo()[i][pos_x1])
        {
            rect.x = (pos_x1 + 1) * TILE_SIZE;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w + 1) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_y1; i <= pos_y2; i++)
        if(MAP->getInfo()[i][pos_x2])
        {
            rect.x = pos_x2 * TILE_SIZE - rect.w - 1;
            break;
        }

}

void Character::collisionY(game_map* MAP)
{
    bool ok = 1;
    int pos_x1 = rect.x / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = (rect.y - 1) / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->getInfo()[pos_y1][i])
        {
            velY = 0;
            rect.y = (pos_y1 + 1) * TILE_SIZE;
            isJumping = false;
            isFalling = true;
            nStatus = STATUS::JUMP;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h + 1) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->getInfo()[pos_y2][i])
        {
            velY = 0;
            ok = 0;
            rect.y = pos_y2 * TILE_SIZE - rect.h - 1;
            isJumping = isFalling = false;
            break;
        }

    if(ok) isFalling = true;

}
std::pair<int, int> Character::getAttackBar()
{
    return {nextAttack, framePerAttack};
}

void Character::setStatus(int _status)
{
    nStatus = _status;
}

int Character::getStatus()
{
    return nStatus;
}

bool Character::getAttack()
{
    return finishAttack;
}
