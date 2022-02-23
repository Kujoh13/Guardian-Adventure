#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "BasicFunction.h"
#include "GameObject.h"

class game_map
{
public:
    game_map();
    ~game_map();
    bool loadMap(std::string path, SDL_Renderer* renderer);
    void render(SDL_Renderer* ren);

protected:
    SDL_Texture* Block[2];
    SDL_Texture* Map_Sheet;
    int Map_X, Map_Y;
};

#endif // GAME_MAP_H
