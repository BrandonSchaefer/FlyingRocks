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

bool PositionUpdater::vector_lines_offscreen(VectorLines& lines) const
{
    auto width  = world.size.width;
    auto height = world.size.height;

    for (auto const& p : lines.positions())
    {
        if (p.x >= 0 && p.x < width &&
            p.y >= 0 && p.y < height)
        {
            return false;
        }
    }

    return true;
}

bool PositionUpdater::update_vector_lines(VectorLines& lines) const
{
    auto width  = world.size.width;
    auto height = world.size.height;

    bool offscreen = vector_lines_offscreen(lines);

    if (offscreen)
    {
        auto main_point = lines.first_position();

        auto new_x = static_cast<float>(mod(floor(main_point.x), width));
        auto new_y = static_cast<float>(mod(floor(main_point.y), height));

        for (auto const& p : lines.positions())
        {
            // Left
            if (p.x < 0)
            {
                new_x = width + main_point.x + lines.surrounding_rect().size.width;
                break;
            }
            // Right
            else if (p.x > width)
            {
                new_x -= lines.surrounding_rect().size.width;
                break;
            }

            // Top
            if (p.y < 0)
            {
                new_y = height + main_point.y + lines.surrounding_rect().size.height;
                break;
            }
            // Down
            else if (p.y > height)
            {
                new_y -= lines.surrounding_rect().size.height;
                break;
            }
        }

        lines.set_position({new_x, new_y});

        return true;
    }

    return false;
}

bool PositionUpdater::update_vector(Vector& position) const
{
    auto width  = world.size.width;
    auto height = world.size.height;

    if (position.x < 0 || position.x >= width ||
        position.y < 0 || position.y >= height)
    {
        position.x = static_cast<float>(mod(floor(position.x), width));
        position.y = static_cast<float>(mod(floor(position.y), height));

        return true;
    }

    return false;
}
