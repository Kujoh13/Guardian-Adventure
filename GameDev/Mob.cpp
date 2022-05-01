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
    dmg = 0;
    hp = 0;
    mobId = 0;
    hostile = true;
    weapon = 0;

    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};

    for(int i = 0; i < 3; i++)
        itemDrop[i] = 0;
}
Mob::~Mob()
{
    Free();
    for(int i = 0; i <= 1; i++){
        SDL_DestroyTexture(idleAnimation[i]);
        SDL_DestroyTexture(moveAnimation[i]);
        SDL_DestroyTexture(attackAnimation[i]);
    }
    for(int i = 0; i <= 1; i++){
        idleAnimation[i] = NULL;
        attackAnimation[i] = NULL;
        moveAnimation[i] = NULL;
    }
}
bool Mob::loadMob(std::string path, SDL_Renderer* renderer)
{
    std::ifstream file(path + "/mob_info.txt");

    if(!file.is_open())
        return false;

    file >> _idle.first >> _idle.second;
    file >> _move.first >> _move.second;
    file >> _attack.first >> _attack.second;
    file >> framePerAttack;
    file >> idProjectile;
    file >> rect.w >> rect.h;
    file >> hp >> dmg;
    maxHp = hp;
    file >> type;
    file >> velX;
    file >> weapon;
    if(type == TYPE::MELEE){
        file >> melee.x >> melee.y >> melee.w >> melee.h;
        file >> frameAttack;
    }
    else{
        file >> prSpeed;
        if(s[weapon] == "bomb") file >> prRadius;
    }
    file >> itemDrop[0];
    file.close();


    SDL_Surface* loadedSurface;

    for(int i = 0; i <= 1; i++){

        std::string st = int2str(i) + ".png";

        loadedSurface = IMG_Load((path + "/idle_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            idle = true;
            idleAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }
        else std::cout << SDL_GetError() << '\n';

        loadedSurface = IMG_Load((path + "/move_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            move = true;
            moveAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }
        else std::cout << SDL_GetError() << '\n';

        loadedSurface = IMG_Load((path + "/attack_" + s[weapon] + st).c_str());

        if(loadedSurface != NULL){
            attack = true;
            attackAnimation[i] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        }
        else std::cout << SDL_GetError() << '\n';
    }

    SDL_FreeSurface(loadedSurface);

    return 1;
}

void Mob::drawIdle(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    nRect.x = (frame % _idle.first) * charSize;
    nRect.y = (frame / _idle.first) * charSize;

    SDL_RenderCopy(renderer, idleAnimation[facing], &nRect, &tRect);

    frame++;
    if(frame == _idle.second) frame -= _idle.second;
}

void Mob::drawMove(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    nRect.x = (frame % _move.first) * charSize;
    nRect.y = (frame / _move.first) * charSize;
    SDL_RenderCopy(renderer, moveAnimation[facing], &nRect, &tRect);
    frame++;
    if(frame == _move.second) frame -= _move.second;
}

void Mob::drawAttack(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - charSize / 3 - view - (charSize / 3 - rect.w) * facing, rect.y - charSize / 3 - (charSize / 3 - rect.h), charSize, charSize};

    nRect.x = (frame % _attack.first) * charSize;
    nRect.y = (frame / _attack.first) * charSize;
    SDL_RenderCopy(renderer, attackAnimation[facing], &nRect, &tRect);
    frame++;
}

void Mob::show(SDL_Renderer* renderer, int view)
{

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect nRect = rect;
    nRect.x -= view;

    SDL_RenderDrawRect(renderer, &nRect);

    if(type == TYPE::MELEE){
        SDL_Rect tempRect = rect;
        tempRect.x += melee.x;
        tempRect.y += melee.y;
        tempRect.w = melee.w;
        tempRect.h = melee.h;

        if(facing) tempRect.x -= melee.w - rect.w;

        tempRect.x -= view;

        SDL_RenderDrawRect(renderer, &tempRect);
    }

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

void Mob::tick(game_map* MAP, std::vector<Projectile> &vProjectile, Character* character)
{
    if(move)
    {
        if(facing) rect.x -= velX;
        else rect.x += velX;
    }
    if(idle)
    {
        if(rect.x >= character->getX()) facing = 1;
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

    if(nextAttack == 0 && type == TYPE::RANGED && abs(rect.x - character->getX()) <= SCREEN_WIDTH)
    {
        Projectile temp;
        if(s[weapon] == "bomb") temp.setThrew(true);
        if(s[weapon] == "bomb") temp.setRadius(prRadius);
        temp.shoot(character->getRect(), rect, idProjectile, dmg, prSpeed);
        temp.setHostile(true);
        vProjectile.push_back(temp);
    }

    if(nextAttack == frameAttack && type == TYPE::MELEE){

        SDL_Rect tempRect = rect;
        tempRect.x += melee.x;
        tempRect.y += melee.y;
        tempRect.w = melee.w;
        tempRect.h = melee.h;

        if(facing) tempRect.x -= melee.w - charWidth;

        if(collision(tempRect, character->getRect())){
            int chance = Rand(1, 4);
            character->setHp(character->getHp() - dmg);
            if(chance == 1 && character->getId() == 2)
                hp = std::max(0, hp - dmg);
        }
    }
}

void Mob::spawnItem(std::vector<Item>& vItem)
{
    for(int id = 0; id < 3; id++)
        for(int i = 0; i < std::min(itemDrop[id], 10); i++){
            Item nItem;
            nItem.setX((rect.x * 2 + rect.w) / 2);
            nItem.setY((rect.y * 2 + rect.h) / 2);
            nItem.setVelY(-30);
            nItem.setVelX(Rand(-10, 10));
            nItem.setW(30);
            nItem.setH(30);
            nItem.setId(id);
            int val = itemDrop[id] / 10;
            if(i < itemDrop[id] % 10) val++;
            nItem.setVal(val);
            vItem.push_back(nItem);
        }
}

void Mob::collisionX(game_map* MAP)
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

void Mob::collisionY(game_map* MAP)
{
    bool ok = 1;
    int pos_x1 = rect.x / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = (rect.y - 1) / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->getInfo()[pos_y1][i])
        {
            rect.y = (pos_y1 + 1) * TILE_SIZE;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h + 1) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->getInfo()[pos_y2][i])
        {
            ok = 0;
            rect.y = pos_y2 * TILE_SIZE - rect.h - 1;
            break;
        }
}

std::pair<int, int> Mob::getAttackBar()
{
    return {nextAttack, framePerAttack};
}

void Mob::setType(int _type)
{
    type = _type;
}

void Mob::setWeapon(int _weapon)
{
    weapon = _weapon;
}

void Mob::setId(int _id)
{
    mobId = _id;
}

int Mob::getId()
{
    return mobId;
}

void Mob::setRange(int l, int r)
{
    minX = l, maxX = r;
}
