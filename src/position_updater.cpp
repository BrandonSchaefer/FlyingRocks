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

#include "position_updater.h"

#include <cmath>

namespace
{
int32_t mod(int32_t x, int32_t y)
{
    int m = x % y;
    if (m < 0.0f)
    {
        m += y;
    }

    return m;
}
}

PositionUpdater::PositionUpdater(Rectangle const& world_size) :
    world(world_size)
{
}

void PositionUpdater::update_vector_lines(VectorLines& lines) const
{
    auto main_point = lines.first_position();
    auto width  = world.size.width;
    auto height = world.size.height;

    if (main_point.x < 0 || main_point.x >= width ||
        main_point.y < 0 || main_point.y >= height)
    {
        auto new_x = static_cast<float>(mod(floor(main_point.x), width));
        auto new_y = static_cast<float>(mod(floor(main_point.y), height));
        lines.set_position({new_x, new_y});
    }
}

void PositionUpdater::update_vector(Vector& position) const
{
    auto width  = world.size.width;
    auto height = world.size.height;

    if (position.x < 0 || position.x >= width ||
        position.y < 0 || position.y >= height)
    {
        position.x = static_cast<float>(mod(floor(position.x), width));
        position.y = static_cast<float>(mod(floor(position.y), height));
    }
}
