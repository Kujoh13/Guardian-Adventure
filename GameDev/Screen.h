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
    void levelSelection(SDL_Renderer* renderer);
    void checkExit(int x, int y, int &isRunning, SDL_Event event);
    void checkStart(int x, int y, int &isRunning, SDL_Event event);

protected:
    GameObject start_screen;
    GameObject start_screen_title0;
    GameObject start_screen_title1;
    GameObject start_button[2];
    GameObject exit_button[2];
    int curAlpha;
};


#endif // SCREEN_H_
