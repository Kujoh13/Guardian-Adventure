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
    void render(SDL_Renderer* ren, int view);
    void setNumBlock(int _NumBlock);
    int getNumBlock(int _NumBlock);
    void setNumBush(int _NumBush);
    int getNumBush(int _NumBush);
    int info[41][301];
    int Map_X, Map_Y;
    int victory;

protected:
    std::vector<SDL_Texture*> Block;
    std::vector<SDL_Texture*> Bush;
    SDL_Surface* Map_Sheet;
    SDL_Texture* background;
    SDL_Texture* castle;
    SDL_Texture* flag;
    int Num_Block;
    int Num_Bush;

};

Uint32 getpixel(SDL_Surface *surface, int x, int y);

#endif // GAME_MAP_H
