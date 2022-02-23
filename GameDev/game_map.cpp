#include "game_map.h"

game_map::game_map()
{
    Map_Sheet = NULL;
}
game_map::~game_map()
{
    SDL_DestroyTexture(Map_Sheet);
}

bool game_map::loadMap(std::string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error\n");
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface );
        if(newTexture == NULL)
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        SDL_FreeSurface(loadedSurface);
    }

    Map_Sheet = newTexture;

    return Map_Sheet != NULL;
}

void game_map::render(SDL_Renderer* ren){
    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for(int i = 0; i < Map_X; i++)
        for(int j = 0; j < Map_Y; j++)
        {
            if()
            rect.x = i * TILE_SIZE;
            rect.y = j * TILE_SIZE;
            SDL_RenderCopy()
        }
}
