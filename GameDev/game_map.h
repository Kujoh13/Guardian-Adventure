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
    void render(SDL_Renderer* renderer, int view, bool map_cleared, bool isBossLevel);
    void setNumBlock(int _NumBlock);
    int getNumBlock(int _NumBlock);
    void setNumDecor(int _NumDecor);
    int getNumDecor(int _NumDecor);

    void setNumCoin(int _numCoin) {numCoin = _numCoin;}
    int getNumCoin() {return numCoin;}

    void setNumGem(int _numGem) {numGem = _numGem;}
    int getNumGem() {return numGem;}

    int **getInfo() {return info;}
    int getVictory() {return victory_w;}
    int getMapWidth() {return Map_Width;}
    int getMapHeight() {return Map_Height;}
    std::vector<int> get_lp_pos() {return lp_pos;}
    void pop() {lp_pos.pop_back();}

protected:
    std::vector<SDL_Texture*> Block;
    std::vector<SDL_Texture*> Decor;
    SDL_Surface* Map_Sheet;
    SDL_Texture* background;
    SDL_Texture* castle;
    SDL_Texture* flag;
    int Num_Block;
    int Num_Decor;
    std::vector<int> lp_pos;
    int Map_Width, Map_Height;
    int victory_w, victory_h;
    int** info;
    int* decor_width;
    int* decor_height;
    int numCoin, numGem;
};

Uint32 getpixel(SDL_Surface *surface, int x, int y);

#endif // GAME_MAP_H
