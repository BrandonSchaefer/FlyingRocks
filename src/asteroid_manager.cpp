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

#include "asteroid_manager.h"
#include "default_asteroid_shapes.h"
#include "random_generator.h"

#include <cstdlib>
#include <functional>

namespace
{
std::vector<VectorLines> const asteroid_shapes{
    end_asteroid,
    snake_asteroid,
    cross_asteroid
};

int32_t const default_number_of_splits{3};

// FIXME Need better ranges, can have a 0,0 speed
std::uniform_real_distribution<float>  random_speed{-100.0f, 100.0f};
std::uniform_int_distribution<int32_t> random_rotation{0, 360};
std::uniform_int_distribution<int32_t> random_asteroid(0, asteroid_shapes.size() - 1);
}

AsteroidMananger::AsteroidMananger(Rectangle const& screen_size, int32_t starting_number) :
    starting_number(starting_number),
    random_x(0, screen_size.size.width),
    random_y(0, screen_size.size.height)
{
    populate();
}

void AsteroidMananger::populate()
{
    for (int32_t i = 0; i < starting_number; i++)
    {
        auto new_basic = asteroid_shapes[random_asteroid(mt())];
        new_basic.scale(20.0f);

        auto new_x = static_cast<float>(random_x(mt()));
        auto new_y = static_cast<float>(random_y(mt()));

        new_basic.set_position({new_x, new_y});
        new_basic.rotate(random_rotation(mt()));

        asteroids_.push_back({new_basic,
                             {random_speed(mt()), random_speed(mt())},
                             default_number_of_splits});
    }
}

std::list<Asteroid> AsteroidMananger::asteroids() const
{
    return asteroids_;
}

void AsteroidMananger::update(float delta)
{
    for (auto& a : asteroids_)
    {
        a.shape.move(a.direction * delta);

        // FIXME Need to rotate around the middle!
        a.shape.rotate(90.0f * delta);
    }
}

void AsteroidMananger::update_position(PositionUpdater const& position_updater)
{
    for (auto& a : asteroids_)
    {
        position_updater.update_vector_lines(a.shape);
    }
}

void AsteroidMananger::draw(SDLRenderer const& renderer) const
{
    for (auto const& a : asteroids_)
    {
        renderer.set_color({0xFF, 0xFF, 0xFF, 0xFF});
        a.shape.draw(renderer);

        //renderer.set_color({0x00, 0xFF, 0xFF, 0xFF});
        //auto r = a.shape.surrounding_rect().shrink(5.0f);
        //renderer.draw(r);
    }
}

bool AsteroidMananger::bullet_colliding(Bullet const& bullet)
{

    Rectangle bullet_rect{static_cast<int32_t>(bullet.position.x),
                          static_cast<int32_t>(bullet.position.y),
                          bullet.size.width, bullet.size.height};

    for (auto it = asteroids_.begin(); it != asteroids_.end(); ++it)
    {
        if (it->shape.surrounding_rect().colliding(bullet_rect))
        {
            // Create 2 new asteroids with a different shape then parent and different speeds
            Asteroid new_asteroid = *it;
            new_asteroid.number_of_splits--;

            if (new_asteroid.number_of_splits > 0)
            {
                new_asteroid.shape.scale(0.5f);
                //new_asteroid.shape = asteroid_shapes[random_asteroid(mt())];

                new_asteroid.direction = {random_speed(mt()) + 1.0f, random_speed(mt()) + 1.0f};
                asteroids_.push_back(new_asteroid);

                new_asteroid.direction = {random_speed(mt()) + 1.0f, random_speed(mt()) + 1.0f};
                asteroids_.push_back(new_asteroid);
            }

            asteroids_.erase(it);

            if (asteroids_.empty())
            {
                populate();
            }

            return true;
        }
    }

    return false;
}
