#include "Projectile.h"

Projectile::Projectile()
{
    angle = 0;
    threw = false;
    curTime = 0;
    lastTime = 100;
    hostile = false;
    radius = 0;
}
Projectile::~Projectile()
{
    Free();
}

void Projectile::tick(game_map* MAP)
{
    if(threw)
    {
        velY += gravity;
        if(velY > MAX_FALL_SPEED) velY = MAX_FALL_SPEED;
    }
    rect.x += velX;
    rect.y += velY;

    if(threw)
    {
        if(rect.x >= 0 && rect.y >= 0 &&
           rect.x + rect.w < MAP->getMapWidth() * TILE_SIZE && rect.y + rect.h < MAP->getMapHeight() * TILE_SIZE)
        {
            int pos_x1 = (rect.x - 1) / TILE_SIZE;
            int pos_x2 = (rect.x + rect.w) / TILE_SIZE;
            int pos_y1 = rect.y / TILE_SIZE;
            int pos_y2 = (rect.y + rect.h) / TILE_SIZE;

            for(int i = pos_y1; i <= pos_y2; i++)
                if(MAP->getInfo()[i][pos_x1])
                {
                    curTime = lastTime - 1;
                }

            pos_x1 = rect.x / TILE_SIZE;
            pos_x2 = (rect.x + rect.w + 1) / TILE_SIZE;
            pos_y1 = rect.y / TILE_SIZE;
            pos_y2 = (rect.y + rect.h) / TILE_SIZE;

            for(int i = pos_y1; i <= pos_y2; i++)
                if(MAP->getInfo()[i][pos_x2])
                {
                    curTime = lastTime - 1;
                }

            pos_x1 = rect.x / TILE_SIZE;
            pos_x2 = (rect.x + rect.w) / TILE_SIZE;
            pos_y1 = (rect.y - 1) / TILE_SIZE;
            pos_y2 = (rect.y + rect.h) / TILE_SIZE;

            for(int i = pos_x1; i <= pos_x2; i++)
                if(MAP->getInfo()[pos_y1][i])
                {
                    curTime = lastTime - 1;
                }

            pos_x1 = rect.x / TILE_SIZE;
            pos_x2 = (rect.x + rect.w) / TILE_SIZE;
            pos_y1 = rect.y / TILE_SIZE;
            pos_y2 = (rect.y + rect.h + 1) / TILE_SIZE;

            for(int i = pos_x1; i <= pos_x2; i++)
                if(MAP->getInfo()[pos_y2][i])
                {
                    curTime = lastTime - 1;
                }
        }
    }

    curTime++;

}

void Projectile::shoot(SDL_Rect finish, SDL_Rect start, int _id, int _dmg, int _speed)
{
    id = _id;

    dmg = _dmg;

    speed = _speed;

    rect.x = (2 * start.x + start.w) / 2;
    rect.y = (2 * start.y + start.h) / 2;

    int x = (2 * finish.x + finish.w - 2 * start.x - start.w) / 2;
    int y = (2 * finish.y + finish.h - 2 * start.y - start.h) / 2;

    if(threw)
    {
        double t = abs(double(x) / speed);
        velX = -speed;
        if(finish.x >= start.x) velX = speed;
        velY = (y - 0.5 * gravity * t * t) / t;
        return;
    }

    if(x * x + y * y == 0)
        angle = 0;
    else
        angle = std::acos(x / sqrt(x * x + y * y)) / (2 * pi) * 360;

    if(finish.y <= start.y) angle = 360.0 - angle;


    double X = (2 * finish.x + finish.w - 2 * start.x - start.w) / 2;
    double Y = (2 * finish.y + finish.h - 2 * start.y - start.h) / 2;

    if(X * X + Y * Y == 0)
        std :: cout << "Divide by 0!!!" << '\n';

    velX = X * speed / sqrt(X * X + Y * Y);

    velY = Y * speed / sqrt(X * X + Y * Y);

}

SDL_Rect Projectile::getHitBox(int view)
{
    return {rect.x - view, rect.y + rect.h / 3, rect.w, rect.h / 3};
}

SDL_Rect Projectile::getHitBox()
{
    return {rect.x, rect.y + rect.h / 3, rect.w, rect.h / 3};
}

bool Projectile::done()
{
    return curTime >= lastTime;
}

double Projectile::getAngle()
{
    return angle;
}

int Projectile::getId()
{
    return id;
}

