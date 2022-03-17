#include "Screen.h"


Screen::Screen()
{
    curAlpha = 0;
}

Screen::~Screen()
{
    start_screen.Free();
    start_screen_title0.Free();
    start_screen_title1.Free();
    start_button[0].Free();
    start_button[1].Free();
    exit_button[0].Free();
    exit_button[1].Free();
}
void Screen::loadTexture(SDL_Renderer* renderer)
{
    start_screen.loadImage("img/background.png", renderer);
    start_screen_title0.loadImage("img/guardian.png", renderer);
    start_screen_title1.loadImage("img/adventure.png", renderer);
    start_button[0].loadImage("img/start_0.png", renderer);
    start_button[1].loadImage("img/start_1.png", renderer);
    exit_button[0].loadImage("img/exit_0.png", renderer);
    exit_button[1].loadImage("img/exit_1.png", renderer);

    start_screen_title0.setX(120);
    start_screen_title0.setY(-200);
    start_screen_title1.setX(15);
    start_screen_title1.setY(721);
    start_screen_title0.setVelY(13);
    start_screen_title1.setVelY(-20 );

    start_button[0].setX(460);
    start_button[0].setY(380);
    start_button[1].setX(450);
    start_button[1].setY(370);

    exit_button[0].setX(460);
    exit_button[0].setY(550);
    exit_button[1].setX(450);
    exit_button[1].setY(542);

}

void Screen::startScreen(SDL_Renderer* renderer)
{
    start_screen.render(renderer, 0);

    if(start_screen_title0.getY() < 5)
        start_screen_title0.setY(std::min(5, start_screen_title0.getY() +
                                     start_screen_title0.getVelY()));

    if(start_screen_title1.getY() > 180 && start_screen_title0.getY() == 5)
        start_screen_title1.setY(std::max(180, start_screen_title1.getY() +
                                     start_screen_title1.getVelY()));

    start_screen_title0.render(renderer, 0);

    start_screen_title1.render(renderer, 0);


    int x, y;

    SDL_GetMouseState(&x, &y);

    if(curAlpha == 255){

        if(x >= start_button[0].getX() &&
           y >= start_button[0].getY() &&
           x <= start_button[0].getX() + start_button[0].getW() &&
           y <= start_button[0].getY() + start_button[0].getH())
            start_button[1].render(renderer, 0);

        else
            start_button[0].render(renderer, 0);
    }

    if(curAlpha == 255){

        if(x >= exit_button[0].getX() &&
           y >= exit_button[0].getY() &&
           x <= exit_button[0].getX() + exit_button[0].getW() &&
           y <= exit_button[0].getY() + exit_button[0].getH())
            exit_button[1].render(renderer, 0);

        else
            exit_button[0].render(renderer, 0);
    }


    if(curAlpha < 255 && start_screen_title1.getY() == 180) curAlpha += 17;

}

void Screen::checkExit(int x, int y, int &isRunning, SDL_Event event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    if(curAlpha == 255)
    {
        if(x >= exit_button[0].getX() &&
           y >= exit_button[0].getY() &&
           x <= exit_button[0].getX() + exit_button[0].getW() &&
           y <= exit_button[0].getY() + exit_button[0].getH())
            isRunning = 0;
    }
}

void Screen::checkStart(int x, int y, int &isRunning, SDL_Event event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    if(curAlpha == 255)
    {
        //std::cout << "Start\n";
        if(x >= start_button[0].getX() &&
           y >= start_button[0].getY() &&
           x <= start_button[0].getX() + start_button[0].getW() &&
           y <= start_button[0].getY() + start_button[0].getH())
        isRunning = 3;
    }
}

void Screen::levelSelection(SDL_Renderer* renderer)
{

}
