#ifndef ITEM_H_
#define ITEM_H_

#include "Character.h"
#include "BasicFunction.h"
#include "GameObject.h"

class Item : public GameObject
{

public:
    Item();
    ~Item();
    void tick(SDL_Rect character, game_map* MAP);
    void collisionX(game_map* MAP);
    void collisionY(game_map* MAP);
    void dropItem(std::vector<Item>& vItem, int numCoin, int numGem);
    void setId(int _id) {id = _id;}
    int getId() {return id;}
    void setVal(int _val) {val = _val;}
    int getVal() {return val;}
    bool getFell() {return fell;}

protected:
    bool fell;
    int id;
    enum ID{
        COIN = 0,
        GEM = 1,
        HEART = 2,
        TREASURE = 3,
    };
    int val;
};

#endif // ITEM_H_
