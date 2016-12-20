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

#include "vector_line_graphic.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>

// TODO Reomve possibly
using namespace std::chrono_literals;

// for debuging
std::ostream& operator<<(std::ostream& out, Vector const& v)
{
  out << "{" << std::fixed << v.x << ", " << v.y << "}";
  return out;
}

int main()
{
    float width = 800;
    float height = 600;
    SDLBackend backend;
    SDLRenderer renderer("Asteroids", {(int)width, (int)height});
    auto sdl_renderer = renderer.renderer();

    //VectorLineGraphic v({{width/2 + 10, height/2, 0}, {width/2 + 5, height/2 + 15, 0}, {width/2 + 15, height/2 + 15, 0}});

    /*
    VectorLineGraphic v({
        {width/2 + 10, height/2 + 10},
        {width/2 + 11, height/2 + 13},
        {width/2 + 7,  height/2 + 18},
        {width/2 + 9,  height/2 + 20},
        {width/2 + 13, height/2 + 19},
        {width/2 + 16, height/2 + 13},
        {width/2 + 13, height/2 + 11},
    });
    v.scale(5.0f);
    */

    VectorLineGraphic v;

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONUP:
                {
                    float x = event.button.x;
                    float y = event.button.y;
                    v.add_point({x, y});
                }
                    break;
                case SDL_KEYUP:
                {
                    if (event.key.keysym.sym == SDLK_RIGHT)
                        v.scale(1.1f);
                    else if (event.key.keysym.sym == SDLK_LEFT)
                        v.scale(0.9f);
                    else if (event.key.keysym.sym == SDLK_UP)
                        v.move({0.0f, -10.0f});
                    else if (event.key.keysym.sym == SDLK_DOWN)
                        v.move({0.0f, 10.0f});
                }
                    break;
                case SDL_QUIT:
                    done = true;
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x0, 0x0, 0xFF);

        SDL_RenderClear(sdl_renderer);
        /*
        std::vector<SDL_Point> points;
        for (auto const& p : v.vector_points)
        {
            points.push_back({(int)p.position.x, (int)p.position.y});
        }
        if (v.vector_points.size() > 2)
            points.push_back({(int)v.vector_points[0].position.x, (int)v.vector_points[0].position.y});

        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLines(sdl_renderer, points.data(), points.size());
        */

/*
        SDL_Point points[8] = {
            {(int)v.vector_points[0].position.x, (int)v.vector_points[0].position.y},
            {(int)v.vector_points[1].position.x, (int)v.vector_points[1].position.y},
            {(int)v.vector_points[2].position.x, (int)v.vector_points[2].position.y},
            {(int)v.vector_points[3].position.x, (int)v.vector_points[3].position.y},
            {(int)v.vector_points[4].position.x, (int)v.vector_points[4].position.y},
            {(int)v.vector_points[5].position.x, (int)v.vector_points[5].position.y},
            {(int)v.vector_points[6].position.x, (int)v.vector_points[6].position.y},
            {(int)v.vector_points[0].position.x, (int)v.vector_points[0].position.y}};

        */

        SDL_RenderPresent(sdl_renderer);

        // TODO cap FPS or something here
        std::this_thread::sleep_for(50ms);
    }

    return 0;
}
