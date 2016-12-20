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

#include "sdl_error.h"
#include "sdl_renderer.h"

#include <SDL2/SDL.h>

namespace
{
void sdl_window_destroy(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}

void sdl_renderer_destroy(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}
}

SDLRenderer::SDLRenderer(std::string const& title, Size const& window_size) :
    window_size(window_size),
    window_(nullptr, sdl_window_destroy),
    renderer_(nullptr, sdl_renderer_destroy)
{
    auto new_window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_size.width, window_size.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (new_window == nullptr)
    {
        throw SDLError("Failed to create SDL Widnow");
    }

    window_ = SDLWindowUPtr(new_window, sdl_window_destroy);

    auto new_renderer = SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED);

    if (new_renderer == nullptr)
    {
        throw SDLError("Failed to create SDL Renderer");
    }

    renderer_ = SDLRendererUPtr(new_renderer, sdl_renderer_destroy);
}

SDL_Window* SDLRenderer::window() const
{
    return window_.get();
}

SDL_Renderer* SDLRenderer::renderer() const
{
    return renderer_.get();
}
