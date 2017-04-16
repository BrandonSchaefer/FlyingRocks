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

#ifndef FLYING_ROCK_SDL_TEXTURE_H_
#define FLYING_ROCK_SDL_TEXTURE_H_

#include "geometry.h"

#include <SDL2/SDL.h>

#include <memory>

class SDLRenderer;

// TODO Expand this as needed
class SDLTexture
{
public:
    SDLTexture() = default;

    // Takes ownership of the pointer
    SDLTexture(SDL_Texture* texture);

    void set_position(Point const& pos);
    void set_size(Size const& size);

    Size texture_size() const;

    void draw(SDLRenderer const& renderer) const;

private:
    Point top_left{0, 0};
    Size size{0, 0};

    std::shared_ptr<SDL_Texture> texture{nullptr};
};

#endif /* FLYING_ROCK_SDL_TEXTURE_H_ */
