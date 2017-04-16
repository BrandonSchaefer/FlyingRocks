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
#include "life_bar.h"
#include "position_updater.h"
#include "score.h"
#include "score_observer.h"
#include "vector_lines.h"
#include "timer.h"
#include "ship.h"
#include "text.h"

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
float const default_fps{59.5f};
//float const frames_per_second{144.0f};
float const one_second{1000.0f};

// Set viewport to smaller then this to get an unrendered section
Rectangle const default_size{{0, 0}, {800, 600}};

Vector const default_ship_acceleration{0, -500};

uint32_t const fps_font_size{36};
SDL_Color const fps_font_color{0x00, 0xFF, 0x00, 0xFF};

uint32_t const menu_font_size{36};
SDL_Color const menu_font_color{0xFF, 0xFF, 0xFF, 0xFF};
}

// TODO Clean this up man ... so ugly
int main(int argc, char* argv[])
{
    // FIXME move to a real random generator
    srand(time(nullptr));

    // TODO get fps from command line
    auto frames_per_second = default_fps;

    SDLBackend backend;
    SDLRenderer renderer("Flying Rocks", default_size.size);
    auto sdl_renderer = renderer.renderer();

    float width  = default_size.size.width;
    float height = default_size.size.height;

    AsteroidMananger asteroid_manager(default_size);
    BulletManager bullet_manager;
    PositionUpdater position_updater(default_size);

    ScoreObserver score_observer;
    // Magic starting pos
    Score score(&score_observer, &renderer, {20, 20});

    LifeBar life_bar;

    asteroid_manager.set_score_observer(&score_observer);

    Ship s({width/2, height/2}, default_ship_acceleration);

    Text fps_counter(&renderer, fps_font_color, fps_font_size);
    fps_counter.set_text(std::to_string(static_cast<int32_t>(default_fps)));

    Text game_over(&renderer, menu_font_color, menu_font_size);
    game_over.set_text("GAME OVER!");
    game_over.set_position({static_cast<int32_t>(width)  / 2 - game_over.text_size().width / 2,
                            static_cast<int32_t>(height) / 2 - game_over.text_size().height / 2});

    Text restart(&renderer, menu_font_color, menu_font_size);
    restart.set_text("Press Enter To Restart");
    restart.set_position({static_cast<int32_t>(width)  / 2 - restart.text_size().width / 2,
                          static_cast<int32_t>(height) / 2 + game_over.text_size().height * 4});

    // FIXME Bah hard coded - ish (mainly the spacing)
    fps_counter.set_position({static_cast<int32_t>(width) - fps_counter.text_size().width - 20, 20});

    float delta_time = 0.0f;
    Timer t;

    float current_second = 0.0f;
    uint32_t frames = 0;

    bool done = false;
    bool fps_enabled = true;

    bool shooting = false;
    bool paused = false;

    while (!done)
    {
        t.start();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                {
                    if (!paused)
                    {
                        if (event.key.keysym.sym == SDLK_SPACE)
                        {
                            // TODO move this to the ship possibly
                            shooting = true;
                        }
                    }

                    // TODO Lets move event handling inside objects such as the ship
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
                    else if (event.key.keysym.sym == SDLK_RETURN && paused)
                    {
                        // TODO Move most this to a main logic class that can be re-create to *reset* everything with
                        // out having to manually do it
                        asteroid_manager.reset();
                        bullet_manager.reset();
                        score.reset();
                        life_bar = LifeBar();
                        s.reset({width / 2, height / 2}, default_ship_acceleration);
                        paused = false;
                    }
                    break;
                }
                case SDL_KEYUP:
                {
                    if (event.key.keysym.sym == SDLK_RIGHT)
                    {
                        s.stop_turning();
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT)
                    {
                        s.stop_turning();
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        s.stop_thruster();
                    }
                    else if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        shooting = false;
                    }
                    else if (event.key.keysym.sym == SDLK_F10)
                    {
                        fps_enabled = !fps_enabled;
                    }
                    break;
                }
                case SDL_QUIT:
                    done = true;
                    break;
                default:
                    break;
            }
        }

        if (shooting)
        {
            bullet_manager.create_bullet(s.pos(), s.accel());
        }

        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x0, 0x0, 0xFF);

        SDL_RenderClear(sdl_renderer);

        // Update
        s.update(delta_time);
        bullet_manager.update(delta_time);
        asteroid_manager.update(delta_time);

        if (!paused)
        {
            // TODO Move this logic of the ship + asteroid and bullet + asteroid to some other place
            for (auto b_it = bullet_manager.begin(); b_it != bullet_manager.end(); ++b_it)
            {
                for (auto a_it = asteroid_manager.begin(); a_it != asteroid_manager.end(); ++a_it)
                {
                    auto bullet = *b_it;
                    Rectangle bullet_rect{{static_cast<int32_t>(bullet.position.x),
                                           static_cast<int32_t>(bullet.position.y)},
                                          {bullet.size.width, bullet.size.height}};

                    // TODO Turn the bullet rect into VectorLines or fix the colliding algorithm
                    if (a_it->shape.surrounding_rect().colliding(bullet_rect))
                    {
                        auto pos = Point{static_cast<int32_t>(s.pos().x),
                                         static_cast<int32_t>(s.pos().y)};
                        asteroid_manager.remove_asteroid(a_it, pos);
                        b_it = bullet_manager.erase(b_it);
                        break;
                    }
                }
            }
            
            if (!s.invulnerable())
            {
                for (auto it = asteroid_manager.begin(); it != asteroid_manager.end(); ++it)
                {
                    if (it->shape.colliding(s.ship_shape()))
                    {
                        s.reset({width / 2, height / 2}, default_ship_acceleration);

                        auto pos = Point{static_cast<int32_t>(s.pos().x),
                                         static_cast<int32_t>(s.pos().y)};

                        asteroid_manager.remove_asteroid(it, pos);
                        life_bar.remove_life();
                        break;
                    }
                }
            }
        }

        // Update Position based on locations
        s.update_position(position_updater);
        bullet_manager.update_position(position_updater);
        asteroid_manager.update_position(position_updater);

        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Draw
        s.draw(renderer);
        bullet_manager.draw(renderer);
        asteroid_manager.draw(renderer);
        score.draw(renderer);
        life_bar.draw(renderer);

        if (fps_enabled)
        {
            fps_counter.draw(renderer);
        }

        if (life_bar.dead())
        {
            game_over.draw(renderer);
            restart.draw(renderer);
            paused = true;
            //life_bar = LifeBar();
        }

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
            if (fps_enabled)
            {
                fps_counter.set_text(std::to_string(static_cast<int32_t>(frames)));

                // FIXME Bah hard coded - ish (mainly the spacing)
                fps_counter.set_position({static_cast<int32_t>(width) - fps_counter.text_size().width - 20, 20});
            }

            frames = 0;
            current_second = 0.0f;
        }
    }

    return 0;
}
