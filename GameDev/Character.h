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
    void handleInput(SDL_Event event, const Uint8 *keyboard_state_array);
    bool loadCharacter(std::string path, SDL_Renderer* renderer, int _id);
    void tick(game_map* MAP);
    void collisionX(game_map* MAP);
    void collisionY(game_map* MAP);
    void drawIdle(SDL_Renderer* renderer, int view);
    void drawMove(SDL_Renderer* renderer, int view);
    void drawAttack(SDL_Renderer* renderer);
    void drawDied(SDL_Renderer* renderer);
    void drawVictory(SDL_Renderer* renderer);
    void setStatus(int _status);
    int getStatus();

protected:
    SDL_Texture* idleAnimation;
    SDL_Texture* moveAnimation;
    SDL_Texture* attackAnimation;
    SDL_Texture* diedAnimation;
    SDL_Texture* victoryAnimation;
    SDL_Texture* jump;

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
};

#endif // CHARACTER_H_
