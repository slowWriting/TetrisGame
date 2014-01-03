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

#ifndef TETRISENGINE_H
#define TETRISENGINE_H

#define START_INTERVAL 1000
#define INTERVAL_STEP 80
#define ROWS 20
#define COLS 10

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "bricks/BricksAll.h"
#include "tiledata.h"

class TetrisEngine: public QObject {
    Q_OBJECT

public:
    TetrisEngine ();
    ~TetrisEngine();

    Q_PROPERTY(QQmlListProperty<TileData> tiles READ tiles CONSTANT)

    Q_PROPERTY(bool endOfGame READ endOfGame NOTIFY endOfGameChanged)
    bool endOfGame() const { return _endofgame; }

    Q_PROPERTY(int linesCounter READ linesCounter NOTIFY linesCounterChanged)
    int linesCounter() const { return _linesCounter; }

    Q_PROPERTY(int curLevel READ curLevel NOTIFY nextLevel)
    int curLevel() const { return _curLevel; }

    QQmlListProperty<TileData> tiles();

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void rotate();
    Q_INVOKABLE void moveFastDown(bool value);
    Q_INVOKABLE bool spawnBrick();
    Q_INVOKABLE void restart();

    bool moveDown();

signals:
    void endOfGameChanged();
    void linesCounterChanged();
    void nextLevel();
    void timeout();

private:

    bool checkCollision(int tileNum);
    void checkForLine();
    void addLine();
    Brick* createNewBrick();
    QColor* resolveBrickColor(int brickNumber);
    int* rotateBrick(int direction);

    void setEndOfGame(bool value) { _endofgame = value; if (value == true) emit endOfGameChanged(); timer.stop();}

    QList<TileData *> _tiles;
    Brick* _nextBrick;
    Brick* _curBrick;
    QTimer timer; // should be in another thread, because spamming rotation can pause game
    int _randomTable[7];
    int _rotatePos;
    int _numRows;
    int _numCols;
    int _curCol;
    int _curRow;
    int _endofgame;
    int _linesCounter;
    int _curLevel;
    int _interval; // interval of timer in ms


private slots:
    void timer_function();

};

#endif // TETRISENGINE_H
