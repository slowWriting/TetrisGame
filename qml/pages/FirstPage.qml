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
    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: "Help"
                onClicked: pageStack.push(Qt.resolvedUrl("Help.qml"))
            }
            MenuItem {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: parent.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.

        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 2*Theme.paddingLarge
            spacing: 20

            PageHeader {
                title: "Tetris 1.0"

            }

            Row {
                spacing: Theme.paddingLarge
                IconButton {
                    icon.source: "image://theme/icon-l-play"
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("Tetris.qml"))
                    }
                }

                Text {
                    text:"Play"
                    color: Theme.primaryColor
                }

//                IconButton {
//                    icon.source: "image://theme/icon-cover-pause"
//                    onClicked: {
//                        pageStack.push(Qt.resolvedUrl("About.qml"))
//                    }
//                }
            }
        }
    }
}
