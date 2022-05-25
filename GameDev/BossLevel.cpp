#include "BossLevel.h"

BossLevel::BossLevel()
{
    timer = 0;
    timer1 = timer2 = timer3 = timer4 = 0;
    Boss.setW(270);
    Boss.setH(350);
    Boss.setX(1580);
    Boss.setY(SCREEN_HEIGHT - TILE_SIZE - Boss.getH());
    frame = 0;
    nStatus = STATUS::WALK;
    finish_entrance = false;
    for(int i = 0; i < numStatus; i++)
    {
        tex[i] = NULL;
        fr[i] = {0, 0};
    }
    curAttack = 0;
    status = -1;
    Boss.setMaxHp(10000);
    Boss.setHp(10000);
    font = NULL;
}

BossLevel::~BossLevel()
{
    for(int i = 0; i < numStatus; i++)
    {
        tex[i] = NULL;
        SDL_DestroyTexture(tex[i]);
    }
    font = NULL;
}
void BossLevel::reset()
{
    timer = 0;
    timer1 = timer2 = timer3 = timer4 = 0;
    Boss.setW(270);
    Boss.setH(350);
    Boss.setX(1580);
    Boss.setY(SCREEN_HEIGHT - TILE_SIZE - Boss.getH());
    frame = 0;
    nStatus = STATUS::WALK;
    finish_entrance = false;
    curAttack = 0;
    status = -1;
    Boss.setMaxHp(10000);
    Boss.setHp(10000);
    vAttack1.clear();
    vAttack2.clear();
    vAttack3.clear();
    dropHP.clear();
    for(int i = 10; i <= 90; i += 10)
        dropHP.push_back(i);
}
void BossLevel::show(SDL_Renderer* renderer, AudioPlayer* audio)
{
    for(int i = 0; i < vAttack3.size(); i++)
    {
        SDL_Rect dRect = {0, 0, SCREEN_WIDTH, Boss.getH() / 2};
        dRect.y = vAttack3[i].first.y;
        SDL_RenderCopy(renderer, range, NULL, &dRect);
    }

    if(nStatus != status)
    {
        frame = 0;
        if(nStatus != STATUS::DIE)
            audio->play_boss_audio(nStatus);
    }
    else if(frame == 0 && (nStatus == STATUS::ATTACK1 || nStatus == STATUS::ATTACK2 || nStatus == STATUS::SCREAM))
    {
        audio->play_boss_audio(nStatus);
    }

    status = nStatus;

    SDL_Rect nRect = {Boss.getX() + Boss.getW() - 2 * bossSize / 3, Boss.getY() + Boss.getH() - 2 * bossSize / 3, bossSize, bossSize};
    SDL_Rect tRect = {(frame % fr[nStatus].first) * bossSize, (frame / fr[nStatus].first) * bossSize, bossSize, bossSize};

    SDL_Rect rect = Boss.getRect();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderCopy(renderer, tex[nStatus], &tRect, &nRect);
    frame++;
    if(status != STATUS::DIE)
        frame %= fr[status].second;
    else if(frame == fr[status].second - 1)
    {
        audio->play_boss_audio(status);
    }

    for(int i = 0; i < vAttack1.size(); i++)
        SDL_RenderCopy(renderer, pr[0], NULL, &vAttack1[i]);

    for(int i = 0; i < vAttack2.size(); i++)
    {
        SDL_Rect dRect = vAttack2[i].first.getRect();
        if(vAttack2[i].second == 0)
        {
            dRect.w /= 2;
            dRect.h /= 2;
        }
        SDL_RenderCopy(renderer, pr[1], NULL, &dRect);
    }

    for(int i = 0; i < vAttack3.size(); i++)
    {
        SDL_Rect nnRect = {0, 0, bossSize / 2, bossSize / 2};
        nnRect.x = vAttack3[i].first.x + vAttack3[i].first.w - bossSize / 3;
        nnRect.y = vAttack3[i].first.y + vAttack3[i].first.h - bossSize / 3;
        SDL_Rect ttRect = {(vAttack3[i].second % fr[5].first) * bossSize, (vAttack3[i].second / fr[5].first) * bossSize, bossSize, bossSize};

        SDL_RenderCopy(renderer, tex[5], &ttRect, &nnRect);
        SDL_RenderDrawRect(renderer, &vAttack3[i].first);
        vAttack3[i].second++;
        vAttack3[i].second %= fr[5].second;
    }
    showBossHealthbar(renderer);
}

void BossLevel::tick(Character* character, std::vector<Item>& vItem)
{
    cnt++;
    if(character->meleeAttack() && collision(Boss.getRect(), character->getMelee()) && vulnerable())
    {
        Boss.takeDamage(character->getDmg());
    }

    if(character->getX() > 920)
        character->setX(920);

    if(Boss.getHp() == 0)
    {
        nStatus = STATUS::DIE;
        if(status == STATUS::DIE && frame == 210)
            character->setStatus(5);

        if(status == STATUS::DIE && frame == fr[status].second - 1)
        {
            for(int id = 0; id <= 1; id++)
                for(int i = 0; i < 100; i++)
                {
                    Item nItem;
                    nItem.setX((Boss.getX() * 2 + Boss.getW()) / 2);
                    nItem.setY((Boss.getY() * 2 + Boss.getH()) / 2);
                    nItem.setVelY(Rand(-35, -10));
                    nItem.setVelX(Rand(-35, -10));
                    nItem.setW(30);
                    nItem.setH(30);
                    nItem.setId(id);
                    nItem.setVal(30);
                    vItem.push_back(nItem);
                }
        }
        vAttack1.clear();
        vAttack2.clear();
        vAttack3.clear();
        return;
    }

    if(!finish_entrance)
    {
        entrance();
        return;
    }
    nStatus = STATUS::IDLE;
    if(curAttack == 0)
        timer++;

    if(timer == 60)
    {
        curAttack = Rand(1, 4);
        if(curAttack == 1) timer1 = 0;
        if(curAttack == 2) timer2 = 0;
        if(curAttack == 3) timer3 = 0;
        if(curAttack == 4) timer4 = 0;
        timer = 0;
    }
    if(curAttack == 1)
    {
        attack1(character->getRect());
    }
    else if(curAttack == 2)
    {
        attack2(character->getRect());
    }
    else if(curAttack == 3)
    {
        attack3();
    }
    else if(curAttack == 4)
    {
        attack4(character);
    }

    if(dropHP.size())
        if(Boss.getHp() * 100 <= dropHP.back() * Boss.getMaxHp())
        {
            Item nItem;
            nItem.setX((Boss.getX() * 2 + Boss.getW()) / 2);
            nItem.setY((Boss.getY() * 2 + Boss.getH()) / 2);
            nItem.setVelY(Rand(-35, -10));
            nItem.setVelX(Rand(-35, -10));
            nItem.setW(30);
            nItem.setH(30);
            nItem.setId(2);
            nItem.setVal(character->getMaxHp() / 2);
            vItem.push_back(nItem);
            dropHP.pop_back();
        }

    if(vAttack1.size())
        for(int i = vAttack1.size() - 1; i >= 0; i--)
        {
            if(collision(character->getRect(), vAttack1[i]))
            {
                character->takeDamage(20);
                std::swap(vAttack1[i], vAttack1.back());
                vAttack1.pop_back();
            }
            else
            {
                if(vAttack1[i].x + vAttack1[i].w < 0)
                {
                    std::swap(vAttack1[i], vAttack1.back());
                    vAttack1.pop_back();
                }
                else vAttack1[i].x -= 10;
            }
        }
    if(vAttack2.size())
        for(int i = vAttack2.size() - 1; i >= 0; i--)
        {
            vAttack2[i].first.setX(vAttack2[i].first.getX() + vAttack2[i].first.getVelX());
            vAttack2[i].first.setY(vAttack2[i].first.getY() + vAttack2[i].first.getVelY());
            if(collision(character->getRect(), vAttack2[i].first.getRect()))
            {
                if(vAttack2[i].second == 0)
                    character->takeDamage(15);
                else
                    character->takeDamage(20);
                std::swap(vAttack2[i], vAttack2.back());
                vAttack2.pop_back();
            }
            else
            {
                if(vAttack2[i].second == 1 && vAttack2[i].first.getX() <= SCREEN_WIDTH / 3)
                {
                    GameObject temp;
                    temp.setRect(vAttack2[i].first.getRect());
                    temp.setVelX(-20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelX(20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelX(0);
                    temp.setVelY(-20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelY(20);
                    vAttack2.push_back({temp, 0});
                    std::swap(vAttack2[i], vAttack2.back());
                    vAttack2.pop_back();
                }
                else if(vAttack2[i].second == 2 && vAttack2[i].first.getX() <= SCREEN_WIDTH / 3)
                {
                    GameObject temp;
                    temp.setRect(vAttack2[i].first.getRect());
                    temp.setVelX(-20);
                    temp.setVelY(-20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelX(20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelY(20);
                    vAttack2.push_back({temp, 0});
                    temp.setVelX(-20);
                    vAttack2.push_back({temp, 0});
                    std::swap(vAttack2[i], vAttack2.back());
                    vAttack2.pop_back();

                }
                else if(vAttack2[i].second == 0)
                {
                    if(!collision({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, vAttack2[i].first.getRect()))
                    {
                        std::swap(vAttack2[i], vAttack2.back());
                        vAttack2.pop_back();
                    }
                }
            }
        }
    if(vAttack3.size())
    {
        for(int i = vAttack3.size() - 1; i >= 0; i--)
        {
            if(vAttack3[i].first.x + vAttack3[i].first.w < 0)
            {
                std::swap(vAttack3[i], vAttack3.back());
                vAttack3.pop_back();
            }
            else if(collision(character->getRect(), vAttack3[i].first))
            {
                character->takeDamage(25);
                std::swap(vAttack3[i], vAttack3.back());
                vAttack3.pop_back();
            }
            else vAttack3[i].first.x -= 50;
        }
    }
}

void BossLevel::entrance()
{
    if(Boss.getX() > 920)
    {
        Boss.setX(Boss.getX() - 5);
        nStatus = STATUS::WALK;
    }
    else
    {
        nStatus = STATUS::SCREAM;
        if(frame == fr[nStatus].second - 1)
            finish_entrance = true;
    }
}

void BossLevel::attack1(SDL_Rect character)
{
    nStatus = STATUS::ATTACK1;
    if(timer1 < 5)
    {
        if(character.y + character.h > Boss.getY() + Boss.getH())
            Boss.setY(Boss.getY() + 5);
        else if(character.y < Boss.getY())
            Boss.setY(Boss.getY() - 5);

        Boss.setY(std::min(Boss.getY(), SCREEN_HEIGHT - TILE_SIZE - Boss.getH()));
        if(frame == 17)
        {
            SDL_Rect nRect = Boss.getRect();
            nRect.x -= 100;
            nRect.w = pr_w[0];
            nRect.h = pr_h[0];
            vAttack1.push_back(nRect);
            timer1++;
        }
    }
    else
    {
        if(frame == fr[nStatus].second - 1 || status == STATUS::IDLE)
            nStatus = STATUS::IDLE;
        Boss.setY(Boss.getY() + 5);
        Boss.setY(std::min(Boss.getY(), SCREEN_HEIGHT - TILE_SIZE - Boss.getH()));
        if(Boss.getY() == SCREEN_HEIGHT - TILE_SIZE - Boss.getH())
            curAttack = 0;
    }


}

void BossLevel::attack2(SDL_Rect character)
{
    nStatus = STATUS::ATTACK2;
    if(timer2 < 5)
    {
        if(character.y + character.h > Boss.getY() + Boss.getH())
            Boss.setY(Boss.getY() + 5);
        else if(character.y < Boss.getY())
            Boss.setY(Boss.getY() - 5);

        Boss.setY(std::min(Boss.getY(), SCREEN_HEIGHT - TILE_SIZE - Boss.getH()));
        if(frame == 10)
        {
            SDL_Rect nRect = Boss.getRect();
            nRect.w = pr_w[1];
            nRect.h = pr_h[1];
            nRect.x -= 135;
            nRect.y += 100;
            GameObject temp;
            temp.setRect(nRect);
            temp.setVelX(-25);
            vAttack2.push_back({temp, Rand(1, 2)});
            timer2++;
        }
    }
    else
    {
        if(frame == fr[nStatus].second - 1 || status == STATUS::IDLE)
            nStatus = STATUS::IDLE;
        Boss.setY(Boss.getY() + 5);
        Boss.setY(std::min(Boss.getY(), SCREEN_HEIGHT - TILE_SIZE - Boss.getH()));
        if(Boss.getY() == SCREEN_HEIGHT - TILE_SIZE - Boss.getH())
            curAttack = 0;
    }
}

void BossLevel::attack3()
{
    nStatus = STATUS::SCREAM;
    if(timer3 < 7)
    {
        if(frame == 21)
        {
            SDL_Rect nRect;
            nRect.x = SCREEN_WIDTH;
            nRect.y = Rand(0, 3) * 4 * TILE_SIZE;
            nRect.w = Boss.getW() / 2;
            nRect.h = Boss.getH() / 2;
            vAttack3.push_back({nRect, 0});
            timer3++;
        }
    }
    else
    {
        if(frame == fr[nStatus].second - 1)
            curAttack = 0;
    }
}

void BossLevel::attack4(Character* character)
{
    if(timer4 == 0)
    {
        nStatus = STATUS::DIVE;
        if(status == STATUS::DIVE && frame == fr[nStatus].second - 1)
            timer4++;
    }
    else if(timer4 == 1)
    {
        nStatus = STATUS::MELT;
        if(character->getX() < Boss.getX())
        {
            Boss.setX(Boss.getX() - 25);
        }
        else
        {
            timer4++;
        }
    }
    else if(timer4 == 2)
    {
        nStatus = STATUS::DEVOUR;
        if(status == STATUS::DEVOUR && frame == fr[nStatus].second - 1)
            timer4++;
        if(status == STATUS::DEVOUR && frame == 15)
        {
            SDL_Rect nRect = Boss.getRect();
            nRect.y -= 180;
            nRect.h += 180;
            if(collision(character->getRect(), nRect))
                character->takeDamage(40);
        }
    }
    else if(timer4 == 3)
    {
        nStatus = STATUS::DIVE;
        if(status == STATUS::DIVE && frame == fr[nStatus].second - 1)
            timer4++;
    }
    else if(timer4 == 4)
    {
        nStatus = STATUS::MELT;
        if(Boss.getX() < 920)
        {
            Boss.setX(Boss.getX() + 25);
        }
        else
        {
            timer4++;
        }
    }
    else if(timer4 == 5)
    {
        nStatus = STATUS::RISE;
        if(status == STATUS::RISE && frame == fr[nStatus].second - 1)
            curAttack = 0;
    }
}

void BossLevel::showBossHealthbar(SDL_Renderer* renderer)
{
    healthbar.render(renderer, 0);
    int len = healthbar.getW() - 10;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if(finish_entrance)
    {
        rRect.w = Boss.getHp() * len / Boss.getMaxHp();
    }
    else
    {
        rRect.w = std::min(rRect.w + 5, len);
    }
    SDL_RenderFillRect(renderer, &rRect);
    SDL_Surface* sf = TTF_RenderText_Solid(font, "Shadow Beast", white);
    int w = sf->w, h = sf->h;
    SDL_Texture* tt = SDL_CreateTextureFromSurface(renderer, sf);
    SDL_Rect nRect;
    nRect.x = healthbar.getX() + (healthbar.getW() - w) / 2;
    nRect.y = healthbar.getY() + (healthbar.getH() - h) / 2;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, tt, NULL, &nRect);

    int percent = ceil(double(Boss.getHp()) / Boss.getMaxHp() * 100);
    sf = TTF_RenderText_Solid(font, (int2str(percent) + "%").c_str(), white);
    tt = SDL_CreateTextureFromSurface(renderer, sf);

    w = sf->w, h = sf->h;
    nRect.x = healthbar.getX() + healthbar.getW() - w - 5;
    nRect.y = healthbar.getY() + (healthbar.getH() - h) / 2;
    nRect.w = w, nRect.h = h;
    SDL_RenderCopy(renderer, tt, NULL, &nRect);

    SDL_DestroyTexture(tt);
    SDL_FreeSurface(sf);

    bossIcon.render(renderer, 0);

}

void BossLevel::loadBoss(SDL_Renderer* renderer)
{
    std::string path = "img/Level_" + int2str(numLevel) + "/";
    SDL_Surface* sf = NULL;
    for(int i = 0; i < numStatus; i++)
    {
        sf = IMG_Load((path + int2str(i) + ".png").c_str());
        tex[i] = SDL_CreateTextureFromSurface(renderer, sf);
    }
    std::ifstream file((path + "info.txt").c_str());
    for(int i = 0; i < numStatus; i++)
    {
        file >> fr[i].first >> fr[i].second;
    }
    file.close();
    for(int i = 0; i < numBossProjectile; i++){
        sf = IMG_Load((path + "pr" + int2str(i) + ".png").c_str());
        pr[i] = SDL_CreateTextureFromSurface(renderer, sf);
        pr_w[i] = sf->w;
        pr_h[i] = sf->h;
    }
    bossIcon.loadImage(path + "bossIcon.png", renderer);
    healthbar.loadImage(path + "healthbar.png", renderer);

    sf = IMG_Load((path + "range.png").c_str());
    range = SDL_CreateTextureFromSurface(renderer, sf);

    SDL_FreeSurface(sf);

    bossIcon.setX((SCREEN_WIDTH - bossIcon.getW()) / 2);
    bossIcon.setY(20);

    healthbar.setX((SCREEN_WIDTH - healthbar.getW()) / 2);
    healthbar.setY(60);

    rRect = healthbar.getRect();

    rRect.x += 5;
    rRect.y += 5;
    rRect.h -= 10;
    rRect.w = 0;

    font = TTF_OpenFont("calibri.ttf", 30);
}
