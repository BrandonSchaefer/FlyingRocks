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
#include "default_ship_shape.h"

namespace
{
float const friction{0.75f};
float const default_invulnerable_time{1.5f};

// FIXME This should be part of an animation thing or something?
float const blink_speed(0.05f);
float const blink_speed_invulnerable(0.1f);
}

Ship::Ship(Vector const& position, Vector const& acceleration) :
    acceleration(acceleration),
    ship(default_ship),
    ship_moving(default_ship_moving)
{
    reset(position, acceleration);
}

void Ship::update(float delta)
{
    switch (turn_direction)
    {
        case TurnDirection::left:
        {
            acceleration.rotate(-360 * delta);
            ship.rotate(-360 * delta);
            ship_moving.rotate(-360 * delta);
            break;
        }
        case TurnDirection::right:
        {
            acceleration.rotate(360 * delta);
            ship.rotate(360 * delta);
            ship_moving.rotate(360 * delta);
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
        velocity += -velocity * (friction * delta);
    }

    if (invulnerable_time >= 0.0f)
    {
        invulnerable_time -= delta;
    }

    if (invulnerable() || thruster_on)
    {
        blink_timer -= delta;
        if (blink_timer <= 0.0f)
        {
            should_blink = !should_blink;
            if (invulnerable())
            {
                blink_timer = blink_speed_invulnerable;
            }
            else
            {
                blink_timer = blink_speed;
            }
        }
    }

    ship.move(velocity * delta);
    ship_moving.move(velocity * delta);
}

void Ship::update_position(PositionUpdater const& position_updater)
{
    if (position_updater.update_vector_lines(ship))
    {
        ship_moving.set_position(ship.first_position());
    }
}

void Ship::draw(SDLRenderer const& renderer) const
{
    if (!invulnerable() || !should_blink)
    {
        // TODO Make a blinker class
        if (thruster_on)// && should_blink)
        {
            ship_moving.draw(renderer);
            //renderer.draw(ship_moving.surrounding_rect());
        }
        else
        {
            ship.draw(renderer);
        }

        //renderer.draw(ship.surrounding_rect().shrink(5.0f));
    }
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

void Ship::reset(Vector const& pos, Vector const& accel)
{
    invulnerable_time = default_invulnerable_time;

    ship = VectorLines(default_ship);
    ship.scale(5.0f);
    ship.set_position(pos);

    ship_moving = VectorLines(default_ship_moving);
    ship_moving.scale(5.0f);
    ship_moving.set_position(pos);

    velocity = 0.0f;
    acceleration = accel;
}

bool Ship::invulnerable() const
{
    return invulnerable_time > 0.0f;
}

Vector Ship::pos() const
{
    return ship.first_position();
}

Vector Ship::accel() const
{
    return acceleration;
}

// Never return the ship_moving as we dont want the thurster to be part of the hit box
VectorLines Ship::ship_shape() const
{
    return ship;
}
