#include "BasicFunction.h"

std::string int2str(int num)
{
    if(num == 0) return "0";
    std::string res;
    while(num > 0){
        res = char(num%10 + 48) + res;
        num /= 10;
    }
    return res;
}

bool operator == (SDL_Color a, SDL_Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

bool collision(SDL_Rect A, SDL_Rect B)
{
    return !(A.x >= B.x + B.w || A.x + A.w <= B.x || A.y >= B.y + B.h || A.y + A.w <= B.y);
}
