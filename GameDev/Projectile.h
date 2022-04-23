#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "GameObject.h"
#include "BasicFunction.h"
#include "game_map.h"

class Projectile : public GameObject
{

public:
    Projectile();
    ~Projectile();
    void tick(game_map* MAP);
    void shoot(SDL_Rect character, SDL_Rect mob, int _id, int _dmg, int _speed);
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

    void setThrew(bool _threw) {threw = _threw;}
    bool getThrew() {return threw;}

protected:
    double angle;
    double speed;
    bool threw;
    int curTime;
    int lastTime;
    int id;
    bool hostile;
    int radius;
};

#endif // PROJECTILE_H_
