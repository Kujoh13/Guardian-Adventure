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
    void ingame(SDL_Renderer* renderer, int char_hp, int char_dmg, int numCoin, int numGem, bool paused, int level_end);
    void levelSelection(SDL_Renderer* renderer, int cur_level, int cur_character, int* character_level, int numCoin, int numGem, int lastLevel, int baseHp, int baseDmg);
    void handleMouseInput(int x, int y, int &isRunning, int &cur_level, int &cur_character, int* character_level, int &numCoin, int &numGem, int lastLevel, bool &paused);
    void handleKeyInput(SDL_Event event, bool &paused, int &isRunning);
    int getFlag() {return flag;}

protected:
    GameObject background;
    GameObject start_screen_title0;
    GameObject start_screen_title1;
    GameObject start_button[2];
    GameObject exit_button[2];
    GameObject hpIcon, dmgIcon;
    GameObject hpIconStat, dmgIconStat;
    GameObject statsRect;
    GameObject option_box, bar;
    GameObject play_button;
    GameObject arrow[2];
    GameObject level_name;
    GameObject itemBox;
    GameObject charBox, infoBox;
    GameObject costBox;
    GameObject selected;
    GameObject paused_box;
    GameObject paused_return;
    GameObject paused_continue;
    GameObject win, lose;
    SDL_Texture* level_background;
    SDL_Texture* character_texture;
    SDL_Texture* gem;
    SDL_Texture* coin;
    SDL_Texture* portrait[numCharacter];
    SDL_Texture* lock;
    SDL_Texture* star;
    SDL_Texture* nostar;
    SDL_Texture* statArrow;
    TTF_Font* font;
    TTF_Font* textFont;
    TTF_Font* textFontBig;
    TTF_Font* textFontSmall;
    SDL_Rect rectBar[2];
    int curAlpha;
    int itemCount[3];
    bool flag = 0;
    bool change_level;
    bool arrow_flag, selected_flag;
    std::pair<int, int> arrow_range, selected_range;
    std::pair<int, int> fr;
    bool change_character;
    int frame = 0;
    int cw, ch;
    int ptSize;
    int prev;
    std::vector<std::string> description;
    long long *cost;
};


#endif // SCREEN_H_
