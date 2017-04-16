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

#include "sdl_renderer.h"
#include "text.h"

namespace
{
// TODO Remove hardcoded bits or packages the font with this
std::string const ttf_path{"/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf"};

void font_deleter(TTF_Font* font)
{
    TTF_CloseFont(font);
}
}

Text::Text(SDLRenderer* renderer, SDL_Color const& color, uint32_t font_size) :
    font(TTF_OpenFont(ttf_path.c_str(), font_size), font_deleter),
    renderer(renderer),
    color(color)
{
    if (font == nullptr)
    {
        throw std::runtime_error("Failed to load font: " + std::string(TTF_GetError()));
    }
}

void Text::set_position(Point const& pos)
{
    position = pos;
    text_texture.set_position(position);
}

void Text::set_text(std::string const& text)
{
    current_text = text;
    text_texture = generate_text_texture();
}

void Text::set_color(SDL_Color const& in_color)
{
    color = in_color;
    text_texture = generate_text_texture();
}

void Text::draw(SDLRenderer const& renderer)
{
    text_texture.draw(renderer);
}

Size Text::text_size() const
{
    return text_texture.texture_size();
}

SDLTexture Text::generate_text_texture() const
{
    auto surface = TTF_RenderText_Solid(font.get(), current_text.c_str(), color);

    if (surface == nullptr)
    {
        throw std::runtime_error("Failed to render text: " + std::string(TTF_GetError()));
    }

    auto texture = SDLTexture(SDL_CreateTextureFromSurface(renderer->renderer(), surface));

    texture.set_size({surface->w, surface->h});
    texture.set_position(position);
    SDL_FreeSurface(surface);

    return texture;
}
