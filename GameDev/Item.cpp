#include "Item.h"

Item::Item()
{
    fell = false;
    id = val = 0;
}
Item::~Item()
{
    Free();
}

void Item::tick(SDL_Rect character, game_map* MAP)
{
    if(!fell)
    {
        velY += gravity;
        if(velY > 15)
            velY = 15;
        rect.y += velY;
        if(rect.y >= SCREEN_HEIGHT || rect.y <= 0)
        {
            fell = true;
            return;
        }
        collisionY(MAP);

        rect.x += velX;
        collisionX(MAP);

    }
    else if(id != ID::TREASURE){
        double X = (2 * character.x + character.w - 2 * rect.x - rect.w) / 2;
        double Y = (2 * character.y + character.h - 2 * rect.y - rect.h) / 2;

        velX = X * 15  / sqrt(X * X + Y * Y);
        velY = Y * 15 / sqrt(X * X + Y * Y);

        rect.x += velX;
        rect.y += velY;
    }
}

void Item::dropItem(std::vector<Item>& vItem, int numCoin, int numGem)
{
    for(int i = 0; i < std::min(numCoin, 10); i++)
    {
        Item nItem;
        nItem.setX((rect.x * 2 + rect.w) / 2);
        nItem.setY((rect.y * 2 + rect.h) / 2);
        nItem.setVelY(-30);
        nItem.setVelX(Rand(-10, 10));
        nItem.setW(30);
        nItem.setH(30);
        nItem.setId(ID::COIN);
        int val = numCoin / 10;
        if(i < numCoin % 10) val++;
        nItem.setVal(val);
        vItem.push_back(nItem);
    }
    for(int i = 0; i < std::min(numGem, 10); i++)
    {
        Item nItem;
        nItem.setX((rect.x * 2 + rect.w) / 2);
        nItem.setY((rect.y * 2 + rect.h) / 2);
        nItem.setVelY(-30);
        nItem.setVelX(Rand(-10, 10));
        nItem.setW(30);
        nItem.setH(30);
        nItem.setId(ID::GEM);
        int val = numGem / 10;
        if(i < numGem % 10) val++;
        nItem.setVal(val);
        vItem.push_back(nItem);
    }

    int t = Rand(1, 1);
    if(t == 1)
    {
        Item nItem;
        nItem.setX((rect.x * 2 + rect.w) / 2);
        nItem.setY((rect.y * 2 + rect.h) / 2);
        nItem.setVelY(-30);
        nItem.setVelX(Rand(-10, 10));
        nItem.setW(30);
        nItem.setH(30);
        nItem.setId(ID::HEART);
        nItem.setVal(1);
        vItem.push_back(nItem);
    }
}

void Item::collisionX(game_map* MAP)
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

void Item::collisionY(game_map* MAP)
{
    bool ok = 0;
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
            ok = 1;
            rect.y = pos_y2 * TILE_SIZE - rect.h - 1;
            break;
        }
    if(ok) fell = true;
}
