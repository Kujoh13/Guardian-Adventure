#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    for(int i = 0; i < numLevel; i++)
        bgm_level[i] = NULL;

    for(int i = 0; i < numCharacter; i++)
        char_attack[i] = NULL;

    for(int i = 0; i < numStatus; i++)
        bossAudio[i] = NULL;

    startScreen = NULL;
    levelSelection = NULL;
    collect_item = NULL;
    change_map = NULL;
    select_character = NULL;
    unlock = NULL;
    upgrade = NULL;
    click = NULL;
    play_button = NULL;
    win = NULL;
    lose = NULL;
    heal = NULL;
    explosion = NULL;
    walk = NULL;
    fly = NULL;
    jump = NULL;

    nextMove = 0;
}
AudioPlayer::~AudioPlayer()
{
    for(int i = 0; i < numLevel; i++)
        Mix_FreeMusic(bgm_level[i]);

    for(int i = 0; i < numCharacter; i++)
        Mix_FreeChunk(char_attack[i]);

    for(int i = 0; i < numStatus; i++)
        Mix_FreeChunk(bossAudio[i]);

    Mix_FreeMusic(startScreen);
    Mix_FreeMusic(levelSelection);
    Mix_FreeChunk(collect_item);
    Mix_FreeChunk(change_map);
    Mix_FreeChunk(select_character);
    Mix_FreeChunk(upgrade);
    Mix_FreeChunk(unlock);
    Mix_FreeChunk(click);
    Mix_FreeChunk(play_button);
    Mix_FreeChunk(win);
    Mix_FreeChunk(lose);
    Mix_FreeChunk(mob_die);
    Mix_FreeChunk(heal);
    Mix_FreeChunk(explosion);
    Mix_FreeChunk(fly);
    Mix_FreeChunk(walk);
    Mix_FreeChunk(jump);
}

void AudioPlayer::loadAudioFiles()
{
    startScreen = Mix_LoadMUS("audio/startScreen.wav");
    levelSelection = Mix_LoadMUS("audio/levelSelection.wav");
    for(int i = 0; i < numLevel; i++)
    {
        std::string path = "audio/bgm_" + int2str(i + 1) + ".wav";
        bgm_level[i] = Mix_LoadMUS(path.c_str());
    }
    collect_item = Mix_LoadWAV("audio/collect_item.wav");
    change_map = Mix_LoadWAV("audio/change_map.wav");
    select_character = Mix_LoadWAV("audio/select_character.wav");
    unlock = Mix_LoadWAV("audio/unlock.wav");
    upgrade = Mix_LoadWAV("audio/upgrade.wav");
    click = Mix_LoadWAV("audio/click.wav");
    play_button = Mix_LoadWAV("audio/play.wav");
    win = Mix_LoadWAV("audio/win.wav");
    lose = Mix_LoadWAV("audio/lose.wav");
    mob_die = Mix_LoadWAV("audio/mob_die.wav");
    heal = Mix_LoadWAV("audio/heal.wav");
    explosion = Mix_LoadWAV("audio/explosion.wav");
    walk = Mix_LoadWAV("audio/walk.wav");
    fly = Mix_LoadWAV("audio/fly.wav");
    jump = Mix_LoadWAV("audio/jump.wav");

    for(int i = 0; i < numCharacter; i++)
    {
        std::string path = "audio/attack_" + int2str(i) + ".wav";
        char_attack[i] = Mix_LoadWAV(path.c_str());
    }

    for(int i = 0; i < numStatus; i++)
    {
        std::string path = "audio/boss_" + int2str(i) + ".wav";
        bossAudio[i] = Mix_LoadWAV(path.c_str());
    }
}

void AudioPlayer::playBackgroundMusic(int isRunning, int level)
{
    if(Mix_PlayingMusic() == false)
    {
        if(isRunning == 1)
            Mix_PlayMusic(startScreen, -1);
        else if(isRunning == 2)
            Mix_PlayMusic(levelSelection, -1);
        else if(isRunning == 3)
            Mix_PlayMusic(bgm_level[level - 1], -1);
    }

    if(Mix_PlayingMusic() == false)
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}

void AudioPlayer::stopAudio()
{
    Mix_HaltMusic();
}

void AudioPlayer::collectItem()
{
    Mix_PlayChannel(-1, collect_item, 0);
}

void AudioPlayer::changeMap()
{
    Mix_PlayChannel(-1, change_map, 0);
}

void AudioPlayer::selectCharacter()
{
    Mix_PlayChannel(-1, select_character, 0);
}

void AudioPlayer::unlockCharacter()
{
    Mix_PlayChannel(-1, unlock, 0);
}

void AudioPlayer::upgradeCharacter()
{
    Mix_PlayChannel(-1, upgrade, 0);
}

void AudioPlayer::mouse_click()
{
    Mix_PlayChannel(-1, click, 0);
}

void AudioPlayer::playButton()
{
    Mix_PlayChannel(-1, play_button, 0);
}

void AudioPlayer::winGame()
{
    Mix_PlayChannel(-1, win, 0);
}

void AudioPlayer::loseGame()
{
    Mix_PlayChannel(-1, lose, 0);
}

void AudioPlayer::mobDie()
{
    Mix_PlayChannel(-1, mob_die, 0);
}

void AudioPlayer::character_heal()
{
    Mix_PlayChannel(-1, heal, 0);
}

void AudioPlayer::bomb_explosion()
{
    Mix_PlayChannel(-1, explosion, 0);
}

void AudioPlayer::character_attack(int cur_character)
{
    Mix_PlayChannel(-1, char_attack[cur_character], 0);
}

void AudioPlayer::character_move(int type)
{
    if(nextMove == 0){
        if(type) Mix_PlayChannel(-1, fly, 0);
        else Mix_PlayChannel(-1, walk, 0);
    }
    nextMove++;
    nextMove %= 13;
}

void AudioPlayer::character_jump()
{
    Mix_PlayChannel(-1, jump, 0);
}

void AudioPlayer::play_boss_audio(int status)
{
    if(bossAudio[status] == NULL) return;
    Mix_PlayChannel(-1, bossAudio[status], 0);
}
