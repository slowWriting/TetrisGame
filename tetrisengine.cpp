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

#include "tetrisengine.h"

//Method for adding class to qt property list
void tilesPropAppend(QQmlListProperty<TileData>* prop, TileData* value)
{
    Q_UNUSED(prop);
    Q_UNUSED(value);
    return; //Append not supported
}

//Method for getting number of elements in qt property list
int tilesPropCount(QQmlListProperty<TileData>* prop)
{
    return static_cast<QList<TileData*>*>(prop->data)->count();
}

//Method for getting number of elements in qt property list
TileData* tilesPropAt(QQmlListProperty<TileData>* prop, int index)
{
    return static_cast<QList<TileData*>*>(prop->data)->at(index);
}

// QT list with containing class TileData
QQmlListProperty<TileData> TetrisEngine::tiles()
{
    return QQmlListProperty<TileData>(this, &_tiles, &tilesPropAppend,
                                      &tilesPropCount, &tilesPropAt, 0);
}

// Constructor of class, sets Columns and Rows, initialize list
TetrisEngine::TetrisEngine ()
{
    _numCols = COLS;
    _numRows = ROWS ;
    _nextBrick = NULL;
    _curBrick = NULL;
    _linesCounter = 0;
    _endofgame = false;

    // Create seed for the random
    // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    for(int i = 0; i < _numRows * _numCols; ++i) {
        _tiles << new TileData;
    }

    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_function()));
}

TetrisEngine::~TetrisEngine()
{
    timer.stop();

    if (_nextBrick != NULL)
    {
        delete(_nextBrick);
        _nextBrick  = NULL;
    }

    if (_curBrick != NULL)
    {
        delete(_curBrick);
        _curBrick = NULL;
    }

    for(int i = 0; i < _tiles.count(); i++)
    {
        if (_tiles[i] != NULL)
        {
            delete(_tiles[i]);
            _tiles[i] = NULL;
        }
    }
}

// Restarting board, tiles, and bricks
void TetrisEngine::restart()
{
    _linesCounter = 0;
    emit linesCounterChanged();
    _endofgame = false;
    _rotatePos = 0;
    _curLevel = 0;
    emit nextLevel();

    // initialize random table with 1 (same chance for every brick)
    for (int i = 0; i < 7; i++)
    {
        _randomTable[i] = 1;
    }

    for(int i = 0; i < _tiles.count(); i++)
        _tiles[i]->restart();

    if (_nextBrick != NULL)
    {
        delete(_nextBrick);
        _nextBrick = NULL;
    }

    if (_curBrick != NULL)
        delete(_curBrick);

    _nextBrick = createNewBrick();
    _curBrick = createNewBrick();

    for (int i= 0; i < BRICKCOUNT; i++)
    {
        _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
    }

    _interval = START_INTERVAL;
    timer.setInterval(_interval);
    timer.start();
    qDebug() << "Undergone restart";
}

// connected to timer
void TetrisEngine::timer_function()
{
    moveDown();
}

// adding 1 to counter of lines, builded by user
void TetrisEngine:: addLine()
{
    _linesCounter++;
    // if current lines tens are higher than _curLevel, emit nextLevel
    // max level is 10
    if ( _linesCounter / 10 > _curLevel && _curLevel < 11)
    {
        _curLevel = _linesCounter / 10;
        _interval -= INTERVAL_STEP;
        timer.setInterval(_interval);
        emit nextLevel();
    }
    emit linesCounterChanged();
}

// Checking if there is full line, if yes then delete and move rest 1 line down
void TetrisEngine:: checkForLine()
{
    int counter = 0;

    for(int i = 0; i < _tiles.count(); i++)
    {
        if(_tiles[i]->highlighted())
        {
            counter++;
        }

        if(((i + 1) / 10) > (i / 10))
        {
            //there is line on i + 1
            if (counter == _numCols)
            {
                addLine();

                // unhighlight line for deletion
                for(int j = i; j > i- _numCols ; --j)
                {
                    _tiles[j]->unhighlight();
                }

                // move all tiles down except for new brick
                for(int j = i- _numCols; j > 0 ; --j)
                {
                    if (_tiles[j]->highlighted() && !_tiles[j]->active())
                    {
                        _tiles[j + _numCols]->highlight(new QColor(_tiles[j]->color()));
                        _tiles[j]->unhighlight();
                    }
                }
            }

            counter = 0;
        }
    }
}

// Checking collisions with other tiles, must provide number of tile
// returns true if collide
bool TetrisEngine:: checkCollision(int tileNum)
{
    // if tile is hightlighted and it isn't active (current tile) it's collision
    if ( tileNum > _tiles.count()
         || tileNum < 0
         || (_tiles[tileNum]->highlighted()
             && !_tiles[tileNum]->active() ) )
        return true;
    else
        return false;
}

// matches number of brick to colors
QColor* TetrisEngine:: resolveBrickColor(int brickNumber)
{
    QColor* color;

    switch (brickNumber)
    {
    case 1: // BrickI red
        color  = new QColor(255,72,56,200);
        break;
    case 2: // BrickJ magenta
        color = new QColor(212,73,255,200);
        break;
    case 3: // BrickL yellow
        color = new QColor(255,242,107,200);
        break;
    case 4: //BrickO cyan
        color = new QColor(127,255,238,200);
        break;
    case 5: // BrickS blue
        color = new QColor(107,119,255,200);
        break;
    case 6:  // BrickT orange
        color = new QColor(255,159,81,200);
        break;
    case 7: // BrickZ lime
        color = new QColor(192,255,33,200);
        break;
    default:
        color = new QColor(201,202,255,200);
        break;
    }

    return color;
}

// Method for moving brick to the left side
void TetrisEngine:: moveLeft()
{
    bool canContinue = true;

    // iterating through all tiles to check if brick isnt near left side of board
    for (int i= 0; i < BRICKCOUNT; i++)
    {
        // if tile, on the far left of brick, moved to left, goes to row above or is colliding raise flag
        if ( ((_curBrick->brickPosition[i] - 1) / 10 != _curBrick->brickPosition[i] / 10)
             || (checkCollision(_curBrick->brickPosition[i]- 1)) )
        {
            canContinue = false;
            break;
        }
    }

    if (canContinue)
    {
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            // going one row down by adding columns count
            _tiles[_curBrick->brickPosition[i]]->setUnactive();
            _curBrick->brickPosition[i]--;
        }

        // highlighting new tiles after changing all to avoid double highlight on one tile
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
        }
    }
}

// Method for moving brick to the right side
void TetrisEngine:: moveRight()
{
    bool canContinue = true;

    // iterating through all tiles to check if brick isnt near right side of board
    for (int i= 0; i < BRICKCOUNT; i++)
    {
        // if tile, on the far right of brick, moved to right, goes to row below, set off flag
        if ( ((_curBrick->brickPosition[i] + 1) / 10 != _curBrick->brickPosition[i] / 10)
             || (checkCollision(_curBrick->brickPosition[i] + 1)) )
        {
            canContinue = false;
            break;
        }
    }

    if (canContinue)
    {
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            // going one row down by adding columns count
            _tiles[_curBrick->brickPosition[i]]->setUnactive();
            _curBrick->brickPosition[i]++;
        }

        // highlighting new tiles after changing all to avoid double highlight on one tile
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
        }
    }
}

// Using current position set up new one, facing direction pointed by direction value:
// 0 - north, 1 - east, 2 - south, 3 - west
// after using delete received array by delete[], return NULL if error
int* TetrisEngine:: rotateBrick( int direction)
{
    int * brickNewPos = NULL;
    bool columnsUsed[_numCols]; // check if all values in array will be 0

    for (int i= 0; i < _numCols; i++)
    {
        columnsUsed[i] = false;
    }

    bool flag = true;
    // get columns number occupied by brick and set up flags on each
    for (int i= 0; i < BRICKCOUNT; i++)
    {
        columnsUsed[_curBrick->brickPosition[i] % _numCols] = true;
        if (_curBrick->brickPosition[i] / 10 > 18)
            flag = false;
    }
    if (flag)
    {
        // switch on color to know which brick is in use currently
        switch (_curBrick->color) // 3- BrickO
        {
        case 1: // BrickI
        {
            brickNewPos = new int[4];
            // only 2 directions
            // if moving from standing position to laying
            if (direction == 1 || direction == 3)
            {
                if (columnsUsed[0])
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + i + (2 - i) * _numCols;
                    }
                }
                else if (columnsUsed[1])
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + (-1 + i) + (2 - i) * _numCols;
                    }
                }
                else if (columnsUsed[9])
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + (-3 + i) + (2 - i) * _numCols;
                    }
                }
                else
                {
                    // do normal turn
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + (-2 + i) + (2 - i) * _numCols;
                    }
                }
            }
            else
            {
                // turn normaly, from position laying to standing
                // if brick just spawned, doesnt have place to rotate
                if (_curBrick->brickPosition[1] / 10 < 3)
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + (2 - i) + i * _numCols;
                    }
                }
                else
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] = _curBrick->brickPosition[i] + (2 - i) + (-2 + i) * _numCols;
                    }
                }
            }
            break;
        }
        case 2: // BrickJ
        {
            brickNewPos = new int[4];
            switch (direction)
            {
            case 0:
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1] ;
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 + _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2;

                // if just spawned move one row down
                if (_curBrick->brickPosition[1] / 10 < 1)
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] += _numCols;
                    }
                }
                break;

            case 1:
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 -  _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2 * _numCols;

                if (columnsUsed[9])
                { // shift 1 tile to left
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] -=1;
                    }
                }
                break;

            case 2:
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1] ;
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 - _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2;
                break;

            case 3:
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 +  _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2 * _numCols;

                if (columnsUsed[0])
                { // shift 1 to right
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] +=1;
                    }
                }

                break;
            }
            break;
        }
        case 3: // BrickL
        {
            brickNewPos = new int[4];
            switch (direction)
            {
            case 0:
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 -  _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2 * _numCols;

                // if just spawned move one row down
                if (_curBrick->brickPosition[1] / 10 < 1)
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] += _numCols;
                    }
                }
                break;

            case 1:
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1] ;
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 + _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2;

                if (columnsUsed[9])
                { // shift 1 tile to left
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] -=1;
                    }
                }
                break;

            case 2:
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 +  _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2 * _numCols;
                break;

            case 3:
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1] ;
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 - _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2;

                if (columnsUsed[0])
                { // shift 1 to right
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] +=1;
                    }
                }

                break;
            }
            break;
        }
        case 5: // BrickS
        {
            brickNewPos = new int[4];
            if (direction == 0 || direction == 2)
            {
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 - _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2;
                // if just spawned move one row down
                if (_curBrick->brickPosition[1] / 10 < 1)
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] += _numCols;
                    }
                }
            }
            else
            {
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 + _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2;

                if (columnsUsed[0])
                { // shift 1 to right
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] +=1;
                    }
                }
            }
            break;
        }
        case 6: // BrickT - 4 directions
        {
            brickNewPos = new int[4];

            switch (direction)
            {
            case 0: // just move 2nd tile
                brickNewPos[0] = _curBrick->brickPosition[0];
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 + _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3];
                break;

            case 1: // just move 0 tile
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2];
                brickNewPos[3] = _curBrick->brickPosition[3];

                if (columnsUsed[0])
                { // shift 1 to right
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] +=1;
                    }
                }
                break;

            case 2:// get to proper positions of tiles
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 - _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 1 + _numCols;
                break;

            case 3:// just move 3rd tile
                brickNewPos[0] = _curBrick->brickPosition[0];
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2];
                brickNewPos[3] = _curBrick->brickPosition[3] + 1 - _numCols;

                if (columnsUsed[9])
                { // shift 1 to left
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] -=1;
                    }
                }

                break;
            }

            break;
        }
        case 7: // BrickZ only 2 directions
        {
            brickNewPos = new int[4];
            if (direction == 0 || direction == 2)
            {
                brickNewPos[0] = _curBrick->brickPosition[0] + 1 - _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] - 1 - _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] - 2;

                // if just spawned move one row down
                if (_curBrick->brickPosition[1] / 10 < 1)
                {
                    for (int i = 0; i < BRICKCOUNT ; i++)
                    {
                        brickNewPos[i] += _numCols;
                    }
                }
            }
            else
            {
                brickNewPos[0] = _curBrick->brickPosition[0] - 1 + _numCols;
                brickNewPos[1] = _curBrick->brickPosition[1];
                brickNewPos[2] = _curBrick->brickPosition[2] + 1 + _numCols;
                brickNewPos[3] = _curBrick->brickPosition[3] + 2;

                if (columnsUsed[9])
                { // shift 1 to left
                    for (int i= 0; i < BRICKCOUNT; i++)
                    {
                        brickNewPos[i] -=1;
                    }
                }
            }
            break;
        }
        }
    }
    return brickNewPos;
}

// Method for rotating brick
void TetrisEngine:: rotate()
{
        qDebug() << "Rotating brick";
        bool canContinue = true;
        int* brickNextPos = rotateBrick(_rotatePos);

        if (brickNextPos != NULL)
        {

            // iterating through all tiles to check if brick after turn cause collision
            for (int i= 0; i < BRICKCOUNT; i++)
            {
                if ( checkCollision(brickNextPos[i]) )
                {
                    canContinue = false;
                    break;
                }
            }
        }
        else
            canContinue = false;

        if (canContinue)
        {
            for (int i= 0; i < BRICKCOUNT; i++)
            {
                // set old fields to unactive, set new ones
                _tiles[_curBrick->brickPosition[i]]->setUnactive();
                _curBrick->brickPosition[i] = brickNextPos[i];
            }

            // highlighting new tiles after changing all to avoid double highlight on one tile
            for (int i= 0; i < BRICKCOUNT; i++)
            {
                _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
            }

            // only when rotating had been done, rotatePos can be changed
            if (_rotatePos >= 3)
                _rotatePos = 0; // after pos west (3) there should be pos north (0)
            else
                _rotatePos++; // change Position of rotation
        }

        if (brickNextPos != NULL)
        {
            delete[](brickNextPos);
            brickNextPos = NULL;
        }
}

// Method for moving brick down,
// detects if new bricks should be spawned, also detects end of game
bool TetrisEngine:: moveDown()
{
    bool canContinue = true;

    for (int i= 0; i < BRICKCOUNT; i++)
    {
        // maximum allowed number for tile table is numRows * 10 - 1
        // if it exceeds brick should be stopped
        // or if its colliding with another brick
        if ((_curBrick->brickPosition[i] + _numCols > _numRows * 10 - 1)
                || (checkCollision(_curBrick->brickPosition[i] + _numCols)) )
        {
            canContinue = false;
            break;
        }
    }

    if (canContinue)
    {
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            // going one row down by adding columns count
            _tiles[_curBrick->brickPosition[i]]->setUnactive();
            _curBrick->brickPosition[i] =
                    _curBrick->brickPosition[i] + _numCols;
        }

        // highlighting new tiles after changing all to avoid double highlight on one tile
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
        }
    }
    else
    {
        qDebug() << "Collision detected, end of life of brick";

        if (spawnBrick())
        {
            setEndOfGame(true);
        }
        // after spawnBrick, so old brick tiles will be unactive
        checkForLine();
    }

    return canContinue;
}

// Method for teleporting brick to bottom
void TetrisEngine:: moveFastDown(bool value)
{
    qDebug() << "Moving fastDown";
    if (value)
    {
        timer.stop();
        timer.setInterval(30);
        timer.start();
    }
    else
    {
        timer.setInterval(_interval);
    }
}

// Randoming brick with balanced random
Brick* TetrisEngine:: createNewBrick()
{
    qDebug() << "Randoming and creating new brick";

    // Get random value between 0-6
    int sum = 0;
    Brick* brick;
    _rotatePos = 0; // reset direction

    for (int i = 0; i < 7; i++)
        sum += _randomTable[i]; // sum all chances

    int randomValue = rand() % sum; // random new value
    qDebug() << "Chosen value " << randomValue << " out of " << sum;

    sum = 0;
    // translate randomed value into 7 possibilites
    for (int i = 0; i < 7; i++)
    {
        sum += _randomTable[i]; // add chances for next brick
        // will trigger only if randomValue is between sum from previous state, and current
        if ( randomValue <= sum )
        {
            randomValue = i;
            break;
        }
    }

    qDebug() << "Chosen value " << randomValue;

    switch (randomValue)
    {
    case 0: // for every brick
        brick = new BrickI(_numCols, _numRows); // create new Brick
        break;
    case 1:
        brick = new BrickJ(_numCols, _numRows);
        break;
    case 2:
        brick = new BrickL(_numCols, _numRows);
        break;
    case 3:
        brick = new BrickO(_numCols, _numRows);
        break;
    case 4:
        brick = new BrickS(_numCols, _numRows);
        break;
    case 5:
        brick = new BrickT(_numCols, _numRows);
        break;
    case 6:
        brick = new BrickZ(_numCols, _numRows);
        break;
    default:
        brick = NULL; // if reach default error
        break;
    }

    //-------------- VALUE ADDED NEEDS ADJUSTMENT
    for (int i = 0; i < 7; i++)
        _randomTable[i] += 1; // add 1 for every other brick not chosen

    _randomTable[randomValue] = 1; // reset to 1 for BrickX

    return brick;
}

// Spawning new brick
// returns true if there was collision when spawning new brick
bool TetrisEngine:: spawnBrick()
{
    qDebug() << "Spawning new brick";
    bool collision = false;

    Brick* temp = _curBrick;

    _curBrick = _nextBrick;
    _nextBrick = createNewBrick();
    _rotatePos = 0;

    for (int i= 0; i < BRICKCOUNT; i++)
    {
        if (checkCollision(_curBrick->brickPosition[i]))
        {
            //if collision detected game is over
            collision = true;
        }
    }

    // if no collision detected, set tiles to active state
    if (!collision)
    {
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            _tiles[_curBrick->brickPosition[i]]->setActive(resolveBrickColor(_curBrick->color));
        }

        // reset timer because after speeding brick, interval is left with low number
        timer.stop();
        timer.setInterval(_interval);
        timer.start();

        // set old tile unactive
        for (int i= 0; i < BRICKCOUNT; i++)
        {
            _tiles[temp->brickPosition[i]]->setUnactive();
            _tiles[temp->brickPosition[i]]->highlight(resolveBrickColor(temp->color));
        }

    }

    if (temp != NULL)
        delete(temp);

    return collision;
}
