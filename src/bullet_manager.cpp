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

#include "bullet_manager.h"

#include <algorithm>

namespace
{
Size const size{3, 3};
float const lifetime{1.0f};
float const speed_mag{1000.0f};
float const reload_time{0.2f};
}

void BulletManager::create_bullet(Vector const& position, Vector const& direction)
{
    if (reloading <= 0.0f)
    {
        auto speed = direction;
        speed.set_magnitude(speed_mag);

        bullets_.push_back({position, speed, size, lifetime});

        reloading = reload_time;
    }
}

void BulletManager::reset()
{
    bullets_.clear();
}

void BulletManager::update(float delta)
{
    for (auto it = bullets_.begin(); it != bullets_.end(); ++it)
    {
        it->lifetime -= delta;
        it->position += it->speed * delta;

        if (it->lifetime < 0.0f)
        {
            it = bullets_.erase(it);
        }
    }

    reloading -= delta;
}

void BulletManager::update_position(PositionUpdater const& position_updater)
{
    for (auto& b : bullets_)
    {
        position_updater.update_vector(b.position);
    }
}

void BulletManager::draw(SDLRenderer const& renderer)
{
    std::vector<Rectangle> rects;
    for (auto const& b : bullets_)
    {
        auto new_x = static_cast<int32_t>(b.position.x);
        auto new_y = static_cast<int32_t>(b.position.y);
        rects.push_back({{new_x, new_y}, {b.size.width, b.size.height}});
    }

    renderer.draw(rects);
}

std::list<Bullet>::iterator BulletManager::begin()
{
    return bullets_.begin();
}

std::list<Bullet>::const_iterator BulletManager::cbegin()
{
    return bullets_.cbegin();
}

std::list<Bullet>::iterator BulletManager::end()
{
    return bullets_.end();
}

std::list<Bullet>::const_iterator BulletManager::cend()
{
    return bullets_.cend();
}

std::list<Bullet>::iterator BulletManager::erase(std::list<Bullet>::iterator const& it)
{
    return bullets_.erase(it);
}
