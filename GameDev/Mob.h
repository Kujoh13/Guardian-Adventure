#ifndef MOB_H_
#define MOB_H_

#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"
#include "Projectile.h"
#include "Character.h"
#include "Item.h"

class Mob : public GameObject
{
public:
    Mob();
    ~Mob();
    bool loadMob(std::string path, SDL_Renderer* renderer);
    void show(SDL_Renderer* renderer, int view);
    void drawIdle(SDL_Renderer* renderer, int view);
    void drawAttack(SDL_Renderer* renderer, int view);
    void drawMove(SDL_Renderer* renderer, int view);
    void collisionX(game_map* MAP);
    void collisionY(game_map* MAP);
    void tick(game_map* MAP, std::vector<Projectile> &vProjectile, Character* character);
    void setType(int _type);
    void setWeapon(int _type);
    void setRange(int l, int r);
    void spawnItem(std::vector<Item>& vItem);

    int getId();
    void setId(int _id);

    std::pair<int, int> getAttackBar();

protected:
    SDL_Texture* idleAnimation[2];
    SDL_Texture* attackAnimation[2];
    SDL_Texture* moveAnimation[2];
    SDL_Rect melee;
    int frameAttack;
    bool hostile;
    int idProjectile;
    int type;
    enum TYPE{
        RANGED = 0,
        MELEE = 1,
        THROW = 2,
        LASER = 3,
    };
    int weapon;
    bool idle, move, attack;
    int frame;
    int nextAttack;
    int framePerAttack;
    int mobId;
    bool facing, direction;
    std::pair<int, int> _idle;
    std::pair<int, int> _attack;
    std::pair<int, int> _move;
    int maxX, minX;
    int itemDrop[3];
    int prSpeed;
    int prRadius;
};

#endif // MOB_H_
