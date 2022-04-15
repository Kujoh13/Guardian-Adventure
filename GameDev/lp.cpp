#include "lp.h"

lp::lp()
{
    _idle = {0, 0};
    _move = {0, 0};
    _victory = {0, 0};
    status = STATUS::IDLE;
    frame = 0;
    type = TYPE::FLOATING;
    done = false;
}

lp::~lp()
{
    Free();
}

void lp::loadVar(pii _idle_, pii _move_, pii _victory_)
{
    _idle = _idle_;
    _move = _move_;
    _victory = _victory_;
}

void lp::tick(SDL_Rect character, game_map* MAP)
{
    rect.x += velX;

    if(type == TYPE::GROUND)
    {
        rect.y += MAX_FALL_SPEED;
        collisionY(MAP);
    }

    if(rect.x < 0 && TYPE::FLOATING)
        done = true;

    if(type == TYPE::GROUND)
    {
        if(character.x >= MAP->victory && status != STATUS::VICTORY)
        {
            velX = -10;
            if(status != STATUS::MOVE) frame = 0;
            status = STATUS::MOVE;
        }

        if(rect.x <= character.x + character.w)
        {
            velX = 0;
            if(status != STATUS::VICTORY) frame = 0;
            status = STATUS::VICTORY;
        }
    }
}

void lp::show(SDL_Renderer* renderer, int view, SDL_Texture* texture)
{
    if(type == TYPE::FLOATING)
    {
        SDL_Rect nRect = {0, 0, 50, 50};
        SDL_Rect tRect = {rect.x, rect.y, 50, 50};

        nRect.x = frame * 50;

        SDL_RenderCopy(renderer, texture, &nRect, &tRect);

        frame++;
        if(frame == 19) frame -= 19;

        return;
    }



    pii temp;
    if(status == STATUS::IDLE)
        temp = _idle;
    else if(status == STATUS::MOVE)
        temp = _move;
    else if(status == STATUS::VICTORY)
        temp = _victory;

    SDL_Rect nRect = {0, 0, charSize, charSize};
    SDL_Rect tRect = {rect.x - 75 - view - 20, rect.y - 90, charSize, charSize};

    SDL_RenderDrawRect(renderer, &tRect);

    nRect.x = (frame % temp.first) * charSize;
    nRect.y = (frame / temp.first) * charSize;

    SDL_RenderCopy(renderer, texture, &nRect, &tRect);

    frame++;
    if(frame == temp.second) frame -= temp.second;
}

void lp::collisionY(game_map* MAP)
{
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
            rect.y = pos_y2 * TILE_SIZE - rect.h - 1;
            break;
        }
}

