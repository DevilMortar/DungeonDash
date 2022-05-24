#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#define NB_CHANNELS 50

typedef struct SL_SOUND SL_SOUND;
struct SL_SOUND
{
    Mix_Chunk *chunk;
    char *name;
    int channel;
    SL_SOUND *next;
};

void SL_initSoundLib(char folder[], int nbChannels, bool allowDevelopment); // Initialize SDL_mixer
void SL_loadSongInQueue(char *path, char *name, int channel);               // Charge une musique dans la file
void SL_playSong(char *name, int volume, int repeat);                                   // Joue la musique
void SL_freeSoundLib();                                                     // Libère la mémoire
int SL_getNumberOfSounds();                                                // Renvoie le nombre de musiques
bool SL_isPlaying();                                                        // Renvoie si la lecture est possible
void SL_mute();                                                             // Empêche les musiques de jouer
void SL_unmute();                                                           // Autorise les musiques à jouer

#endif
