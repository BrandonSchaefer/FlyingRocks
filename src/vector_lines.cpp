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

#include "vector_lines.h"

#include <cmath>
#include <cstddef>
#include <limits>
#include <iostream>

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
    for (auto& vector_point : vector_points)
    {
        vector_point.direction *= scalar;
    }

    update_positions_from_direction();
}

void VectorLines::move(Vector const& direction)
{
    for (auto& vector_point : vector_points)
    {
        vector_point.position += direction;
    }
}

void VectorLines::rotate(float angle)
{
    //auto bounding_rect = surrounding_rect();
    //Vector middle = {static_cast<float>(bounding_rect.top_left.x + bounding_rect.size.width / 2),
                     //static_cast<float>(bounding_rect.top_left.y + bounding_rect.size.height / 2)};

    for (size_t i = 0; i < vector_points.size(); i++)
    {
        //vector_points[i].direction.rotate(angle, middle);
        vector_points[i].direction.rotate(angle);
    }

    update_positions_from_direction();
}

void VectorLines::set_position(Vector const& position)
{
    if (!vector_points.empty())
    {
        vector_points[0].position = position;

        update_positions_from_direction();
    }
}

void VectorLines::add_point(Vector const& point)
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
}

Rectangle VectorLines::surrounding_rect() const
{
    int32_t lowest_x  = std::numeric_limits<int32_t>::max();
    int32_t highest_x = 0;
    int32_t lowest_y  = std::numeric_limits<int32_t>::max();
    int32_t highest_y = 0;

    for (auto const& p : vector_points)
    {
        if (p.position.x < lowest_x)
        {
            lowest_x = p.position.x;
        }
        if (p.position.x > highest_x)
        {
            highest_x = p.position.x;
        }
        if (p.position.y < lowest_y)
        {
            lowest_y = p.position.y;
        }
        if (p.position.y > highest_y)
        {
            highest_y = p.position.y;
        }
    }

    return {{lowest_x, lowest_y},
            {highest_x - lowest_x, highest_y - lowest_y}};
}

void VectorLines::update_positions_from_direction()
{
    for (size_t i = 1; i < vector_points.size(); i++)
    {
        vector_points[i].position =
            vector_points[i - 1].position +
            vector_points[i - 1].direction;
    }

}

Vector VectorLines::first_position() const
{
    if (vector_points.empty())
    {
        throw std::runtime_error("Empty VectorLines");
    }

    return vector_points[0].position;
}

void VectorLines::draw(SDLRenderer const& renderer) const
{
    std::vector<Point> points;
    for (auto const& p : vector_points)
    {
        auto x = static_cast<int32_t>(std::round(p.position.x));
        auto y = static_cast<int32_t>(std::round(p.position.y));
        points.push_back({x, y});
    }

    if (!vector_points.empty())
    {
        auto x = static_cast<int32_t>(std::round(vector_points[0].position.x));
        auto y = static_cast<int32_t>(std::round(vector_points[0].position.y));
        points.push_back({x, y});
    }

    renderer.draw(points);
}
