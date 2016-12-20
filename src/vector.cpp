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


#include "vector.h"

#include <cmath>

#include <iostream>

Vector::Vector(float x, float y, float z)
  : x(x)
  , y(y)
  , z(z)
{
}

Vector Vector::operator+(Vector const& v) const
{
    return {x + v.x, y + v.y, z + v.z};
}

Vector Vector::operator-(Vector const& v) const
{
    return {x - v.x, y - v.y, z - v.z};
}

Vector Vector::operator*(float n) const
{
    return {x * n, y * n, z * n};
}

Vector Vector::operator/(float n) const
{
    return {x / n, y / n, z / n};
}

Vector& Vector::operator+=(Vector const& v)
{
    *this = *this + v;
    return *this;
}

Vector& Vector::operator-=(Vector const& v)
{
    *this = *this - v;
    return *this;
}

Vector& Vector::operator*=(float n)
{
    *this = *this * n;
    return *this;
}

Vector& Vector::operator/=(float n)
{
    *this = *this / n;
    return *this;
}

float Vector::magnitude() const
{
    return sqrtf(x * x + y * y + z * z);
}

void Vector::normalize()
{
    float m = magnitude();
    if (m > 0 || m < 0)
        *this = *this / m;
}

void Vector::set_magnitude(float mag)
{
    normalize();
    *this *= mag;
}

void Vector::limit(float max)
{
    if (magnitude() > max)
        set_magnitude(max);
}

// FIXME just rotating on x/y
void Vector::rotate(float angle)
{
    float rad = angle * M_PI / 180;
    float cs = cosf(rad);
    float sn = sinf(rad);

    float new_x = x * cs - y * sn;
    float new_y = x * sn + y * cs;

    x = new_x;
    y = new_y;
}
