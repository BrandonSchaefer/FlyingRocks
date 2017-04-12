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

#include "score.h"
#include "score_observer.h"

namespace
{
uint32_t const default_font_size{36};
std::string const ttf_path{"/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf"};
SDL_Color const default_color{0xFF, 0xFF, 0xFF, 0xFF};

struct AccumulatorScoreObserver : ScoreObserver
{
    AccumulatorScoreObserver(Score* score) :
        score(score)
    {
    }

    void add_score(uint32_t amount) const override
    {
        score->add_score(amount);
    }

    Score* score;
};
}

Score::Score(ScoreObserver* in_observer, SDLRenderer* renderer) :
    observer(std::make_unique<AccumulatorScoreObserver>(this)),
    font(TTF_OpenFont(ttf_path.c_str(), default_font_size)),
    renderer(renderer)
{
    if (font == nullptr)
    {
        throw std::runtime_error("Failed to load font: " + std::string(TTF_GetError()));
    }

    // FIXME Magic
    score_texture.set_position({20, 20});

    in_observer->add_observer(observer.get());
    create_score_texture();
}

void Score::create_score_texture()
{
    auto score_surface = TTF_RenderText_Solid(font, std::to_string(total_score).c_str(), default_color);

    if (score_surface == nullptr)
    {
        throw std::runtime_error("Failed to render text: " + std::string(TTF_GetError()));
    }

    score_texture.create_texture_from_surface(renderer, score_surface);
    SDL_FreeSurface(score_surface);
}

void Score::add_score(uint32_t amount)
{
    total_score += amount;

    create_score_texture();
}

void Score::draw(SDLRenderer const& renderer)
{
    score_texture.draw(renderer);
}
