#include "../include/sound.h"

int numberOfSounds = 0;
bool isPlaying = true;

SL_SOUND *SL_loadSongInQueue(SL_SOUND *soundList, char *path, char *name, int channel)
{
    SL_SOUND *song = malloc(sizeof(SL_SOUND));
    song->channel = channel;
    song->chunk = Mix_LoadWAV(path);
    song->name = name;
    if (soundList == NULL)
    {
        soundList = song;
        song->next = NULL;
    }
    else
    {
        song->next = soundList;
        soundList = song;
    }
    numberOfSounds++;
    printf("SL_loadSongInQueue | Sound from %s sucessfully loaded !\n", path);
    return soundList;
}

void SL_playSong(SL_SOUND *soundList, char *name, int volume)
{
    if (SL_isPlaying())
    {
        SL_SOUND *song = soundList;
        while (song != NULL)
        {
            if (strcmp(song->name, name) == 0)
            {
                if (volume > 0)
                {
                    Mix_Volume(song->channel, (int)MIX_MAX_VOLUME * volume / 100);
                }
                Mix_PlayChannel(song->channel, song->chunk, 0);
            }
            song = song->next;
        }
    }
}

SL_SOUND *SL_initSoundLib(int channels, char folder[], char *name[])
{
    Mix_Init(MIX_INIT_MOD | MIX_INIT_OGG);
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("%s", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_AllocateChannels(channels);
    SL_SOUND *soundList = NULL;
    for (int i = 0; i < channels; i++)
    {
        char path[100];
        strcpy(path, folder);
        strcat(path, "/");
        strcat(path, name[i]);
        strcat(path, ".wav");
        soundList = SL_loadSongInQueue(soundList, path, name[i], i);
    }
    return soundList;
}

void SL_freeSoundLib(SL_SOUND *soundList)
{
    SL_SOUND *tmp = soundList;
    while (tmp != NULL)
    {
        SL_SOUND *tmp2 = tmp->next;
        Mix_FreeChunk(tmp->chunk);
        free(tmp);
        tmp = tmp2;
    }
    Mix_CloseAudio();
}

int SL_getNumberOfSounds()
{
    return numberOfSounds;
}

bool SL_isPlaying()
{
    return isPlaying;
}

void SL_mute()
{
    isPlaying = false;
    Mix_PauseMusic();
}

void SL_unmute()
{
    isPlaying = true;
    Mix_ResumeMusic();
}
