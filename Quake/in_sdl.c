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

static int mouse_x, mouse_y = 0;

void IN_Init (void)
{
}

void IN_Shutdown (void)
{
}

void IN_Commands (void)
{
}

void IN_Move (usercmd_t *cmd)
{
	mouse_x *= sensitivity.value;
	mouse_y *= sensitivity.value;

	if ( (in_strafe.state & 1) || (lookstrafe.value && (in_mlook.state & 1) ))
		cmd->sidemove += m_side.value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw.value * mouse_x;
	if (in_mlook.state & 1)
		V_StopPitchDrift ();

	if ( (in_mlook.state & 1) && !(in_strafe.state & 1)) {
		cl.viewangles[PITCH] += m_pitch.value * mouse_y;
		if (cl.viewangles[PITCH] > 80)
			cl.viewangles[PITCH] = 80;
		if (cl.viewangles[PITCH] < -70)
			cl.viewangles[PITCH] = -70;
	} else {
		if ((in_strafe.state & 1) && noclip_anglehack)
			cmd->upmove -= m_forward.value * mouse_y;
		else
			cmd->forwardmove -= m_forward.value * mouse_y;
	}
	mouse_x = mouse_y = 0;
}

void IN_SendKeyEvents()
{
    SDL_Event event;
    int keycode, pressed;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Host_Quit_f();
                break;

            case SDL_WINDOWEVENT:
                if (windowID == event.window.windowID) {
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            if (key_dest == key_game) {
                                SDL_SetRelativeMouseMode(SDL_TRUE);
                            }
                            isFocused = true;
                            break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            if (key_dest == key_game) {
                                SDL_SetRelativeMouseMode(SDL_FALSE);
                            }
                            isFocused = false;
                            break;
                        case SDL_WINDOWEVENT_HIDDEN:
                            scr_skipupdate = true;
                            break;
                        case SDL_WINDOWEVENT_SHOWN:
                            scr_skipupdate = false;
                            break;
                        default:
                            break;
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                // ignore mouse in menu
                if (key_dest == key_game) {
                    mouse_x += event.motion.xrel;
                    mouse_y += event.motion.yrel;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                // K_MOUSE1-K_MOUSE3 is 200-202, SDL_BUTTON_LEFT-SDL_BUTTON_RIGHT is 1-3
                Key_Event(event.button.button + 199, event.button.state);
                SDL_BUTTON_LEFT;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                keycode = event.key.keysym.sym;
                pressed = event.key.state;

                switch (keycode) {
                    case SDLK_BACKSPACE:
                        keycode = K_BACKSPACE;
                        break;
                    case SDLK_UP:
                        keycode = K_UPARROW;
                        break;
                    case SDLK_DOWN:
                        keycode = K_DOWNARROW;
                        break;
                    case SDLK_LEFT:
                        keycode = K_LEFTARROW;
                        break;
                    case SDLK_RIGHT:
                        keycode = K_RIGHTARROW;
                        break;
                    case SDLK_RALT:
                    case SDLK_LALT:
                        keycode = K_ALT;
                        break;
                    case SDLK_RCTRL:
                    case SDLK_LCTRL:
                        keycode = K_CTRL;
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        keycode = K_SHIFT;
                        break;
                    case SDLK_F1:
                        keycode = K_F1;
                        break;
                    case SDLK_F2:
                        keycode = K_F2;
                        break;
                    case SDLK_F3:
                        keycode = K_F3;
                        break;
                    case SDLK_F4:
                        keycode = K_F4;
                        break;
                    case SDLK_F5:
                        keycode = K_F5;
                        break;
                    case SDLK_F6:
                        keycode = K_F6;
                        break;
                    case SDLK_F7:
                        keycode = K_F7;
                        break;
                    case SDLK_F8:
                        keycode = K_F8;
                        break;
                    case SDLK_F9:
                        keycode = K_F9;
                        break;
                    case SDLK_F10:
                        keycode = K_F10;
                        break;
                    case SDLK_F11:
                        keycode = K_F11;
                        break;
                    case SDLK_F12:
                        keycode = K_F12;
                        break;
                    case SDLK_INSERT:
                        keycode = K_INS;
                        break;
                    case SDLK_DELETE:
                        keycode = K_DEL;
                        break;
                    case SDLK_PAGEDOWN:
                        keycode = K_PGDN;
                        break;
                    case SDLK_PAGEUP:
                        keycode = K_PGUP;
                        break;
                    case SDLK_HOME:
                        keycode = K_HOME;
                        break;
                    case SDLK_END:
                        keycode = K_END;
                        break;
                    case SDLK_PAUSE:
                        keycode = K_PAUSE;
                        break;
                    default:
                        if (keycode > 126) {
                            keycode = 0;
                        }
                        break;
                }
                Key_Event(keycode, pressed);
                break;

            default:
                break;
        }
    }
}
