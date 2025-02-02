#include "music.h"
#include "game.h"

Mix_Music *music;
char *musics[3] = {"music/main_theme_the_black_waltz.mp3", "music/main_theme_magic_dustbin.mp3", "music/main_theme_i_got_a_stick.mp3"};
#define MUSIC_SZ 3

char *MAIN_MUSIC;


Mix_Music* loadMusic(const char* filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath);
    if (!music) {
        printf("Failed to load music: %s\n", Mix_GetError());
        return NULL;
    }
    return music;
}

int initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! Mix_Error: %s\n", Mix_GetError());
        return 0;
    }
    return 1;
}

void initMusic(char *s)
{
    music = loadMusic(s);
    if (!music)
    {
        closeMusic(music);
    }

    Mix_PlayMusic(music, -1);
}

void closeMusic() {
    if (music)
    {
        Mix_FreeMusic(music);
    }
}

void musicMenu() {
  char *s[4] = {"Play Music", "Stop Music", "Change Main Music"};
  char *msg[4] = {"", "", "", ""};
  int x = createMenu(s, msg, "Music", 3);
  switch (x)
  {
    case 0:
        initMusic(MAIN_MUSIC);
        MUSIC = 1;
        return;
    case 1:
        closeMusic();
        MUSIC = 0;
        return;
    case 2:
        MUSIC = 1;
        MAIN_MUSIC = musics[rand() % MUSIC_SZ];
        initMusic(MAIN_MUSIC);
        return;
  }
}