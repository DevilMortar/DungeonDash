#include "sound.h"

SL_SOUND * SL_loadSongInQueue(SL_SOUND * soundList, char * path, char * name, int channel) {
    SL_SOUND * song = malloc(sizeof(SL_SOUND));
    song->channel = channel;
    song->chunk = Mix_LoadWAV(path);
    song->name = name;
    if (soundList == NULL) {
        soundList = song;
        song->next = NULL;
    }
    else {
        song->next = soundList;
        soundList = song;
    }
    return soundList;
}

void SL_playSong(SL_SOUND * soundList, char * name) {
    SL_SOUND * song = soundList;
    while (song != NULL) {
        if (strcmp(song->name, name) == 0) {
            Mix_PlayChannel(song->channel, song->chunk, 0);
        }
        song = song->next;
    }
}

SL_SOUND * SL_initSoundLib(int channels, char folder[], char * name[]) {
    SL_SOUND * soundList = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("%s", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_AllocateChannels(channels);
    for (int i = 0; i < channels; i++) {
        char path[100];
        strcpy(path, folder);
        strcat(path,"/");
        strcat(path, name[i]);
        strcat(path, ".wav");
        soundList = SL_loadSongInQueue(soundList, path, name[i], i);
    }
    return soundList;
}