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
    void render(SDL_Renderer* ren);
    void Free();

protected:
    SDL_Texture* pObject;
    SDL_Rect rect;
    int velX, velY;
};

#endif
