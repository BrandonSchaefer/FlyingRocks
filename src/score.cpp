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
#include "sdl_renderer.h"

namespace
{
SDL_Color const score_color{0xFF, 0xFF, 0xFF, 0xFF};
uint32_t const score_font_size{36};

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

Score::Score(ScoreObserver* in_observer, SDLRenderer* renderer, Point const& pos) :
    observer(std::make_unique<AccumulatorScoreObserver>(this)),
    score_text(renderer, score_color, score_font_size)
{
    score_text.set_position(pos);
    score_text.set_text(std::to_string(total_score));
    in_observer->add_observer(observer.get());
}

void Score::add_score(uint32_t amount)
{
    total_score += amount;
    score_text.set_text(std::to_string(total_score));
}

void Score::reset()
{
    total_score = 0;
    score_text.set_text(std::to_string(total_score));
}

void Score::draw(SDLRenderer const& renderer)
{
    score_text.draw(renderer);
}
