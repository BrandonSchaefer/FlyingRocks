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


#ifndef FLYING_ROCK_DEFAULT_SHIP_SHAPE_H_
#define FLYING_ROCK_DEFAULT_SHIP_SHAPE_H_

#include "vector_lines.h"

/*
ship
         0
        / \
       /   \
      /     \
     2-------3
    /         \
    1         4

ship moving - need to go from 5 -> 3, then 3 -> 6
         0
        / \
       /   \
      /     \
     2--3-5--6
    /   \/    \
    1   \/      7
        4
*/

VectorLines const default_ship = {{
    {4.0f, 0.0f},  // 0
    {1.0f, 10.0f}, // 1
    {2.0f, 8.0f},  // 2
    {6.0f, 8.0f},  // 3
    {7.0f, 10.0f}  // 4
}};

VectorLines const default_ship_moving = {{
    {4.0f, 0.0f},  // 0
    {1.0f, 10.0f}, // 1
    {2.0f, 8.0f},  // 2
    {3.0f, 8.0f},  // 3
    {4.0f, 11.0f}, // 4
    {5.0f, 8.0f},  // 5
    {3.0f, 8.0f},  // 3
    {6.0f, 8.0f},  // 6
    {7.0f, 10.0f}  // 7
}};

#endif /* FLYING_ROCK_DEFAULT_SHIP_SHAPE_H_ */
