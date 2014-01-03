/*
  Copyright (C) 2013 Maciej Regulski
  Contact: Maciej Regulski <regulski.maciej@gmail.com>
  All rights reserved.

  This file is part of Tetris Game.
  Tetris Game is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Tetris Game is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Tetris Game.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BRICK_H
#define BRICK_H
#define BRICKCOUNT 4

// Parent Class for all bricks, avoid using it
class Brick {

protected:
    int _maxCols; // max column count
    int _maxRows; // max rows count

public:
    // Pointer to table holding tile number
    int brickPosition[BRICKCOUNT];
    int color;

    /* Sets _curCol to half of _maxCol
    * rows is max rows count
    * cols is max column count
    */
    Brick(int cols, int rows): _maxCols(cols), _maxRows(rows) {
    }

    Brick(const Brick& arg): _maxCols(arg._maxCols),
        _maxRows(arg._maxRows), color(arg.color) {

        for (int i = 0; i < BRICKCOUNT; ++i)
            brickPosition[i] = arg.brickPosition[i];
    }

    virtual ~Brick() {}

};

#endif // BRICK_H
