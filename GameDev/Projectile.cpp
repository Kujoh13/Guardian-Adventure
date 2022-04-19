#include "Projectile.h"

Projectile::Projectile()
{
    angle = 0;
    isBomb = 0;
    curTime = 0;
    lastTime = 50;
    speed = 15;
    hostile = false;
    radius = 0;
}
Projectile::~Projectile()
{
    Free();
}

void Projectile::tick()
{
    rect.x += velX;
    rect.y += velY;

    curTime++;

}

void Projectile::shoot(SDL_Rect finish, SDL_Rect start, int _id, int _dmg)
{
    id = _id;

    dmg = _dmg;

    rect.x = (2 * start.x + start.w) / 2;
    rect.y = start.y;

    int x = (2 * finish.x + finish.w - 2 * start.x - start.w) / 2;
    int y = (2 * finish.y + finish.h - 2 * start.y - start.h) / 2;

    if(x * x + y * y == 0)
        angle = 0;
    else
        angle = std::acos(y / sqrt(x * x + y * y)) / (2 * pi) * 360;

    if(finish.x >= start.x) angle = 180.0 - angle;
    else angle = angle + 180.0;

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

