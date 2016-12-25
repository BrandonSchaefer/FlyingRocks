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

#ifndef ASTEROIDS_SDL_RENDERER_H_
#define ASTEROIDS_SDL_RENDERER_H_

#include <string>
#include <memory>
#include <vector>

#include "color.h"
#include "geometry.h"

class SDL_Window;
class SDL_Renderer;

typedef std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>     SDLWindowUPtr;
typedef std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> SDLRendererUPtr;

class SDLRenderer
{
public:
    SDLRenderer(std::string const& title, Size const& window_size);

    void set_color(Color const& color) const;

    void draw(Point const& p1, Point const& p2) const;
    void draw(std::vector<Point> const& points) const;

    void draw(Rectangle const& rect) const;
    void draw_solid(Rectangle const& rect) const;

    // TODO Wrap these up in real classes/functions to avoid needing to expose SDL2
    SDL_Window* window() const;
    SDL_Renderer* renderer() const;

private:
    Size window_size;

    SDLWindowUPtr window_;
    SDLRendererUPtr renderer_;
};

#endif /* ASTEROIDS_SDL_RENDERER_H_ */
