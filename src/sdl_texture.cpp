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

#include "sdl_texture.h"
#include "sdl_renderer.h"

namespace
{
void sdl_texture_deleter(SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
}
}

SDLTexture::SDLTexture(SDL_Texture* texture) :
    texture{std::shared_ptr<SDL_Texture>(texture, sdl_texture_deleter)}
{
    if (texture == nullptr)
    {
        throw std::runtime_error("Failed to create texture");
    }
}

void SDLTexture::set_position(Point const& pos)
{
    top_left = pos;
}

void SDLTexture::set_size(Size const& in_size)
{
    size = in_size;
}

Size SDLTexture::texture_size() const
{
    return size;
}

void SDLTexture::draw(SDLRenderer const& renderer) const
{
    SDL_Rect rect{top_left.x, top_left.y, size.width, size.height};

    SDL_RenderCopy(renderer.renderer(), texture.get(), nullptr, &rect);
}
