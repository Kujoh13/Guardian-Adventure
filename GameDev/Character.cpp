#include "Character.h"


Character::Character()
{
    frame = 0;
    status = 0;
    id = 0;
    isFalling = 1;
    isJumping = 0;

    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};
    _died = {0, 0};
    _victory = {0, 0};
}

Character::~Character()
{
    _idle = {0, 0};
    _move = {0, 0};
    _attack = {0, 0};
    _died = {0, 0};
    _victory = {0, 0};

    frame = 0;
    SDL_DestroyTexture(jump);
    SDL_DestroyTexture(idleAnimation);
    SDL_DestroyTexture(moveAnimation);
    SDL_DestroyTexture(diedAnimation);
    SDL_DestroyTexture(victoryAnimation);
    SDL_DestroyTexture(attackAnimation);
    Free();
}

bool Character::loadCharacter(std::string path, SDL_Renderer* renderer, int _id)
{
    SDL_Surface* loadedSurface;

    loadedSurface = IMG_Load((path + "/jump.png").c_str());
    if(loadedSurface == NULL) return 0;

    jump = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/idle.png").c_str());
    if(loadedSurface == NULL) return 0;

    idleAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/move.png").c_str());
    if(loadedSurface == NULL) return 0;

    moveAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/died.png").c_str());
    if(loadedSurface == NULL) return 0;

    diedAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/attack.png").c_str());
    if(loadedSurface == NULL) return 0;

    attackAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    loadedSurface = IMG_Load((path + "/victory.png").c_str());
    if(loadedSurface == NULL) return 0;

    victoryAnimation = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    SDL_FreeSurface(loadedSurface);

    id = _id;

    {
        std::ifstream file(path + "/info.txt");
        file >> _idle.first >> _idle.second;
        file >> _move.first >> _move.second;
        file >> _attack.first >> _attack.second;
        file >> _died.first >> _died.second;
        file >> _victory.first >> _victory.second;
        file.close();
    }

    rect = {0, 0, charWidth, charHeight};

    return 1;
}

void Character::drawIdle(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, 240, 240};
    SDL_Rect tRect = {rect.x - 75 - view, rect.y - 75, 240, 240};
    if(status != 0){
        frame = 0;
    }
    else{
        frame++;
        if(frame == _idle.second) frame -= _idle.second;
    }
    status = 0;

    nRect.x = (frame % _idle.first) * charSize;
    nRect.y = (frame / _idle.first) * charSize;
    SDL_RenderCopy(renderer, idleAnimation, &nRect, &tRect);
}

void Character::drawMove(SDL_Renderer* renderer, int view)
{
    SDL_Rect nRect = {0, 0, 240, 240};
    SDL_Rect tRect = {rect.x - 75 - view, rect.y - 75, 240, 240};
    if(status != 1){
        frame = 0;
    }
    else{
        frame++;
        if(frame == _move.second) frame -= _move.second;
    }
    status = 1;

    nRect.x = (frame % _move.first) * charSize;
    nRect.y = (frame / _move.first) * charSize;
    SDL_RenderCopy(renderer, moveAnimation, &nRect, &tRect);
}

void Character::show(SDL_Renderer* renderer, int view)
{

    //std::cout << view << '\n';
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_Rect tRect = rect;
    tRect.x = rect.x - view;

    SDL_RenderDrawRect(renderer, &tRect);

    if(isJumping || isFalling)
    {
        SDL_Rect nRect = {rect.x - 75 - view, rect.y - 75, 240, 240};
        SDL_RenderCopy(renderer, jump, NULL, &nRect);
        status = 2;
    }
    else
    {
        if(nStatus == 0) drawIdle(renderer, view);
        if(nStatus == 1) drawMove(renderer, view);
    }
    velX = 0;
}

void Character::handleInput(SDL_Event event,const Uint8 *keyboard_state_array)
{
//    bool ok = 1;
//
//    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
//        && isFalling == false)
//    {
//        isJumping = isFalling = true;
//        velY = -34;
//        nStatus = 2;
//    }
//
//    if(keyboard_state_array[SDL_SCANCODE_A])
//    {
//        nStatus = 1;
//        velX = -MAX_RUN_SPEED;
//        leftP = true;
//        rightP = false;
//        ok = 0;
//    }
//    if(keyboard_state_array[SDL_SCANCODE_D])
//    {
//        nStatus = 1;
//        velX = MAX_RUN_SPEED;
//        leftP = false;
//        rightP = true;
//        ok = 0;
//    }
//
//
//    if(isFalling || isJumping)
//    {
//        if(leftP)
//            velX = -MAX_RUN_SPEED;
//        else if(rightP)
//            velX = MAX_RUN_SPEED;
//    }
//
//    if(isFalling)
//        std::cout << leftP << ' ' << rightP << '\n';
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
                    nStatus = 2;
                    isFalling = isJumping = true;
                }
            }
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
        }
    }


}

void Character::tick(game_map* MAP)
{
    if(pressed[' ']) {
        isJumping = isFalling = true;
        nStatus = 2;
        velY = -40;
    }

    pressed[' '] = false;

    if(isJumping || isFalling){
        velY += gravity;

        if(velY > MAX_FALL_SPEED)
            velY = MAX_FALL_SPEED;

    }

    rect.y += velY;

    collisionY(MAP);

    if(pressed['a'])
    {
        nStatus = 1;
        rect.x -= MAX_RUN_SPEED;
    }
    else if(pressed['d'])
    {
        nStatus = 1;
        rect.x += MAX_RUN_SPEED;
    }

    if(rect.x < 0) rect.x = 0;
    if(rect.x > MAP->Map_X * TILE_SIZE) rect.x = MAP->Map_X * TILE_SIZE;

    collisionX(MAP);

}

void Character::collisionX(game_map* MAP)
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

void Character::collisionY(game_map* MAP)
{
    bool ok = 1;
    int pos_x1 = rect.x / TILE_SIZE;
    int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    int pos_y1 = (rect.y - 1) / TILE_SIZE;
    int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->info[pos_y1][i])
        {
            velY = 0;
            rect.y = (pos_y1 + 1) * TILE_SIZE;
            isJumping = false;
            isFalling = true;
            nStatus = 2;
            break;
        }

    pos_x1 = rect.x / TILE_SIZE;
    pos_x2 = (rect.x + rect.w) / TILE_SIZE;
    pos_y1 = rect.y / TILE_SIZE;
    pos_y2 = (rect.y + rect.h + 1) / TILE_SIZE;

    for(int i = pos_x1; i <= pos_x2; i++)
        if(MAP->info[pos_y2][i])
        {
            velY = 0;
            ok = 0;
            rect.y = pos_y2 * TILE_SIZE - charHeight - 1;
            isJumping = isFalling = false;
            break;
        }

    if(ok) isFalling = true;

}

void Character::setStatus(int _status)
{
    nStatus = _status;
}

int Character::getStatus()
{
    return nStatus;
}
