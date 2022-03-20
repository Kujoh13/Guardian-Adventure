#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "GameObject.h"
#include "Character.h"
#include "BasicFunction.h"

class Projectile : public GameObject
{

public:
    Projectile();
    ~Projectile();
    bool collide(Character* character);
    void tick();
    void shoot(SDL_Rect character, SDL_Rect mob, int _id, int _dmg);
    void setSpeed(double _speed);
    void addCurTime();
    bool done();
    double getAngle();
    int getId();
    SDL_Rect getHitBox(int view);
    SDL_Rect getHitBox();

protected:
    double angle;
    double speed;
    bool isBomb;
    int curTime;
    int lastTime;
    int id;
};

#endif // PROJECTILE_H_
