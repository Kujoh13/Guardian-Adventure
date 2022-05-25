#ifndef HANDLER_H_
#define HANDLER_H_

#include "bits/stdc++.h"
#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"
#include "Screen.h"
#include "Character.h"
#include "Mob.h"
#include "Projectile.h"
#include "lp.h"
#include "AudioPlayer.h"
#include "BossLevel.h"

class Handler
{
public:
    Handler();
    ~Handler();
    bool loadLevel(int level, SDL_Renderer* renderer);
    void load(SDL_Renderer* renderer);
    void tick(SDL_Renderer* renderer);
    void show(SDL_Renderer* renderer);
    int get_is_running() {return isRunning;}
    void character1();

protected:
    int view;
    Character _character[numCharacter];
    std::vector<Projectile> vProjectile;
    std::vector<Mob> vMob;
    std::vector<Item> vItem, vItem_temp;
    std::vector<lp> v_lp;
    std::vector<std::pair<SDL_Rect, int> > rectMob;
    std::vector<std::pair<SDL_Rect, std::pair<int, int> > > vExplosion;
    SDL_Texture* pr[numProjectile];
    SDL_Texture* itemDrop[numItem];
    SDL_Texture* lpTexture;
    SDL_Texture* lp_Animation[3];
    SDL_Texture* explosion;
    int pr_w[numProjectile];
    int pr_h[numProjectile];
    game_map* MAP;
    int isRunning, current_level, current_character;
    Screen scr;
    SDL_Event event;
    int *character_level;
    int numGem, numCoin;
    int lastLevel;
    bool paused;
    int frame_back;
    int frame_char3;
    int id;
    int prev, prev_map, prev_char;
    int *prev_char_level;
    AudioPlayer Audio_Player;
    int **char_growth;
    BossLevel boss;
};



#endif // HANDLER_H_
