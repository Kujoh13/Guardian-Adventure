#ifndef BASIC_FUNCTION_H_
#define BASIC_FUNCTION_H_

#include <bits/stdc++.h>
#include <SDL.h>
#include <Windows.h>
#include <SDL_image.h>
#include <SDL_audio.h>
#include <SDL_ttf.h>

typedef std::pair<int, int> pii;

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static SDL_Event gEvent;

//Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

//function
std::string int2str(int num);
bool operator == (SDL_Color a, SDL_Color b);
bool collision(SDL_Rect A, SDL_Rect B);
long long Rand(long long l, long long r);

//Map
const int TILE_SIZE = 36;

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

const int numCharacter = 2;
const int numItem = 4;
const int charSize = 300;
const int charWidth = 40;
const int charHeight = 85;
const int portraitSize = 100;

//Projectiles
const int numProjectile = 2;
const double pi = acos(-1);


#endif
