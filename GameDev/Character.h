#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "GameObject.h"
#include "BasicFunction.h"
#include "game_map.h"
#include "Projectile.h"

class Character : public GameObject
{
public:
    Character();
    ~Character();
    void show(SDL_Renderer* renderer, int view);
    void handleInput(SDL_Event event);
    bool loadCharacter(std::string path, SDL_Renderer* renderer, int _id);
    void tick(game_map* MAP, std::vector<std::pair<SDL_Rect, int> >& rectMob, std::vector<Projectile>& vProjectile);
    void collisionX(game_map* MAP);
    void collisionY(game_map* MAP);
    void drawIdle(SDL_Renderer* renderer, int view);
    void drawMove(SDL_Renderer* renderer, int view);
    void drawAttack(SDL_Renderer* renderer, int view);
    void drawDied(SDL_Renderer* renderer, int view);
    void drawVictory(SDL_Renderer* renderer, int view);
    void setStatus(int _status);
    void setFrame(int _frame) {frame = _frame;}
    int getStatus();
    bool getAttack();
    int getId() {return id;}
    std::pair<int, int> getAttackBar();
    bool startAttack() {return nextAttack == 0;}
    bool getMove() {return (pressed['a'] || pressed['d']);}
    bool onGround() {return (!isFalling);}
    bool jumped() {return pressed[' '] && isFalling == false;}
    SDL_Rect getMelee();
    bool meleeAttack(){return !finishAttack && frame == frameAttack && type == TYPE::MELEE;}

protected:
    SDL_Texture* idleAnimation[2];
    SDL_Texture* moveAnimation[2];
    SDL_Texture* attackAnimation[2];
    SDL_Texture* diedAnimation[2];
    SDL_Texture* victoryAnimation;
    SDL_Texture* jumpAnimation[2];
    SDL_Texture* portrait;
    SDL_Rect melee;

    std::pair<int, int> _idle;
    std::pair<int, int> _move;
    std::pair<int, int> _attack;
    std::pair<int, int> _died;
    std::pair<int, int> _victory;

    int id;
    int frame;
    bool isJumping;
    bool isFalling;
    int status;
    int nStatus;
    /** Notes:
    0. idle
    1. move
    2. jump
    3. attack
    4. died
    5. victory
    **/
    enum STATUS{
        IDLE = 0,
        MOVE = 1,
        JUMP = 2,
        ATTACK = 3,
        DIED = 4,
        VICTORY = 5,
    };

    bool leftP = false, rightP = false;
    bool pressed[300] = {false};
    bool finishAttack;
    int nextAttack, framePerAttack, frameAttack;
    bool facing;
    int type;
    int idProjectile, prSpeed, prLastTime;

    enum TYPE{
        RANGED = 0,
        MELEE = 1,
    };
};

#endif // CHARACTER_H_
