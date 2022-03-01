#include "game_map.h"

game_map::game_map()
{
    Map_Sheet = NULL;
    background = NULL;
    Map_X = 100;
    Map_Y = 20;
    Num_Block = 4;
    Num_Bush = 2;
    while(Block.size()) Block.pop_back();
    while(Bush.size()) Bush.pop_back();
}
game_map::~game_map()
{
    SDL_FreeSurface(Map_Sheet);
    SDL_DestroyTexture(background);
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
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
            return 0;       /* shouldn't happen, but avoids warnings */
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
    Map_Sheet = IMG_Load((path + "/level_" + int2str(level) + "/level.png").c_str());

    SDL_Texture* newTexture;

    SDL_Surface* loadedSurface = IMG_Load((path + "/level_" + int2str(level) + "/background.png").c_str());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    background = newTexture;

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


    return Map_Sheet != NULL && background != NULL;
}

void game_map::render(SDL_Renderer* ren){

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(ren, background, NULL, &rect);

    SDL_Color rgb;
    Uint32 data;

    int numGrass = 0;

    rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for(int i = 0; i < Map_Y; i++)
        for(int j = 0; j < Map_X; j++)
        {
            data = getpixel(Map_Sheet, j, i);
            SDL_GetRGB(data, Map_Sheet->format, &rgb.r, &rgb.g, &rgb.b);
            //std::cout << rect.x << ' ' << rect.y << '\n';
            if(rgb == white){
                rect.x = j * TILE_SIZE;
                rect.y = i * TILE_SIZE;
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                SDL_RenderCopy(ren, Block[2], NULL, &rect);
            }
            else if(rgb == green){
                rect.x = j * TILE_SIZE;
                rect.y = i * TILE_SIZE;
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                numGrass++;
                SDL_RenderCopy(ren, Block[numGrass%2], NULL, &rect);
            }
            else if(rgb == blue){
                rect.x = j * TILE_SIZE;
                rect.y = i * TILE_SIZE;
                rect.w = 2 * TILE_SIZE;
                rect.h = 2 * TILE_SIZE;
                SDL_RenderCopy(ren, Bush[0], NULL, &rect);
            }
            else if(rgb == red){
                rect.x = j * TILE_SIZE;
                rect.y = i * TILE_SIZE;
                rect.w = 4 * TILE_SIZE;
                rect.h = 4 * TILE_SIZE;
                SDL_RenderCopy(ren, Bush[1], NULL, &rect);
            }
        }
}
