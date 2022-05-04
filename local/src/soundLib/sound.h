#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
void SL_playSong(SL_SOUND * soundList, char * name, int volume); //Joue la musique
void SL_freeSoundLib(SL_SOUND * soundList); //Libère la mémoire
int SL_getNumberOfSounds(); //Renvoie le nombre de musiques
bool SL_isMusicPlaying(); //Renvoie si la lecture est possible
void SL_mute(); //Empêche les musiques de jouer
void SL_unmute(); //Autorise les musiques à jouer
