#ifndef MUSIC_H
#define MUSIC_H

#include "SDL.h"
#include "SDL_mixer.h"

extern  Mix_Music *music;
extern int MUSIC;
extern char* MAIN_MUSIC;

void initMusic(char *s);
int initSDL();
void closeMusic();
void musicMenu();

#endif // MUSIC_H