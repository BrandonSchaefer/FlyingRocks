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

#include "vector.h"
#include "sdl_backend.h"
#include "sdl_renderer.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>

// TODO Reomve possibly
using namespace std::chrono_literals;

// for debuging
std::ostream& operator<<(std::ostream& out, Vector const& v)
{
  out << "{" << std::fixed << v.x << ", " << v.y << ", " << v.z << "}";
  return out;
}

int main()
{
    SDLBackend backend;
    SDLRenderer renderer("Asteroids", {800, 600});
    /*
    auto renderer = backend.renderer();

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                default:
                    break;
            }
        }

        SDL_RenderClear(renderer);
        // Draw
        SDL_RenderPresent(renderer);

        // TODO cap FPS or something here
        std::this_thread::sleep_for(50ms);
    }
    */

    return 0;
}
