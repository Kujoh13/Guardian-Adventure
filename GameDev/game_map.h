#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "BasicFunction.h"
#include "GameObject.h"

class game_map
{
public:
    game_map();
    ~game_map();
    bool loadMap(std::string path, SDL_Renderer* renderer, int level);
    void render(SDL_Renderer* ren);

protected:
    std::vector<SDL_Texture*> Block;
    SDL_Surface* Map_Sheet;
    SDL_Texture* background;
    int Map_X, Map_Y;
    int Num_Block;
};

Uint32 getpixel(SDL_Surface *surface, int x, int y);

#endif // GAME_MAP_H
