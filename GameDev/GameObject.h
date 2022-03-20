#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "BasicFunction.h"

class GameObject
{
public:
    GameObject();
    ~GameObject();
    void SetRect(const int &x, const int &y){rect.x = x, rect.y = y;}
    SDL_Rect getRect() {return rect;}
    SDL_Texture* getObject() {return pObject;}

    bool loadImage(std::string path, SDL_Renderer* renderer);
    void render(SDL_Renderer* ren, int view);
    void Free();

    int getX() {return rect.x;}
    int getY() {return rect.y;}
    int getW() {return rect.w;}
    int getH() {return rect.h;}
    int getVelX() {return velX;}
    int getVelY() {return velY;}
    int getHp() {return hp;}
    int getDmg() {return dmg;}

    void setX(int x) {rect.x = x;}
    void setY(int y) {rect.y = y;}
    void setW(int w) {rect.w = w;}
    void setH(int h) {rect.h = h;}
    void setVelX(int _velX) {velX = _velX;}
    void setVelY(int _velY) {velY = _velY;}
    void setHp(int _hp) {hp = std::max(_hp, 0);}
    void setDmg(int _dmg) {dmg = _dmg;}

protected:
    SDL_Texture* pObject;
    SDL_Rect rect;
    int velX, velY;
    int hp, dmg;
};

#endif
