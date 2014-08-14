/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include <SDL2_mixer/SDL_mixer.h>

#include "quakedef.h"

static Mix_Music *music;

void CDAudio_Play(byte track, qboolean looping)
{
    unsigned long len = SDL_strlen(host_parms.basedir) + 25;
    char *music_location = (char *) malloc(len);
    SDL_snprintf(music_location, len, "%sid1/music/track%02d.ogg", host_parms.basedir, track);
    music = Mix_LoadMUS(music_location);
    Mix_PlayMusic(music, looping);
}


void CDAudio_Stop(void)
{
    Mix_FreeMusic(music);
}


void CDAudio_Pause(void)
{
    Mix_PauseMusic();
}


void CDAudio_Resume(void)
{
    Mix_ResumeMusic();
}


void CDAudio_Update(void)
{
}


int CDAudio_Init(void)
{
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	return 0;
}


void CDAudio_Shutdown(void)
{
    Mix_CloseAudio();
    Mix_Quit();
}