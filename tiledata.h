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

#ifndef TILEDATA_H
#define TILEDATA_H
#define DEFAULTCOLOR 220,220,220,180
#include <QtQml>
#include <QColor>

class TileData : public QObject {
     Q_OBJECT
public:
    TileData() : _color(new QColor(DEFAULTCOLOR)), _active(false), _highlighted(false) {}
    ~TileData(){ if(_color != NULL) delete(_color); _color = NULL;}
    void restart() {setColor(new QColor(DEFAULTCOLOR)); _active = false; _highlighted = false;}

    Q_PROPERTY(bool highlighted READ highlighted NOTIFY highlightedChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

    QColor color() const {
        return (*_color);
    }

    bool highlighted() const { return _highlighted; }
    bool active() const { return _active;}

    void highlight(QColor* color) { if (_highlighted) return; setHighlight(true); setColor(color); }
    void unhighlight() { if(!_highlighted) return; setHighlight(false); setColor(new QColor(DEFAULTCOLOR));}

    void setActive(QColor* color) { if (_active) return; setActive(true); highlight(color); }
    void setUnactive() { if(!_active) return; setActive(false); unhighlight(); }

signals:
    void highlightedChanged();
    void activeChanged();
    void  colorChanged();

private:
    void setColor(QColor* color) { delete(_color); _color = color; emit colorChanged(); }
    void setHighlight(bool value) { _highlighted = value; emit highlightedChanged(); }
    void setActive(bool value) { _active = value; emit activeChanged(); }
     //QColor* _defColor; // default color of tile
     QColor* _color;
     bool _active;
     bool _highlighted;     
 };
#endif // TILEDATA_H
