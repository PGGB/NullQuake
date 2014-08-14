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
#include "d_local.h"

extern int           windowID;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;
static uint32_t      palette[256];

// global video state
viddef_t vid;

#define	BASEWIDTH	320
#define	BASEHEIGHT	200
#define WIDTH       320
#define HEIGHT      240

byte	vid_buffer[BASEWIDTH*BASEHEIGHT];
short	zbuffer[BASEWIDTH*BASEHEIGHT];
byte	surfcache[256*1024];

unsigned short	d_8to16table[256];
unsigned        d_8to24table[256];

void VID_SetPalette(unsigned char *pal)
{
    for (int i=0; i<256; ++i) {
        palette[i] = pal[i*3] << 16 | pal[i*3+1] << 8 | pal[i*3+2];
    }
}

void VID_ShiftPalette(unsigned char *pal)
{
    VID_SetPalette(pal);
}

void VID_Init(unsigned char *pal)
{
    SDL_Window *window = SDL_GetWindowFromID(windowID);
    if (window == NULL) {
        Con_Printf("SDL_GetWindowFromID error: %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        Con_Printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        return;
    }
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, BASEWIDTH, BASEHEIGHT);
    if (texture == 0) {
        printf("SDL_CreateTexture error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        return;
    }

	vid.maxwarpwidth = WARP_WIDTH;
    vid.maxwarpheight = WARP_HEIGHT;
    vid.width = vid.conwidth = BASEWIDTH;
    vid.height = vid.conheight = BASEHEIGHT;
	vid.aspect = ((float)vid.height / (float)vid.width) * (320.0 / 240.0);
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;
	
	d_pzbuffer = zbuffer;
	D_InitCaches (surfcache, sizeof(surfcache));
}

void VID_Shutdown(void)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
}

void VID_Update(vrect_t *rects)
{
    uint32_t *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);
    for (int x=0; x<BASEWIDTH; ++x) {
        for (int y=0; y<BASEHEIGHT; ++y) {
            int pos = y * BASEWIDTH + x;
            pixels[pos] = palette[vid.buffer[pos]];
        }
    }
    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void D_BeginDirectRect(int x, int y, byte *pbitmap, int width, int height)
{
}

void D_EndDirectRect(int x, int y, int width, int height)
{
}


