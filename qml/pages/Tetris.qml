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

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id:tetris

    anchors.fill: parent
    allowedOrientations : Orientation.Portrait

    // bricks starting to fall down, starting timer
    Component.onCompleted: {
        engine.restart()
    }

    Connections {
             target: engine
             onEndOfGameChanged: {
                 console.log("End of Game!")
                 pageStack.replace(Qt.resolvedUrl("About.qml"))
             }
             onNextLevel: {
                 console.log("Next Level!")
             }
         }


    Column {
        id: column

        width: tetris.width
        anchors.bottom: tetris.bottom
        anchors.bottomMargin: Theme.paddingLarge*2
        spacing: Theme.paddingLarge

        Text{
            id:lineCounter
            anchors.right: parent.right;
            anchors.margins: 10
            text: "Level:" + engine.curLevel + "  Lines: " + engine.linesCounter
            color: Theme.primaryColor
        }


        Grid {
                id: field;
                anchors.horizontalCenter: parent.horizontalCenter
                columns: 10; spacing: 1

                   Repeater {
                       id: repeater
                       model: engine.tiles // count of tiles
                       delegate: Tile {}
                   }
                }

        Row {
            spacing: Theme.paddingMedium * 4

            IconButton {
                id: leftArrowButton
                icon.source: "../images/leftarrow.png"
                onClicked: {
                    engine.moveLeft()
                }
                onPressAndHold: {
                    while(leftArrowButton.down)
                    {
                        engine.moveLeft()
                    }
                }
            }

            IconButton {
                id: downArrowButton
                icon.source: "../images/downarrow.png"
                onClicked: {
                    engine.moveFastDown(true)
                }
                onPressAndHold: {
                    engine.moveFastDown(true)
                }
                onReleased:
                    engine.moveFastDown(false)
            }

            IconButton {
                id: rotateButton
                icon.source: "../images/rotate.png"
                onClicked: {
                    engine.rotate()
                }
            }
            IconButton {
                id: rightArrowButton
                icon.source: "../images/rightarrow.png"
                onClicked: {
                    engine.moveRight()
                }
                onPressAndHold: {
                    engine.moveRight()
                }
            }
        }
    }
}
