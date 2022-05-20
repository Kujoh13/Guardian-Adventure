#ifndef AUDIOPLAYER_H_
#define AUDIOPLAYER_H_

#include "BasicFunction.h"
#include <SDL_mixer.h>

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();
    void loadAudioFiles();
    void playBackgroundMusic(int isRunning, int level);
    void stopAudio();
    void collectItem();
    void changeMap();
    void selectCharacter();
    void unlockCharacter();
    void upgradeCharacter();
    void mouse_click();
    void playButton();
    void winGame();
    void loseGame();
    void mobDie();
    void character_heal();
    void bomb_explosion();
    void character_attack(int cur_character);
protected:
    Mix_Music* bgm_level[numLevel];
    Mix_Music* startScreen;
    Mix_Music* levelSelection;
    Mix_Chunk* collect_item;
    Mix_Chunk* change_map;
    Mix_Chunk* select_character;
    Mix_Chunk* unlock;
    Mix_Chunk* upgrade;
    Mix_Chunk* click;
    Mix_Chunk* play_button;
    Mix_Chunk* win;
    Mix_Chunk* lose;
    Mix_Chunk* mob_die;
    Mix_Chunk* explosion;
    Mix_Chunk* heal;
    Mix_Chunk* char_attack[numCharacter];
};

#endif //AUDIOPLAYER_H_
