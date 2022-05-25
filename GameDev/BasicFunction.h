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

//Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//function
std::string int2str(int num);
bool operator == (SDL_Color a, SDL_Color b);
bool collision(SDL_Rect A, SDL_Rect B);
bool inRect(int x, int y, SDL_Rect rect);
double distance(SDL_Rect A, SDL_Rect B);
long long Rand(long long l, long long r);
long long getCost(int level);

//Map
const int TILE_SIZE = 36;

const SDL_Color red = {255, 0, 0, 255};
const SDL_Color blue = {0, 0, 255, 255};
const SDL_Color green = {0, 255, 0, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color white = {255, 255, 255, 255};
const SDL_Color yellow = {255, 255, 0, 255};
const SDL_Color pink = {255, 0, 255, 255};
const SDL_Color cyan = {0, 255, 255, 255};
const SDL_Color textColor = {245, 220, 189, 255};
const SDL_Color backColor = {45, 38, 32, 255};

//Character and Mob
const double gravity = 3;
const double MAX_FALL_SPEED = 25;
const double MAX_RUN_SPEED = 15;

const int numCharacter = 6;
const int numItem = 4;
const int charSize = 300;
const int portraitSize = 100;
const int bossSize = 900;
const int numStatus = 11;
const int numBossProjectile = 2;

const int numLevel = 4;

//Projectiles
const int numProjectile = 7;
const double pi = acos(-1);

#endif
