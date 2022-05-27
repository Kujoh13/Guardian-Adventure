#include "game_map.h"

game_map::game_map()
{
    Map_Sheet = NULL;
    background = NULL;
    flag = NULL;
    castle = NULL;
    while(Block.size()) Block.pop_back();
    while(Bush.size()) Bush.pop_back();

    info = NULL;
    bush_width = NULL;
    bush_height = NULL;

    victory_w = 1300;
    victory_h = 0;
}
game_map::~game_map()
{
    SDL_FreeSurface(Map_Sheet);
    Map_Sheet = NULL;

    SDL_DestroyTexture(background);
    background = NULL;

    SDL_DestroyTexture(flag);
    flag = NULL;

    SDL_DestroyTexture(castle);
    castle = NULL;

    delete(bush_height);
    delete(bush_width);
    delete(info);

    bush_height = NULL;
    bush_width = NULL;
    info = NULL;

}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
      }
}

void game_map::setNumBlock(int _NumBlock)
{
    Num_Block = _NumBlock;
}
int game_map::getNumBlock(int _NumBlock)
{
    return Num_Block;
}
void game_map::setNumBush(int _NumBush)
{
    Num_Bush = _NumBush;
}
int game_map::getNumBush(int _NumBush)
{
    return Num_Bush;
}

bool game_map::loadMap(std::string path, SDL_Renderer* renderer, int level)
{
    std::ifstream file_map((path + "/level_" + int2str(level) + "/map_info.txt").c_str());

    file_map >> Map_Width;
    file_map >> Map_Height;
    file_map >> Num_Block;
    file_map >> Num_Bush;
    bush_height = new int [Num_Bush];
    bush_width = new int [Num_Bush];
    for(int i = 0; i < Num_Bush; i++)
    {
        file_map >> bush_width[i] >> bush_height[i];
    }

    file_map.close();

    info = new int* [Map_Height];

    for(int i = 0; i < Map_Height; i++)
        info[i] = new int [Map_Width];

    Map_Sheet = IMG_Load((path + "/level_" + int2str(level) + "/level.png").c_str());

    SDL_Texture* newTexture;

    SDL_Surface* loadedSurface = IMG_Load((path + "/level_" + int2str(level) + "/background.png").c_str());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    background = newTexture;

    loadedSurface = IMG_Load((path + "/castle.png").c_str());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    castle = newTexture;

    loadedSurface = IMG_Load((path + "/flag.png").c_str());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    flag = newTexture;

    SDL_FreeSurface(loadedSurface);

    for(int i = 0; i < Num_Block; i++){

        SDL_Surface* loadedSurface = IMG_Load((path + "/level_" + int2str(level) + "/block_" + int2str(i) + ".png").c_str());

        Block.push_back(SDL_CreateTextureFromSurface(renderer, loadedSurface));

        SDL_FreeSurface(loadedSurface);
    }

    for(int i = 0; i < Num_Bush; i++){

        SDL_Surface* loadedSurface = IMG_Load((path + "/level_" + int2str(level) + "/bush_" + int2str(i) + ".png").c_str());

        Bush.push_back(SDL_CreateTextureFromSurface(renderer, loadedSurface));

        SDL_FreeSurface(loadedSurface);
    }

    SDL_Color rgb;
    Uint32 data;

    int lp_x;

    for(int i = 0; i < Map_Height; i++)
        for(int j = 0; j < Map_Width; j++)
        {
            data = getpixel(Map_Sheet, j, i);
            SDL_GetRGB(data, Map_Sheet->format, &rgb.r, &rgb.g, &rgb.b);

            if(rgb == white || rgb == green) info[i][j] = 1;
            else
            {
                info[i][j] = 0;
                if(level < numLevel)
                {
                    if(rgb == yellow) lp_x = j * TILE_SIZE + 11 * TILE_SIZE;
                    if(rgb == pink)
                    {
                        victory_w = j * TILE_SIZE;
                        victory_h = (i + 4) * TILE_SIZE;
                    }
                }
            }
        }

    std::ifstream file((path + "/level_" + int2str(level) + "/lp_pos.txt").c_str());
    int num_lp;
    file >> num_lp;

    for(int i = 0; i < num_lp; i++)
    {
        int temp_pos;
        file >> temp_pos;
        lp_pos.push_back(temp_pos);
    }
    file.close();
    std::sort(lp_pos.begin(), lp_pos.end(), std::greater<int> ());

    if(level < numLevel)
        lp_pos.push_back(lp_x);

    return Map_Sheet != NULL && background != NULL;
}

void game_map::render(SDL_Renderer* renderer, int view, bool map_cleared, bool isBossLevel){

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(renderer, background, NULL, &rect);

    SDL_Color rgb;
    Uint32 data;

    rect = {0, 0, TILE_SIZE, TILE_SIZE};

    int numGrass = 0;

    if(!isBossLevel)
    {
        if(map_cleared == false)
            for(int i = 0; i <= victory_h / TILE_SIZE; i++)
            {
                info[i][victory_w / TILE_SIZE - 1] = 1;
                rect.x = victory_w - TILE_SIZE - view;
                rect.y = i * TILE_SIZE;
                SDL_RenderCopy(renderer, Block[Num_Block - 1], NULL, &rect);
            }
        else
            for(int i = 0; i <= victory_h / TILE_SIZE; i++)
            {
                info[i][victory_w / TILE_SIZE - 1] = 0;
            }
    }

    for(int i = 0; i < Map_Height; i++)
        for(int j = 0; j < Map_Width; j++)
        {
            data = getpixel(Map_Sheet, j, i);
            SDL_GetRGB(data, Map_Sheet->format, &rgb.r, &rgb.g, &rgb.b);

            rect.x = j * TILE_SIZE - view;
            rect.y = i * TILE_SIZE;

            if(rgb == white){
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                SDL_RenderCopy(renderer, Block[Num_Block - 1], NULL, &rect);
            }
            else if(rgb == green){
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                numGrass++;
                SDL_RenderCopy(renderer, Block[numGrass%(Num_Block - 1)], NULL, &rect);
            }
            else if(rgb == blue){
                rect.w = bush_width[0] * TILE_SIZE;
                rect.h = bush_height[0] * TILE_SIZE;
                SDL_RenderCopy(renderer, Bush[0], NULL, &rect);
            }
            else if(rgb == red){
                rect.w = bush_width[1] * TILE_SIZE;
                rect.h = bush_height[1] * TILE_SIZE;
                SDL_RenderCopy(renderer, Bush[1], NULL, &rect);
            }
            else if(rgb == cyan){
                rect.w = bush_width[2] * TILE_SIZE;
                rect.h = bush_height[2] * TILE_SIZE;
                SDL_RenderCopy(renderer, Bush[2], NULL, &rect);
            }
            else if(rgb == yellow){
                rect.w = 23 * TILE_SIZE;
                rect.h = 18 * TILE_SIZE;

                SDL_RenderCopy(renderer, castle, NULL, &rect);
            }
            else if(rgb == pink){
                rect.w = 2 * TILE_SIZE;
                rect.h = 5 * TILE_SIZE;
                SDL_RenderCopy(renderer, flag, NULL, &rect);
            }
        }
}


