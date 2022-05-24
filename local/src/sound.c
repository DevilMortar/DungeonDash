#include "../include/sound.h"

static int numberOfSounds = 0;
static bool isPlaying = true;
static SL_SOUND *soundList = NULL;
static bool development = false;

void SL_initSoundLib(char folder[], int nbChannels, bool allowDevelopment)
{
    // Check if user want to use development mode
    development = allowDevelopment;
    // Initialize SDL_mixer
    Mix_Init(MIX_INIT_MOD | MIX_INIT_OGG);
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("%s", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_AllocateChannels(NB_CHANNELS);
    // Load sounds
    soundList = NULL;
    DIR *rep;
    struct dirent *file;

    rep = opendir(folder);
    if (rep != NULL)
    {
        while (file = readdir(rep))
        {
            if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && strstr(file->d_name, ".wav") != NULL)
            {
                char path[100];
                strcpy(path, folder);
                strcat(path, file->d_name);
                SL_loadSongInQueue(path, file->d_name, numberOfSounds);
            }
        }
        (void)closedir(rep);
    }
}

void SL_loadSongInQueue(char *path, char *name, int channel)
{
    SL_SOUND *song = malloc(sizeof(SL_SOUND));
    char *songToken = strtok(name, ".");
    song->chunk = Mix_LoadWAV(path);
    song->name = malloc(sizeof(char) * (strlen(songToken) + 1));
    strcpy(song->name, songToken);
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
    if (development)
    {
        printf("SL_loadSongInQueue | Sound %s from %s sucessfully loaded !\n", songToken, path);
    }
}

void SL_playSong(char *name, int volume, int repeat)
{
    int channel = -1;
    if (SL_isPlaying())
    {
        SL_SOUND *song = soundList;
        while (song != NULL && strcmp(song->name, name) != 0)
        {
            song = song->next;
        }
        if (song != NULL)
        {
            channel = Mix_PlayChannel(-1, song->chunk, repeat);
            if (volume > 0)
            {
                Mix_Volume(channel, (int)MIX_MAX_VOLUME * volume / 100);
            }
            if (development)
            {
                printf("SL_playSong | Sound %s played !\n", song->name);
            }
        }
    }
}

void SL_freeSoundLib()
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
    Mix_Quit();
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
    for (int i = 0; i<=numberOfSounds; i++) {
        Mix_Pause(i);
    }

}

void SL_unmute()
{
    isPlaying = true;
    Mix_ResumeMusic();
    for (int i = 0; i<=numberOfSounds; i++) {
        Mix_Resume(i);
    }
}