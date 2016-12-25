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

#include "ship.h"

#include <iostream>

namespace
{
int const size = 50;
float const friction = 0.75f;
}

Ship::Ship(Vector const& position, Vector const& acceleration) :
    ship({position,
         {position.x - size / 4, position.y + size},
         {position.x - size / 4, position.y + size},
         {position.x + size / 4, position.y + size}
         //{position.x + 20, position.y + 20},
         //{position.x + 20, position.y}}
    }),
    acceleration(acceleration),
    position(position)
{
}

void Ship::update(float delta)
{
    switch (turn_direction)
    {
        case TurnDirection::left:
        {
            acceleration.rotate(-360 * delta);
            ship.rotate(-360 * delta);
            //ship.scale(1 - 1 * delta);
            break;
        }
        case TurnDirection::right:
        {
            acceleration.rotate(360 * delta);
            ship.rotate(360 * delta);
            //ship.scale(1 + 1 * delta);
            break;
        }
        default:
            break;
    }

    if (thruster_on && velocity.magnitude() < acceleration.magnitude())
    {
        velocity += acceleration * delta;
    }
    else
    {
        velocity += (-velocity * (friction * delta));
    }

    ship.move(velocity);
    position = ship.vector_points[0].position;
}

void Ship::update_position(PositionUpdater const& position_updater)
{
    position_updater.update_vector_lines(ship);
}

void Ship::start_turning(TurnDirection dir)
{
    turn_direction = dir;
}

void Ship::stop_turning()
{
    turn_direction = TurnDirection::none;
}

void Ship::start_thruster()
{
    thruster_on = true;
}

void Ship::stop_thruster()
{
    thruster_on = false;
}

Vector Ship::pos() const
{
    return position;
}

Vector Ship::accel() const
{
    return acceleration;
}
