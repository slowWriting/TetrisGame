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

#ifndef BRICKO_H
#define BRICKO_H

#include "Brick.h"

class BrickO:
        public Brick
{
public:
    BrickO(int cols, int rows): Brick(cols, rows) {
        brickPosition[0] = cols/2 - 1;
        brickPosition[1] = cols/2;
        brickPosition[2] = cols/2 - 1 + cols*1;
        brickPosition[3] = cols/2 + cols*1;
        color = 4;
    }
};

#endif // BRICKO_H
