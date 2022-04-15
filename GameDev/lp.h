#ifndef LP_H_
#define LP_H_

#include "BasicFunction.h"
#include "GameObject.h"
#include "game_map.h"

class lp : public GameObject
{
public:
    lp();
    ~lp();
    void loadVar(pii _idle_, pii _move_, pii _victory_);
    void tick(SDL_Rect character, game_map* MAP);
    void show(SDL_Renderer* renderer, int view, SDL_Texture* texture);
    void collisionY(game_map* MAP);
    bool get_done() {return done;}
    int getStatus() {return status;}
    int getType() {return type;}
    void setType(int _type) {type = _type;}

protected:
    int type;
    enum TYPE{
        FLOATING = 0,
        GROUND = 1,
    };
    int status;
    enum STATUS{
        IDLE = 0,
        MOVE = 1,
        VICTORY = 2,
    };
    int frame;
    pii _idle, _move, _victory;
    bool done;
};

#endif // LP_H_
