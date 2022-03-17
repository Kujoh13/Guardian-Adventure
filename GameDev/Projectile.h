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
    bool collision(Character* character);
    void tick();
    void shoot(SDL_Rect character, SDL_Rect mob, int id);
    void setSpeed(double _speed);
    void addCurTime();
    bool done();
    double getAngle();
    int getId();
    SDL_Rect getHitBox(int view);

protected:
    double angle;
    double speed;
    bool isBomb;
    int curTime;
    int lastTime;
    int id;
};

#endif // PROJECTILE_H_
