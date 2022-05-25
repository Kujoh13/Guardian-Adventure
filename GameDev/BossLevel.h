#ifndef BOSS_LEVEL_H
#define BOSS_LEVEL_H

#include "BasicFunction.h"
#include "Character.h"
#include "GameObject.h"
#include "Projectile.h"
#include "AudioPlayer.h"
#include <SDL_ttf.h>
#include "Item.h"

class BossLevel
{
public:
    BossLevel();
    ~BossLevel();
    void tick(Character* character, std::vector<Item>& vItem);
    void show(SDL_Renderer* renderer, AudioPlayer* audio);
    void loadBoss(SDL_Renderer* renderer);
    void entrance();
    void reset();
    void attack1(SDL_Rect character);
    void attack2(SDL_Rect character);
    void attack3();
    void attack4(Character* character);
    void showBossHealthbar(SDL_Renderer* renderer);
    SDL_Rect getBossHitbox() {return Boss.getRect();}
    bool vulnerable() {return status != STATUS::MELT && finish_entrance && cnt >= 20;}
    void takeDamage(int damage) {Boss.takeDamage(damage);}

protected:
    SDL_Texture* pr[numBossProjectile];
    TTF_Font* font;
    int pr_w[numBossProjectile];
    int pr_h[numBossProjectile];
    SDL_Texture* tex[numStatus];
    SDL_Texture* range;
    GameObject healthbar;
    GameObject bossIcon;
    std::pair<int, int> fr[numStatus];
    int timer, timer1, timer2, timer3, timer4;
    int frame;
    GameObject Boss;
    int status, nStatus, curAttack;
    enum STATUS{
        WALK = 0,
        SCREAM = 1,
        IDLE = 2,
        ATTACK1 = 3,
        ATTACK2 = 4,
        ATTACK3 = 5,
        DIVE = 6,
        MELT = 7,
        DEVOUR = 8,
        RISE = 9,
        DIE = 10,
    };
    bool finish_entrance;
    SDL_Rect rRect;
    std::vector<SDL_Rect> vAttack1;
    std::vector<std::pair<GameObject, int> > vAttack2;
    std::vector<std::pair<SDL_Rect, int> > vAttack3;
    int cnt;
    std::vector<int> dropHP;
};

#endif // BOSS_LEVEL_H
