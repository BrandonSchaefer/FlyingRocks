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

#include "vector_line_graphic.h"

#include <cstddef>

/* Set the position for each point
 * Each vector direction = next_point.position % points - current_point.position
 */
VectorLines::VectorLines(std::vector<Vector> const& points)
{
    // If there is only one point in the list, the direction will be 0.0f
    // which will be updated correctly on an add_point
    for (size_t i = 0; i < points.size(); i++)
    {
        vector_points.push_back(
            {points[i],
             points[(i + 1) % points.size()] - points[i]});
    }
}

/*
 * Go through all directions, apply a dot scalar to it. Go through
 * and then regenerate positions based on the new vectors directions
 */
void VectorLines::scale(float scalar)
{
    for (auto& p : vector_points)
    {
        p.direction *= scalar;
    }

    for (size_t i = 1; i < vector_points.size(); i++)
    {
        vector_points[i].position =
            vector_points[i - 1].position +
            vector_points[i - 1].direction;
    }
}

void VectorLines::move(Vector const& direction)
{
    for (auto& p : vector_points)
    {
        p.position += direction;
    }
}

VectorLines& VectorLines::add_point(Vector const& point)
{
    // TODO Clean this up a bit
    if (vector_points.size() >= 1)
    {
        // Make the last point, point to the first point
        vector_points.push_back(
            {point,
             vector_points[0].position - point});

        // Update the last points direction to now point to our new point
        vector_points[vector_points.size() - 2].direction =
            point - vector_points[vector_points.size() - 2].position;
    }
    else
    {
        vector_points.push_back({point, {0.0f, 0.0f}});
    }

    return *this;
}
