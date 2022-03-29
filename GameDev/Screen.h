#ifndef SCREEN_H_
#define SCREEN_H_

#include "BasicFunction.h"
#include "GameObject.h"

class Screen
{
public:
    Screen();
    ~Screen();
    void loadTexture(SDL_Renderer* renderer);
    void startScreen(SDL_Renderer* renderer);
    void ingame(SDL_Renderer* renderer, int char_hp, int char_dmg);
    void levelSelection(SDL_Renderer* renderer);
    void checkExit(int x, int y, int &isRunning, SDL_Event event);
    void checkStart(int x, int y, int &isRunning, SDL_Event event);

protected:
    GameObject start_screen;
    GameObject start_screen_title0;
    GameObject start_screen_title1;
    GameObject start_button[2];
    GameObject exit_button[2];
    GameObject hpIcon, dmgIcon;
    GameObject statsRect;
    TTF_Font* font;
    int curAlpha;
    int itemCount[3];
};


#endif // SCREEN_H_
