#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "GameObject.h"
#include "BasicFunction.h"
#include "game_map.h"

class Character : public GameObject
{
public:
    Character();
    ~Character();
    void show(SDL_Renderer* renderer, int view);
    void handleInput(SDL_Event event);
    bool loadCharacter(std::string path, SDL_Renderer* renderer, int _id);
    void tick(game_map* MAP);
    void collisionX(game_map* MAP);
    void collisionY(game_map* MAP);
    void drawIdle(SDL_Renderer* renderer, int view);
    void drawMove(SDL_Renderer* renderer, int view);
    void drawAttack(SDL_Renderer* renderer, int view);
    void drawDied(SDL_Renderer* renderer, int view);
    void drawVictory(SDL_Renderer* renderer, int view);
    void setStatus(int _status);
    int getStatus();
    bool getAttack();

protected:
    SDL_Texture* idleAnimation[2];
    SDL_Texture* moveAnimation[2];
    SDL_Texture* attackAnimation[2];
    SDL_Texture* diedAnimation[2];
    SDL_Texture* victoryAnimation;
    SDL_Texture* jump[2];

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

    bool leftP = false, rightP = false;
    bool pressed[300];
    bool finishAttack;
    int nextAttack, framePerAttack;
    bool facing;
};

#endif // CHARACTER_H_