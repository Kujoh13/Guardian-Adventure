#include "Mob.h"

Mob::Mob()
{
    for(int i = 0; i <= 1; i++){
        idleAnimation[i] = NULL;
        attackAnimation[i] = NULL;
        moveAnimation[i] = NULL;
    }
    move = attack = idle = false;
    facing = 1;
    frame = 0;
    type = 1;
    maxX = minX = 0;
    nextAttack = 0;
    idProjectile = 0;

    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};
}
Mob::~Mob()
{
    for(int i = 0; i <= 1; i++){
        SDL_DestroyTexture(idleAnimation[i]);
        SDL_DestroyTexture(moveAnimation[i]);
        SDL_DestroyTexture(attackAnimation[i]);
    }
}
bool Mob::loadMob(std::string path, SDL_Renderer* renderer)
{
    SDL_Surface* loadedSurface;

    for(int i = 0; i <= 1; i++){

        std::string st = int2str(i) + ".png";

        loadedSurface = IMG_Load((path + "/idle_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            idle = true;
            idleAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }

        loadedSurface = IMG_Load((path + "/move_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            move = true;
            moveAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }

        loadedSurface = IMG_Load((path + "/attack_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            attack = true;
            attackAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }
    }

    std::ifstream file(path + "/info.txt");

    if(!file.is_open())
        return false;

    file >> _idle.first >> _idle.second;
    file >> _move.first >> _move.second;
    file >> _attack.first >> _attack.second;
    file >> minX >> maxX;
    file >> framePerAttack;
    file >> idProjectile;

    file.close();

    SDL_FreeSurface(loadedSurface);
    rect = {450, -100, 40, 85};

    return 1;
}

void Mob::drawIdle(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - 75 - view - 50 * facing, rect.y - 75, charSize, charSize};

    nRect.x = (frame % _idle.first) * charSize;
    nRect.y = (frame / _idle.first) * charSize;

    SDL_RenderCopy(renderer, idleAnimation[facing], &nRect, &tRect);
    frame++;
    if(frame == _idle.second) frame -= _idle.second;
}

void Mob::drawMove(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - 75 - view - 50 * facing, rect.y - 75, charSize, charSize};

    nRect.x = (frame % _move.first) * charSize;
    nRect.y = (frame / _move.first) * charSize;
    SDL_RenderCopy(renderer, moveAnimation[facing], &nRect, &tRect);
    frame++;
    if(frame == _move.second) frame -= _move.second;
}

void Mob::drawAttack(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - 75 - view - 50 * facing, rect.y - 75, charSize, charSize};

    nRect.x = (frame % _attack.first) * charSize;
    nRect.y = (frame / _attack.first) * charSize;
    SDL_RenderCopy(renderer, attackAnimation[facing], &nRect, &tRect);
    frame++;
}

void Mob::show(SDL_Renderer* renderer, int view)
{

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect tRect = rect;
    tRect.x = rect.x - view;

    SDL_RenderDrawRect(renderer, &tRect);

    if(nextAttack >= _attack.second){
        if(nextAttack == _attack.second) frame = 0;
        if(idle) drawIdle(renderer, view);
        else drawMove(renderer, view);
    }
    else {
        drawAttack(renderer, view);
    }

    nextAttack++;
    if(nextAttack == framePerAttack) {
        nextAttack -= framePerAttack;
        frame = 0;
    }

}

void Mob::tick(game_map* MAP, std::vector<Projectile> &vProjectile, SDL_Rect character)
{
    if(move)
    {
        if(facing) rect.x -= velX;
        else rect.x += velX;
    }
    if(idle)
    {
        if(rect.x >= character.x) facing = 1;
        else facing = 0;
    }
    if(rect.x > maxX)
    {
        rect.x = maxX;
        facing = 1;
    }
    else if(rect.x < minX)
    {
        rect.x = minX;
        facing = 0;
    }

    collisionX(MAP);

    rect.y += MAX_FALL_SPEED;
    collisionY(MAP);

    if(nextAttack == 0)
    {
        Projectile temp;
        temp.shoot(character, rect, idProjectile);
        vProjectile.push_back(temp);
    }

}

void Mob::collisionX(game_map* MAP)
{

    int pos_x1 = (rect.x - 1) / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = rect.y / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_y1; i <= pos_y2; i++)
        if(MAP->info[i][pos_x1])
        {
            rect.x = (pos_x1 + 1) * TILE_SIZE;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w + 1) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_y1; i <= pos_y2; i++)
        if(MAP->info[i][pos_x2])
        {
            rect.x = pos_x2 * TILE_SIZE - charWidth - 1;
            break;
        }

}

void Mob::collisionY(game_map* MAP)
{
    bool ok = 1;
    int pos_x1 = rect.x / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = (rect.y - 1) / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->info[pos_y1][i])
        {
            rect.y = (pos_y1 + 1) * TILE_SIZE;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h + 1) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->info[pos_y2][i])
        {
            ok = 0;
            rect.y = pos_y2 * TILE_SIZE - charHeight - 1;
            break;
        }
}

void Mob::setType(int _type)
{
    type = _type;
}

void Mob::setWeapon(int _weapon)
{
    weapon = _weapon;
}
