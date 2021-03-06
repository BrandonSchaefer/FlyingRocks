//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Brandon Schaefer
 *                    brandontschaefer@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "sdl_backend.h"
#include "sdl_error.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace
{
void init_sdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw SDLError("Failed to init SDL");
    }
}

void init_ttf()
{
    if (TTF_Init() < 0)
    {
        SDL_Quit();
        throw SDLError("Failed to init TTF");
    }
}

void init_mixer()
{
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        TTF_Quit();
        SDL_Quit();
        throw SDLError("Failed to init SDL mixer");
    }
}
}

// TODO Add overwritable default builders for img/ttf/mixer which will allow for custom
// creation of those backends
SDLBackend::SDLBackend()

{
    // Order matters when it comes to failing
    init_sdl();
    init_ttf();
    init_mixer();
}

SDLBackend::~SDLBackend()
{
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}
