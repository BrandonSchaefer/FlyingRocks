//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Brandon Schaefer
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

#include "life_bar.h"
#include "default_ship_shape.h"

namespace
{
float const spacing{5.0f};
}

LifeBar::LifeBar(int starting_lifes)
{
    for (int i = 0; i < starting_lifes; i++)
    {
        lifes.push_back(default_ship);
        lifes[i].scale(2.5f);
        // TODO Fix the hardcoded bits. Need to get the height from the score
        lifes[i].set_position({i * (lifes[i].surrounding_rect().size.width + spacing) + 30.0f, 60.0f});
    }
}

bool LifeBar::dead() const
{
    return lifes.empty();
}

void LifeBar::remove_life()
{
    lifes.pop_back();
}

void LifeBar::set_position(Point const& position)
{
    life_position = position;
}

void LifeBar::draw(SDLRenderer const& renderer) const
{
    for (auto const& life : lifes)
    {
        life.draw(renderer);
    }
}
