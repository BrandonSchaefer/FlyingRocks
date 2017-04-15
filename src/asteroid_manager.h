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

#ifndef FLYING_ROCK_ASTEROID_MANAGER_H_
#define FLYING_ROCK_ASTEROID_MANAGER_H_

#include "asteroid.h"
#include "bullet.h"
#include "geometry.h"
#include "position_updater.h"
#include "sdl_renderer.h"

#include <list>
#include <random>

class ScoreObserver;

/*
TODO Need a way to:
     avoid spawning on the player
     when hit a player remove an asteroid
*/
class AsteroidMananger
{
public:
    explicit AsteroidMananger(Rectangle const& screen_size, int32_t starting_number = 5);

    void set_score_observer(ScoreObserver* observer);

    std::list<Asteroid>::iterator begin();
    std::list<Asteroid>::iterator end();

    std::list<Asteroid>::const_iterator cbegin() const;
    std::list<Asteroid>::const_iterator cend() const;

    std::list<Asteroid>::iterator remove_asteroid(std::list<Asteroid>::iterator const& it, Point const& avoid_center);

    void update(float delta);
    void update_position(PositionUpdater const& position_updater);
    void draw(SDLRenderer const& renderer) const;

private:
    void populate(Rectangle const& avoid_region);
    void respawn_asteroid(Asteroid const& asteroid_removed);

    int32_t starting_number;
    std::list<Asteroid> asteroids_;

    ScoreObserver* score_observer{nullptr};

    std::uniform_int_distribution<int32_t> random_x;
    std::uniform_int_distribution<int32_t> random_y;
};

#endif /* FLYING_ROCK_ASTEROID_MANAGER_H_ */
