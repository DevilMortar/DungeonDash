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

/**
 * @brief Initialize SDL_Mixer
 * @param folder
 * @param nbChannels
 * @param allowDevelopment
 */
void SL_initSoundLib(char folder[], int nbChannels, bool allowDevelopment);

/**
 * @brief Loading a music from the queue
 * @param path
 * @param name
 * @param channel
 */
void SL_loadSongInQueue(char *path, char *name, int channel);

/**
 * @brief Play a specific music
 * @param name
 * @param volume
 * @param repeat
 */
void SL_playSong(char *name, int volume, int repeat);

/**
 * @brief Freeing memory
 */
void SL_freeSoundLib();

/**
 * @brief Returning the number of musics in the queue
 * @return int
 */
int SL_getNumberOfSounds();

/**
 * @brief Tell if it is possible to play a music or not
 * @return bool
 */
bool SL_isPlaying();

/**
 * @brief Mute a music
 */
void SL_mute();

/**
 * @brief Unmute a music
 */
void SL_unmute();

#endif
