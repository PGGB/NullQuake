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

#include <SDL2/SDL.h>

#include "quakedef.h"

static int bufferSize;
static SDL_AudioDeviceID dev;

static void audioCallback(void *userData, Uint8 *stream, int len)
{
	int	bytesPosition, bytesRemaining;

	if (!shm) {
		memset(stream, 0, len);
		return;
	}

	bytesPosition = (shm->samplepos * (shm->samplebits / 8));
	bytesRemaining = bufferSize - bytesPosition;

    if (len < bytesRemaining) {
        memcpy(stream, shm->buffer + bytesPosition, len);
        shm->samplepos += len / (shm->samplebits /8);
    } else {
        memcpy(stream, shm->buffer + bytesPosition, bytesRemaining);
        memcpy(stream + bytesRemaining, shm->buffer, len - bytesRemaining);
        shm->samplepos = (len - bytesRemaining) / (shm->samplebits / 8);
    }
}

qboolean SNDDMA_Init(void)
{
	SDL_AudioSpec desired, obtained;
    SDL_zero(desired);

    desired.freq        = 11025;
    desired.format      = loadas8bit.value ? AUDIO_U8 : AUDIO_S16SYS;
    desired.channels    = 2;
    desired.samples     = 512;
    desired.callback    = audioCallback;

    dev = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (dev == 0) {
        Con_Printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
        return false;
    }

	shm = &sn;

    // SDL_AudioFormat layout
    //    +----------------------sample is signed if set
    //    |
    //    |        +----------sample is bigendian if set
    //    |        |
    //    |        |           +--sample is float if set
    //    |        |           |
    //    |        |           |  +--sample bit size---+
    //    |        |           |  |                    |
    //   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
    shm->samplebits         = obtained.format & 0xFF;
    shm->speed              = obtained.freq;
    shm->channels           = obtained.channels;
    shm->samples            = obtained.samples * obtained.channels * 8;
    shm->submission_chunk   = 1;
    shm->samplepos          = 0;

	bufferSize = shm->samples * (shm->samplebits / 8);

	shm->buffer = (unsigned char *) calloc(1, bufferSize);
	if (shm->buffer == NULL) {
        Con_Printf("Couldn't allocate sound buffer\n");
		SDL_CloseAudioDevice(dev);
		shm = NULL;
		return false;
	}
    
	SDL_PauseAudioDevice(dev, 0);
    
	return true;
}

int SNDDMA_GetDMAPos(void)
{
    return shm->samplepos;
}

void SNDDMA_Shutdown(void)
{
    if (shm) {
		SDL_CloseAudioDevice(dev);
		if (shm->buffer) {
			free(shm->buffer);
        }
		shm->buffer = NULL;
		shm = NULL;
	}
}

void SNDDMA_Submit(void)
{
}



