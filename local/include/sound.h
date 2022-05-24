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
    SL_SOUND *next;
};

/**
 * @brief Initialize SDL_Mixer
 * @param folder The folder containing the sounds
 * @param nbChannels The number of channels to use
 * @param allowDevelopment Allow the use of development mode
 */
void SL_initSoundLib(char folder[], int nbChannels, bool allowDevelopment);

/**
 * @brief Loading a music from the queue
 * @param path The path to the music
 * @param name The name of the music
 * @param channel The channel to use
 */
void SL_loadSongInQueue(char *path, char *name);

/**
 * @brief Play a specific music
 * @param name The name of the music
 * @param volume The % of volume to use
 * @param repeat The number of times to repeat the music
 */
void SL_playSong(char *name, int volume, int repeat);

/**
 * @brief Freeing memory
 */
void SL_freeSoundLib();

/**
 * @brief Returning the number of musics in the queue
 * @return Int The number of musics in the queue
 */
int SL_getNumberOfSounds();

/**
 * @brief Tell if it is possible to play a music or not
 * @return True if it is possible to play a music, false otherwise
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
