#ifndef BASIC_FUNCTION_H_
#define BASIC_FUNCTION_H_

#include <bits/stdc++.h>
#include <SDL.h>
#include <Windows.h>
#include <SDL_image.h>
#include <SDL_audio.h>
#include <SDL_ttf.h>


static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static SDL_Event gEvent;

//Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

//Map
const int TILE_SIZE = 36;
std::string int2str(int num);
bool operator == (SDL_Color a, SDL_Color b);
bool collision(SDL_Rect A, SDL_Rect B);

const SDL_Color red = {255, 0, 0, 255};
const SDL_Color blue = {0, 0, 255, 255};
const SDL_Color green = {0, 255, 0, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color white = {255, 255, 255, 255};
const SDL_Color yellow = {255, 255, 0, 255};
const SDL_Color pink = {255, 0, 255, 255};

//Character and Mob
const double gravity = 3;
const double MAX_FALL_SPEED = 25;
const double MAX_RUN_SPEED = 10;

const int numCharacter = 1;
const int charSize = 240;
const int charWidth = 40;
const int charHeight = 85;

//Projectiles
const int numProjectile = 1;
const double pi = acos(-1);


#endif
