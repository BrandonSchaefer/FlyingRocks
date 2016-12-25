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
float const speed_mag{15.0f};
}

void BulletManager::create_bullet(Vector const& position, Vector const& direction)
{
    auto speed = direction;
    speed.set_magnitude(speed_mag);

    bullets_.push_back({position, speed, size, lifetime});
}

void BulletManager::update(float delta)
{
    for (auto& b : bullets_)
    {
        b.lifetime -= delta;
        b.position += b.speed;
    }

    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
        [](Bullet const& b) { return b.lifetime <= 0.0f; }), bullets_.end());
}

std::vector<Bullet> BulletManager::bullets() const
{
    return bullets_;
}
