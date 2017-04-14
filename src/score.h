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

#ifndef FLYING_ROCK_SCORE_H_
#define FLYING_ROCK_SCORE_H_

#include <memory>
#include <SDL2/SDL_ttf.h>

#include "sdl_texture.h"

class ScoreObserver;
class SDLRenderer;

class Score
{
public:
    Score(ScoreObserver* observer, SDLRenderer* renderer, Point const& point);

    void add_score(uint32_t amount);

    void draw(SDLRenderer const& renderer);

private:
    SDLTexture create_score_texture() const;

    std::unique_ptr<ScoreObserver> observer;
    uint32_t total_score{0};
    Point score_position;

    std::shared_ptr<TTF_Font> font{nullptr};
    SDLRenderer* renderer{nullptr};
    SDLTexture score_texture;
};

#endif /* FLYING_ROCK_SCORE_H_ */
