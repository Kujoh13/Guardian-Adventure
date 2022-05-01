#include "Screen.h"


Screen::Screen()
{
    level_background = NULL;
    character_texture = NULL;
    gem = NULL;
    coin = NULL;
    for(int i = 0; i < numCharacter; i++)
        portrait[i] = NULL;
    curAlpha = 0;
    change_level = true;
    change_character = true;
    arrow_flag = true;
    selected_flag = true;
    flag = false;
    font = NULL;
    for(int i = 0; i < 3; i++)
        itemCount[i] = 0;
    frame = 0;
    ptSize = 150;
    cost = new long long[numCharacter];
    for(int i = 0; i < numCharacter; i++)
        cost[i] = 20;

    prev = 0;
}

Screen::~Screen()
{
    for(int i = 0; i < numCharacter; i++)
        SDL_DestroyTexture(portrait[i]);
    SDL_DestroyTexture(level_background);
    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(gem);
    SDL_DestroyTexture(coin);
}
void Screen::loadTexture(SDL_Renderer* renderer)
{

    background.loadImage("img/background.png", renderer);
    start_screen_title0.loadImage("img/guardian.png", renderer);
    start_screen_title1.loadImage("img/adventure.png", renderer);
    start_button[0].loadImage("img/start_0.png", renderer);
    start_button[1].loadImage("img/start_1.png", renderer);
    exit_button[0].loadImage("img/exit_0.png", renderer);
    exit_button[1].loadImage("img/exit_1.png", renderer);
    hpIcon.loadImage("img/hpIcon.png", renderer);
    dmgIcon.loadImage("img/dmgIcon.png", renderer);
    statsRect.loadImage("img/statsRect.png", renderer);
    option_box.loadImage("img/optionbox.png", renderer);
    bar.loadImage("img/bar.png", renderer);
    arrow[0].loadImage("img/arrow0.png", renderer);
    arrow[1].loadImage("img/arrow1.png", renderer);
    play_button.loadImage("img/play_button.png", renderer);
    itemBox.loadImage("img/itembox.png", renderer);
    charBox.loadImage("img/charBox.png", renderer);
    infoBox.loadImage("img/infoBox.png", renderer);
    costBox.loadImage("img/costBox.png", renderer);
    selected.loadImage("img/selected.png", renderer);
    paused_box.loadImage("img/paused.png", renderer);
    paused_return.loadImage("img/main_menu.png", renderer);
    paused_continue.loadImage("img/continue.png", renderer);
    win.loadImage("img/win.png", renderer);
    lose.loadImage("img/lose.png", renderer);

    hpIconStat = hpIcon;
    dmgIconStat = dmgIcon;

    for(int i = 0; i < numCharacter; i++)
    {
        SDL_Surface* sf = IMG_Load(("Character/" + int2str(i) + "/portrait.png").c_str());
        portrait[i] = SDL_CreateTextureFromSurface(renderer, sf);
        SDL_FreeSurface(sf);
    }

    SDL_Surface* sf = IMG_Load("img/item0.png");
    coin = SDL_CreateTextureFromSurface(renderer, sf);

    sf = IMG_Load("img/item1.png");
    gem = SDL_CreateTextureFromSurface(renderer, sf);

    sf = IMG_Load("img/lock.png");
    lock = SDL_CreateTextureFromSurface(renderer, sf);

    sf = IMG_Load("img/star.png");
    star = SDL_CreateTextureFromSurface(renderer, sf);

    sf = IMG_Load("img/nostar.png");
    nostar = SDL_CreateTextureFromSurface(renderer, sf);

    sf = IMG_Load("img/statArrow.png");
    statArrow = SDL_CreateTextureFromSurface(renderer, sf);

    font = TTF_OpenFont("REVUE.ttf", 45);

    textFont = TTF_OpenFont("calibrib.ttf", 25);

    textFontBig = TTF_OpenFont("calibrib.ttf", 35);

    textFontSmall = TTF_OpenFont("calibrib.ttf", 20);

    start_screen_title0.setX(120);
    start_screen_title0.setY(-200);
    start_screen_title1.setX(15);
    start_screen_title1.setY(721);
    start_screen_title0.setVelY(13);
    start_screen_title1.setVelY(-20);

    start_button[0].setX(460);
    start_button[0].setY(380);
    start_button[1].setX(450);
    start_button[1].setY(370);

    exit_button[0].setX(460);
    exit_button[0].setY(550);
    exit_button[1].setX(450);
    exit_button[1].setY(542);

    hpIcon.setX(15 + portraitSize);
    hpIcon.setY(20);

    dmgIcon.setX(15 + portraitSize);
    dmgIcon.setY(70);

    option_box.setX((SCREEN_WIDTH - option_box.getW()) / 2);
    option_box.setY(30);

    bar.setX(option_box.getX() + 5);
    bar.setY(35);

    rectBar[0] = bar.getRect();
    rectBar[1] = bar.getRect();
    rectBar[1].x += bar.getW() + 5;

    arrow[0].setX(SCREEN_WIDTH / 4 - arrow[0].getW() - 40);
    arrow[0].setY((SCREEN_HEIGHT - arrow[0].getH()) / 2);

    arrow_range = {arrow[0].getX(), arrow[0].getX() + 40};

    arrow[1].setX(3 * SCREEN_WIDTH / 4 + 40);
    arrow[1].setY((SCREEN_HEIGHT - arrow[1].getH()) / 2);

    play_button.setX((SCREEN_WIDTH - play_button.getW()) / 2);
    play_button.setY(3 * SCREEN_HEIGHT / 4 + 75);

    itemBox.setX(10);
    itemBox.setY(10);

    charBox.setX(10);
    charBox.setY(itemBox.getY() + 20 + 2 * itemBox.getH());

    infoBox.setX(20 + charBox.getW() + charBox.getX());
    infoBox.setY(charBox.getY());

    costBox.setX(infoBox.getX() + (infoBox.getW() - costBox.getW()) / 2);
    costBox.setY(infoBox.getY() + infoBox.getH() - costBox.getH() - 20);

    selected.setH(150);
    selected.setW(150);

    selected_range = {150, 170};

    paused_box.setX((SCREEN_WIDTH - paused_box.getW()) / 2);
    paused_box.setY((SCREEN_HEIGHT - paused_box.getH()) / 2);

    paused_return.setX(paused_box.getX() + (paused_box.getW() - paused_return.getW()) / 2);
    paused_return.setY(paused_box.getY() + paused_box.getH() - paused_return.getH() - 10);

    paused_continue.setX(paused_return.getX());
    paused_continue.setY(paused_return.getY() - paused_continue.getH() - 10);

    win.setX((SCREEN_WIDTH - win.getW()) / 2);
    win.setY((SCREEN_HEIGHT / 4));

    lose.setX((SCREEN_WIDTH - lose.getW()) / 2);
    lose.setY((SCREEN_HEIGHT / 4));

    dmgIconStat.setX(infoBox.getX() + 20);
    dmgIconStat.setY(costBox.getY() - 20 - dmgIconStat.getH());

    hpIconStat.setX(dmgIconStat.getX());
    hpIconStat.setY(dmgIconStat.getY() - 10 - hpIconStat.getH());

    SDL_FreeSurface(sf);


}

void Screen::startScreen(SDL_Renderer* renderer)
{
    background.render(renderer, 0);

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

        if(inRect(x, y, start_button[0].getRect()))
            start_button[1].render(renderer, 0);

        else
            start_button[0].render(renderer, 0);
    }

    if(curAlpha == 255){

        if(inRect(x, y, exit_button[0].getRect()))
            exit_button[1].render(renderer, 0);

        else
            exit_button[0].render(renderer, 0);
    }


    if(curAlpha < 255 && start_screen_title1.getY() == 180) curAlpha += 17;

}

void Screen::handleMouseInput(int x, int y, int &isRunning, int &cur_level, int &cur_character, int* character_level, int &numCoin, int &numGem, int lastLevel, bool &paused)
{
    if(isRunning == 1)
    {
        if(curAlpha == 255)
        {
            if(inRect(x, y, exit_button[0].getRect()))
                isRunning = 0;
            if(inRect(x, y, start_button[0].getRect()))
                isRunning = 2;
        }
    }
    else if(isRunning == 2)
    {
        if(flag && inRect(x, y, rectBar[0]))
            flag = 0;
        else if(!flag && inRect(x, y, rectBar[1]))
            flag = 1;

        if(!flag)
        {
            if(character_level[cur_character] == 0)
            {
                cur_character = prev;
                change_character = true;
            }
            if(inRect(x, y, arrow[0].getRect()) && cur_level > 1)
            {
                cur_level--;
                change_level = true;
            }
            else if(inRect(x, y, arrow[1].getRect()) && cur_level < numLevel)
            {
                cur_level++;
                change_level = true;
            }

            if(inRect(x, y, play_button.getRect()))
            {
                if(cur_level <= lastLevel)
                    isRunning = 3;
            }
        }
        else
        {
            int col = 5;
            int row = (numCharacter - 1) / 5;
            int dis = 20;
            int ptSize = 150;
            for(int i = 0; i <= row; i++)
                for(int j = 0; j < 5; j++)
                {
                    int id = i * 5 + j;
                    if(id >= numCharacter) break;
                    SDL_Rect nRect = {0, 0, ptSize, ptSize};
                    nRect.x = charBox.getX() + dis + j * (dis + ptSize);
                    nRect.y = charBox.getY() + dis + i * (dis + ptSize);
                    if(inRect(x, y, nRect))
                    {
                        if(character_level[id]  > 0)
                            prev = id;
                        if(id != cur_character){
                            change_character = true;
                        }
                        cur_character = id;
                    }
                }
            if(inRect(x, y, costBox.getRect()))
            {
                if(character_level[cur_character] == 0 && numGem >= cost[cur_character])
                {
                    numGem -= cost[cur_character];
                    character_level[cur_character]++;
                }
                else if(character_level[cur_character] > 0 && numCoin >= cost[cur_character])
                {
                    numCoin -= cost[cur_character];
                    character_level[cur_character]++;
                }
            }
        }
    }
    else if(isRunning == 3)
    {
        if(paused)
        {
            if(inRect(x, y, paused_continue.getRect()))
                paused = false;
            if(inRect(x, y, paused_return.getRect()))
            {
                paused = false;
                isRunning = 2;
            }
        }
    }
}

void Screen::handleKeyInput(SDL_Event event, bool &paused, int &isRunning)
{
    switch(event.key.keysym.sym)
    {
    case SDLK_p:
        {
            paused = 1 - paused;
        }
        break;
    case SDLK_ESCAPE:
        {
            if(isRunning == 1)
                isRunning = 0;
            else if(isRunning == 2)
                isRunning = 1;
            else paused = 1 - paused;
        }

    }
}

void Screen::levelSelection(SDL_Renderer* renderer, int cur_level, int cur_character, int* character_level, int numCoin, int numGem, int lastLevel, int baseHp, int baseDmg)
{
    ///arrow
    if(arrow_flag == 0)
    {
        arrow[0].setX(arrow[0].getX() - 5);
        arrow[1].setX(arrow[1].getX() + 5);
    }
    else
    {
        arrow[0].setX(arrow[0].getX() + 5);
        arrow[1].setX(arrow[1].getX() - 5);
    }
    if(arrow[0].getX() == arrow_range.first)
        arrow_flag = 1;
    else if(arrow[0].getX() == arrow_range.second)
        arrow_flag = 0;

    ///upper bar
    if(flag == 1 && bar.getX() < rectBar[1].x)
        bar.setX(std::min(bar.getX() + 75, rectBar[1].x));
    else if(flag == 0 && bar.getX() > rectBar[0].x)
        bar.setX(std::max(bar.getX() - 75, rectBar[0].x));

    background.render(renderer, 0);
    option_box.render(renderer, 0);
    bar.render(renderer, 0);

    std::string s = "Level Selection";
    SDL_Surface* sf_font = NULL;
    if(flag == 0) sf_font = TTF_RenderText_Solid(textFont, s.c_str(), backColor);
    else sf_font = TTF_RenderText_Solid(textFont, s.c_str(), textColor);
    int w = sf_font->w, h = sf_font->h;
    SDL_Texture* nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);

    SDL_Rect nRect = {0, 0, w, h};
    nRect.x = rectBar[0].x + (rectBar[0].w - w) / 2;
    nRect.y = rectBar[0].y + (rectBar[0].h - h) / 2;

    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

    s = "Characters";

    if(flag == 1) sf_font = TTF_RenderText_Solid(textFont, s.c_str(), backColor);
    else sf_font = TTF_RenderText_Solid(textFont, s.c_str(), textColor);

    w = sf_font->w, h = sf_font->h;
    nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);

    nRect = {0, 0, w, h};
    nRect.x = rectBar[1].x + (rectBar[1].w - w) / 2;
    nRect.y = rectBar[1].y + (rectBar[1].h - h) / 2;

    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

    ///Gem and Coin box
    itemBox.render(renderer, 0);

    nRect = itemBox.getRect();
    nRect.y += itemBox.getH() + 5;
    SDL_RenderCopy(renderer, itemBox.getObject(), NULL, &nRect);

    nRect.x += 5;
    nRect.y += 5;
    nRect.w = nRect.h = 30;
    SDL_RenderCopy(renderer, gem, NULL, &nRect);

    nRect.y = itemBox.getY() + 5;
    SDL_RenderCopy(renderer, coin, NULL, &nRect);

        ///Coin text
    sf_font = TTF_RenderText_Solid(textFont, int2str(numCoin).c_str(), white);
    w = sf_font->w, h = sf_font->h;
    nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);
    nRect.x = itemBox.getX() + itemBox.getW() - 10 - w;
    nRect.y = itemBox.getY() + 10;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

        ///Gem text
    sf_font = TTF_RenderText_Solid(textFont, int2str(numGem).c_str(), white);
    w = sf_font->w, h = sf_font->h;
    nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);
    nRect.x = itemBox.getX() + itemBox.getW() - 10 - w;
    nRect.y = itemBox.getY() + 10 + itemBox.getH() + 5;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

    ///load Character
    if(change_character){ /// load Character in Level Selection
        description.clear();
        frame = 0;
        std::string path = "Character/" + int2str(cur_character);
        SDL_Surface* sf = IMG_Load((path + "/idle0.png").c_str());
        character_texture = SDL_CreateTextureFromSurface(renderer, sf);
        change_character = false;
        std::ifstream file((path + "/char_info.txt").c_str());
        file >> cw >> ch;
        file >> fr.first;
        file >> fr.first >> fr.second;
        file.close();

        std::ifstream file_description((path + "/description.txt").c_str());
        std::string s;
        while(std::getline(file_description, s))
            description.push_back(s);

        file_description.close();

        SDL_FreeSurface(sf);
    }
    SDL_Rect fRect = {0, 0, charSize, charSize};
    fRect.x = (frame % fr.first) * charSize;
    fRect.y = (frame / fr.first) * charSize;

    if(flag == 0)
    {
        if(change_level)
        {
            std::string path = "img/Level_" + int2str(cur_level) + "/background.png";
            SDL_Surface* sf = IMG_Load(path.c_str());
            level_background = SDL_CreateTextureFromSurface(renderer, sf);

            path = "img/Level_" + int2str(cur_level) + "/name.png";
            level_name.loadImage(path, renderer);
            level_name.setX(SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - level_name.getW()) / 2);
            level_name.setY(SCREEN_HEIGHT / 4 + 10);

            SDL_FreeSurface(sf);
            change_level = false;
        }

        SDL_Rect nRect, tRect;
        nRect.x = SCREEN_WIDTH / 4;
        nRect.y = SCREEN_HEIGHT / 4;
        nRect.w = SCREEN_WIDTH / 2;
        nRect.h = SCREEN_HEIGHT / 2;
        tRect = nRect;
        tRect.x -= 4;
        tRect.y -= 4;
        tRect.w += 8;
        tRect.h += 8;
        SDL_SetRenderDrawColor(renderer, 120, 120, 0, 255);
        SDL_RenderFillRect(renderer, &tRect);

        SDL_RenderCopy(renderer, level_background, NULL, &nRect);

        nRect.x = nRect.x + nRect.w - 40;
        nRect.y = nRect.y + 10;
        nRect.w = nRect.h = 30;
        if(cur_level < lastLevel)
            SDL_RenderCopy(renderer, star, NULL, &nRect);
        else if(cur_level > lastLevel)
            SDL_RenderCopy(renderer, lock, NULL, &nRect);
        else SDL_RenderCopy(renderer, nostar, NULL, &nRect);

        level_name.render(renderer, 0);

        if(cur_level > 1) arrow[0].render(renderer, 0);
        if(cur_level < numLevel) arrow[1].render(renderer, 0);

        if(cur_level <= lastLevel)
            play_button.render(renderer, 0);

        SDL_Rect cRect = {0, 0, charSize, charSize};
        cRect.x = (SCREEN_WIDTH - charSize) / 2 + (charSize / 3 - cw) / 2;
        cRect.y = (SCREEN_HEIGHT - charSize) / 2;
        SDL_RenderCopy(renderer, character_texture, &fRect, &cRect);
        frame++;
        while(frame >= fr.second) frame -= fr.second;
    }
    else
    {
        if(selected_flag == 0)
        {
            selected.setW(selected.getW() - 4);
            selected.setH(selected.getH() - 4);
        }
        else
        {
            selected.setW(selected.getW() + 4);
            selected.setH(selected.getH() + 4);
        }
        if(selected.getW() == selected_range.first)
            selected_flag = 1;
        else if(selected.getW() == selected_range.second)
            selected_flag = 0;

        charBox.render(renderer, 0);
        infoBox.render(renderer, 0);
        costBox.render(renderer, 0);
        int col = 5;
        int row = (numCharacter - 1) / 5;
        int dis = 20;
        ///draw charBox
        for(int i = 0; i <= row; i++)
            for(int j = 0; j < 5; j++)
            {
                int id = i * 5 + j;
                if(id >= numCharacter) break;
                SDL_Rect nRect = {0, 0, ptSize, ptSize};
                nRect.x = charBox.getX() + dis + j * (dis + ptSize);
                nRect.y = charBox.getY() + dis + i * (dis + ptSize);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if(id == cur_character)
                    SDL_RenderFillRect(renderer, &nRect);
                SDL_RenderDrawRect(renderer, &nRect);
                SDL_RenderCopy(renderer, portrait[id], NULL, &nRect);

                SDL_Rect nnRect = nRect;
                SDL_RenderDrawRect(renderer, &nnRect);

                nnRect.x = nnRect.x + nnRect.w - 40;
                nnRect.y = nnRect.y + 10;
                nnRect.w = nnRect.h = 30;
                if(character_level[id] == 0)
                    SDL_RenderCopy(renderer, lock, NULL, &nnRect);

                if(id == cur_character)
                {
                    selected.setX(nRect.x - (selected.getW() - nRect.w) / 2);
                    selected.setY(nRect.y - (selected.getH() - nRect.h) / 2);
                }
            }
        ///draw infoBox
        SDL_Rect cRect = {0, 0, charSize, charSize};
        cRect.x = infoBox.getX() + (infoBox.getW() - charSize) / 2 + (charSize / 3 - cw) / 2;
        cRect.y = infoBox.getY() - 80;
        SDL_RenderCopy(renderer, character_texture, &fRect, &cRect);

        SDL_Surface* sf = NULL;
        SDL_Texture* tt = NULL;

        ///draw Stat
        hpIconStat.render(renderer, 0);
        dmgIconStat.render(renderer, 0);

        int newHp = baseHp, newDmg = baseDmg;
        if(character_level[cur_character] > 0)
        {
            newHp += (character_level[cur_character] - 1) * 2;
            newDmg += character_level[cur_character] - 1;
        }

        SDL_Rect hpRect, dmgRect;
        sf = TTF_RenderText_Solid(textFont, int2str(newDmg).c_str(), textColor);
        tt = SDL_CreateTextureFromSurface(renderer, sf);

        dmgRect = {0, 0, sf->w ,sf->h};
        dmgRect.x = dmgIconStat.getX() + dmgIconStat.getW() + 10;
        dmgRect.y = dmgIconStat.getY() + (dmgIconStat.getH() - sf->h) / 2;
        SDL_RenderCopy(renderer, tt, NULL, &dmgRect);

        if(character_level[cur_character] > 0)
        {
            sf = TTF_RenderText_Solid(textFontBig, int2str(newDmg + 1).c_str(), green);
            tt = SDL_CreateTextureFromSurface(renderer, sf);
            dmgRect.w = sf->w;
            dmgRect.h = sf->h;
            dmgRect.x = 60 + infoBox.getX() + infoBox.getW() / 2;
            dmgRect.y -= 5;
            SDL_RenderCopy(renderer, tt, NULL, &dmgRect);
        }

        sf = TTF_RenderText_Solid(textFont, int2str(newHp).c_str(), textColor);
        tt = SDL_CreateTextureFromSurface(renderer, sf);

        hpRect = {0, 0, sf->w ,sf->h};
        hpRect.x = hpIconStat.getX() + hpIconStat.getW() + 10;
        hpRect.y = hpIconStat.getY() + (hpIconStat.getH() - sf->h) / 2;
        SDL_RenderCopy(renderer, tt, NULL, &hpRect);

        if(character_level[cur_character] > 0)
        {
            sf = TTF_RenderText_Solid(textFontBig, int2str(newHp + 2).c_str(), green);
            tt = SDL_CreateTextureFromSurface(renderer, sf);
            hpRect.w = sf->w;
            hpRect.h = sf->h;
            hpRect.x = 60 + infoBox.getX() + infoBox.getW() / 2;
            hpRect.y -= 5;
            SDL_RenderCopy(renderer, tt, NULL, &hpRect);
        }

        ///draw Level
        sf = TTF_RenderText_Solid(textFont, ("Lv. " + int2str(character_level[cur_character])).c_str(), textColor);
        tt =  SDL_CreateTextureFromSurface(renderer, sf);
        SDL_Rect lvRect = {0, 0, sf->w, sf->h};
        lvRect.x = hpIconStat.getX() + hpIconStat.getW() + 10;
        lvRect.y = hpIconStat.getY() - lvRect.h - 10;
        SDL_RenderCopy(renderer, tt, NULL, &lvRect);

        if(character_level[cur_character] > 0)
        {
            sf = TTF_RenderText_Solid(textFontBig, ("Lv. " + int2str(character_level[cur_character] + 1)).c_str(), green);
            tt = SDL_CreateTextureFromSurface(renderer, sf);
            lvRect.w = sf->w;
            lvRect.h = sf->h;
            lvRect.x = 60 + infoBox.getX() + infoBox.getW() / 2;
            lvRect.y -= 5;
            SDL_RenderCopy(renderer, tt, NULL, &lvRect);
        }

        if(character_level[cur_character] > 0)
        {
            SDL_Rect aRect = {0, 0, 25, 23};

            aRect.x = dmgIconStat.getX() + 150;
            aRect.y = dmgIconStat.getY() + (hpIconStat.getH() - aRect.h) / 2;
            SDL_RenderCopy(renderer, statArrow, NULL, &aRect);

            aRect.x = hpIconStat.getX() + 150;
            aRect.y = hpIconStat.getY() + (hpIconStat.getH() - aRect.h) / 2;
            SDL_RenderCopy(renderer, statArrow, NULL, &aRect);

            aRect.y -= 20 + aRect.h;
            SDL_RenderCopy(renderer, statArrow, NULL, &aRect);
        }


        ///Draw Description

        SDL_Rect dRect;
        dRect.x = infoBox.getX() + 10;
        dRect.y = infoBox.getY() + charSize - 170;

        for(int i = 0; i < description.size(); i++)
        {
            sf = TTF_RenderText_Solid(textFontSmall, description[i].c_str(), textColor);
            tt = SDL_CreateTextureFromSurface(renderer, sf);

            dRect.w = sf->w;
            dRect.h = sf->h;

            SDL_RenderCopy(renderer, tt, NULL, &dRect);

            dRect.y += 25;
        }

        ///draw Cost
        if(character_level[cur_character] > 0)
            cost[cur_character] = getCost(character_level[cur_character]);

        std::string temp = "Unlock";
        if(character_level[cur_character] > 0) temp = "Upgrade";

        sf = TTF_RenderText_Solid(textFont, temp.c_str(), black);
        tt = SDL_CreateTextureFromSurface(renderer, sf);

        lvRect = {0, 0, sf->w, sf->h};
        lvRect.x = costBox.getX() + 40;
        lvRect.y = costBox.getY() + (costBox.getH() - sf->h) / 2;
        SDL_RenderCopy(renderer, tt, NULL, &lvRect);

        lvRect.x += 30 + sf->w;
        lvRect.y = costBox.getY() + (costBox.getH() - 30) / 2;
        lvRect.w = lvRect.h = 30;
        if(character_level[cur_character] > 0)
            SDL_RenderCopy(renderer, coin, NULL, &lvRect);
        else
            SDL_RenderCopy(renderer, gem, NULL, &lvRect);

        if(character_level[cur_character] > 0 && numCoin >= cost[cur_character])
            sf = TTF_RenderText_Solid(textFont, int2str(cost[cur_character]).c_str(), black);
        else if(character_level[cur_character] == 0 && numGem >= cost[cur_character])
            sf = TTF_RenderText_Solid(textFont, int2str(cost[cur_character]).c_str(), black);
        else
            sf = TTF_RenderText_Solid(textFont, int2str(cost[cur_character]).c_str(), red);

        tt = tt = SDL_CreateTextureFromSurface(renderer, sf);

        lvRect.w = sf->w;
        lvRect.h = sf->h;
        lvRect.x += 10 + 30;
        lvRect.y = costBox.getY() + (costBox.getH() - sf->h) / 2;
        SDL_RenderCopy(renderer, tt, NULL, &lvRect);

        frame++;
        while(frame >= fr.second) frame -= fr.second;
        selected.render(renderer, 0);

        SDL_FreeSurface(sf);
        SDL_DestroyTexture(tt);
    }

}

void Screen::ingame(SDL_Renderer* renderer, int char_hp, int char_dmg, int numCoin, int numGem, bool paused, int level_end)
{
    statsRect.render(renderer, 0);
    hpIcon.render(renderer, 0);
    dmgIcon.render(renderer, 0);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, int2str(char_hp).c_str(), white);

    SDL_Texture* nTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect temp = {160, 15, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, nTexture, NULL, &temp);

    textSurface = TTF_RenderText_Solid(font, int2str(char_dmg).c_str(), white);

    nTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    temp = {160, 65, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, nTexture, NULL, &temp);

    SDL_FreeSurface(textSurface);

    SDL_DestroyTexture(nTexture);

    ///Gem and Coin box

    SDL_Rect nRect = itemBox.getRect();
    nRect.x = SCREEN_WIDTH / 2;
    nRect.y = 5;
    SDL_RenderCopy(renderer, itemBox.getObject(), NULL, &nRect);

    nRect.x += itemBox.getW() + 10;
    SDL_RenderCopy(renderer, itemBox.getObject(), NULL, &nRect);

    nRect.x += 5;
    nRect.y += 5;
    nRect.w = nRect.h = 30;
    SDL_RenderCopy(renderer, gem, NULL, &nRect);

    nRect.x = SCREEN_WIDTH / 2 + 5;
    SDL_RenderCopy(renderer, coin, NULL, &nRect);

        ///Coin text
    SDL_Surface* sf_font = TTF_RenderText_Solid(textFont, int2str(numCoin).c_str(), white);
    int w = sf_font->w, h = sf_font->h;
    nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);
    nRect.x = SCREEN_WIDTH / 2 + itemBox.getW() - 10 - w;
    nRect.y = 15;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

        ///Gem text
    sf_font = TTF_RenderText_Solid(textFont, int2str(numGem).c_str(), white);
    w = sf_font->w, h = sf_font->h;
    nTexture = SDL_CreateTextureFromSurface(renderer, sf_font);
    nRect.x = SCREEN_WIDTH / 2 + 2 * itemBox.getW() - w;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, nTexture, NULL, &nRect);

    if(paused == true)
    {
        paused_box.render(renderer, 0);
        paused_return.render(renderer, 0);
        paused_continue.render(renderer, 0);
    }

    if(level_end == 2) win.render(renderer, 0);
    else if(level_end == 1) lose.render(renderer, 0);
}
