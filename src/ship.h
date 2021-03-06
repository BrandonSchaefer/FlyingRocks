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

#ifndef FLYING_ROCK_SHIP_H_
#define FLYING_ROCK_SHIP_H_

#include "position_updater.h"
#include "sdl_renderer.h"
#include "vector.h"
#include "vector_lines.h"

enum TurnDirection
{
    none,
    right,
    left
};

class Ship
{
public:
    Ship(Vector const& position, Vector const& acceleration);

    void update(float delta);
    void update_position(PositionUpdater const& position_updater);

    void draw(SDLRenderer const& renderer) const;

    void start_thruster();
    void stop_thruster();

    void start_turning(TurnDirection dir);
    void stop_turning();

    //void set_position(Vector const& position);
    void reset(Vector const& position, Vector const& acceleration);

    bool invulnerable() const;

    Vector pos() const;
    Vector accel() const;

    VectorLines ship_shape() const;

private:
    Vector acceleration;
    Vector velocity;
    VectorLines ship;
    VectorLines ship_moving;

    bool thruster_on{false};

    float invulnerable_time{0.0f};

    // TODO create an animation class
    float blink_timer{0.0f};
    bool should_blink{false};

    TurnDirection turn_direction{TurnDirection::none};
};

#endif /* FLYING_ROCK_SHIP_H_ */
