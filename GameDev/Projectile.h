#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "GameObject.h"
#include "BasicFunction.h"

class Projectile : public GameObject
{

public:
    Projectile();
    ~Projectile();
    void tick();
    void shoot(SDL_Rect character, SDL_Rect mob, int _id, int _dmg);
    void setSpeed(double _speed) {speed = _speed;}
    bool done();
    double getAngle();
    int getId();
    SDL_Rect getHitBox(int view);
    SDL_Rect getHitBox();
    void setHostile(bool _h) {hostile = _h;}
    bool getHostile() {return hostile;}

    void setRadius(int _radius) {radius = _radius;}
    int getRadius() {return radius;}

protected:
    double angle;
    double speed;
    bool isBomb;
    int curTime;
    int lastTime;
    int id;
    bool hostile;
    int radius;
};

#endif // PROJECTILE_H_
