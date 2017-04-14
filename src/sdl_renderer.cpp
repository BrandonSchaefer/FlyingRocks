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

void SDLRenderer::set_color(Color const& color) const
{
    SDL_SetRenderDrawColor(renderer_.get(), color.red, color.green, color.blue, color.alpha);
}

void SDLRenderer::draw(Point const& p1, Point const& p2) const
{
    SDL_RenderDrawLine(renderer_.get(), p1.x, p1.y, p2.x, p2.y);
}

void SDLRenderer::draw(std::vector<Point> const& points) const
{
    std::vector<SDL_Point> sdl_points;
    for (auto const& p : points)
    {
        sdl_points.push_back({p.x, p.y});
    }

    SDL_RenderDrawLines(renderer_.get(), sdl_points.data(), sdl_points.size());
}

void SDLRenderer::draw(std::vector<Rectangle> const& rect) const
{
    std::vector<SDL_Rect> sdl_rects;
    for (auto const& r : rect)
    {
        sdl_rects.push_back({r.top_left.x, r.top_left.y, r.size.width, r.size.height});
    }

    SDL_RenderDrawRects(renderer_.get(), sdl_rects.data(), sdl_rects.size());
}

void SDLRenderer::draw(Rectangle const& rect) const
{
    SDL_Rect sdl_rect{rect.top_left.x, rect.top_left.y, rect.size.width, rect.size.height};
    SDL_RenderDrawRect(renderer_.get(), &sdl_rect);
}

void SDLRenderer::draw_solid(Rectangle const& rect) const
{
    SDL_Rect sdl_rect{rect.top_left.x, rect.top_left.y, rect.size.width, rect.size.height};
    SDL_RenderFillRect(renderer_.get(), &sdl_rect);
}

SDL_Window* SDLRenderer::window() const
{
    return window_.get();
}

SDL_Renderer* SDLRenderer::renderer() const
{
    return renderer_.get();
}

SDLTexture SDLRenderer::create_texture_from_surface(SDL_Surface* surface) const
{
    SDLTexture texture(SDL_CreateTextureFromSurface(renderer_.get(), surface));

    texture.set_size({surface->w, surface->h});

    return texture;
}
