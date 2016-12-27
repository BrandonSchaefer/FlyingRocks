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

#include "asteroid_manager.h"
#include "bullet_manager.h"
#include "position_updater.h"
#include "vector_lines.h"
#include "timer.h"
#include "ship.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include <SDL2/SDL.h>

// TODO Reomve possibly
using namespace std::chrono_literals;

// for debuging
std::ostream& operator<<(std::ostream& out, Vector const& v)
{
  out << "{" << std::fixed << v.x << ", " << v.y << "}";
  return out;
}

namespace
{
// FIXME Should depend on the display but 59.5 is fine for now
float const frames_per_second = 59.5f;
float const one_second = 1000.0f;

// Set viewport to smaller then this to get an unrendered section
Rectangle const default_size{{0, 0}, {800, 600}};

}

int main()
{
    // FIXME move to a real random generator
    srand(time(nullptr));

    //float width = 800;
    //float height = 600;

    // expand size so we can cheap warp
    auto expanded = default_size;
    // Use ship size? Or largest object size
    //expanded.expand(50);

    SDLBackend backend;
    SDLRenderer renderer("Flying Rocks", default_size.size);
    auto sdl_renderer = renderer.renderer();
    SDL_Rect viewport{expanded.top_left.x, expanded.top_left.y, expanded.size.width, expanded.size.height};
    SDL_RenderSetViewport(sdl_renderer, &viewport);

    float width  = expanded.size.width;
    float height = expanded.size.height;

    AsteroidMananger asteroid_manager(default_size);
    BulletManager bullet_manager;
    PositionUpdater position_updater(expanded);

    Ship s({width/2, height/2}, {0.0, -7.5});

    float delta_time = 0.0f;
    Timer t;

    float current_second = 0.0f;
    uint32_t frames = 0;

    bool done = false;
    while (!done)
    {
        t.start();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONUP:
                {
                    float x = event.button.x;
                    float y = event.button.y;
                    s.ship.set_position({x, y});
                }
                    break;
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_UP)
                    {
                        s.start_thruster();
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT && event.key.repeat == 0)
                    {
                        s.start_turning(TurnDirection::right);
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT && event.key.repeat == 0)
                    {
                        s.start_turning(TurnDirection::left);
                    }
                    else if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        bullet_manager.create_bullet(s.pos(), s.accel());
                    }
                    break;
                }
                case SDL_KEYUP:
                {
                    if (event.key.keysym.sym == SDLK_RIGHT)
                        s.stop_turning();
                    else if (event.key.keysym.sym == SDLK_LEFT)
                        s.stop_turning();
                    else if (event.key.keysym.sym == SDLK_UP)
                        s.stop_thruster();
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

        // Update
        s.update(delta_time);
        bullet_manager.update(delta_time);
        asteroid_manager.update(delta_time);

        // TODO The ship should hold onto the *gun*. How could we do collision then?
        for (auto it = bullet_manager.begin(); it != bullet_manager.end(); ++it)
        {
            if (asteroid_manager.bullet_colliding(*it))
            {
                it = bullet_manager.erase(it);
            }
        }
        
        for (auto const& a : asteroid_manager.asteroids())
        {
            auto asteroid_rect = a.shape.surrounding_rect();
            if (s.ship.surrounding_rect().colliding(asteroid_rect))
            {
                printf("YOU LOSE\n");
            }
        }

        // Update Position based on locations
        s.update_position(position_updater);
        bullet_manager.update_position(position_updater);
        asteroid_manager.update_position(position_updater);

        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Draw
        s.ship.draw(renderer);
        bullet_manager.draw(renderer);
        asteroid_manager.draw(renderer);

        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
        auto r = s.ship.surrounding_rect();
        SDL_Rect ship_rect{r.top_left.x, r.top_left.y, r.size.width, r.size.height};

        SDL_RenderDrawRect(sdl_renderer, &ship_rect);

        SDL_RenderPresent(sdl_renderer);

        // TODO Clean this FPS into a class or something
        if (t.elapsed().count() < one_second / frames_per_second)
        {
            std::this_thread::sleep_for(1000ms / frames_per_second - t.elapsed());
        }

        delta_time = t.elapsed().count() / one_second;
        current_second += delta_time;
        frames++;

        if (current_second >= 1.0f)
        {
            std::cout << frames << " frames per second " << std::endl;
            frames = 0;
            current_second = 0.0f;
        }
    }

    return 0;
}
