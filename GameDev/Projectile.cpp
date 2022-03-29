#include "Projectile.h"

Projectile::Projectile()
{
    angle = 0;
    isBomb = 0;
    curTime = 0;
    lastTime = 50;
    speed = 15;
}
Projectile::~Projectile()
{
    Free();
}

bool Projectile::collide(Character* character)
{
    SDL_Rect hitbox = getHitBox();

    if(collision(hitbox, character->getRect()))
    {
        character->setHp(character->getHp() - dmg);
        return 1;
    }
    return 0;
}

void Projectile::tick()
{
    rect.x += velX;
    rect.y += velY;

    curTime++;

}

void Projectile::shoot(SDL_Rect character, SDL_Rect mob, int _id, int _dmg)
{
    id = _id;

    dmg = _dmg;

    rect.x = (2 * mob.x + mob.w) / 2;
    rect.y = (2 * mob.y + mob.h) / 2;

    int x = (2 * character.x + character.w - 2 * mob.x - mob.w) / 2;
    int y = (2 * character.y + character.h - 2 * mob.y - mob.h) / 2;

    if(x * x + y * y == 0)
        angle = 0;
    else
        angle = std::acos(y / sqrt(x * x + y * y)) / (2 * pi) * 360;

    if(character.x >= mob.x) angle = 180.0 - angle;
    else angle = angle + 180.0;

    double X = (2 * character.x + character.w - 2 * mob.x - mob.w) / 2;
    double Y = (2 * character.y + character.h - 2 * mob.y - mob.h) / 2;

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

void Projectile::setSpeed(double _speed)
{
    speed = _speed;
}

void Projectile::addCurTime()
{
    curTime++;
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

