#include <SDL2/SDL_mixer.h>

typedef struct SL_SOUND SL_SOUND;
struct SL_SOUND
{
    Mix_Chunk * chunk;
    char * name;
    int channel;
    SL_SOUND * next;
};

SL_SOUND * SL_initSoundLib(int channels, char folder[], char * name[]);
SL_SOUND * SL_loadSongInQueue(SL_SOUND * soundList, char * path, char * name, int channel); //Charge une musique dans la file
void SL_playSong(SL_SOUND * soundList, char * name); //Joue la musique
