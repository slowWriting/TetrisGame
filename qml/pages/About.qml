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
    id: page
    SilicaListView {
        id: listView
        model: 1
        anchors.fill: parent
        header: PageHeader {
            title: "About Authors"
        }
        delegate: BackgroundItem {
            id: delegate

            Column{
                anchors.margins: 20
                anchors.left : parent.left
                Row {
                    Label {
                        x: Theme.paddingLarge
                        text: "Tetris©"
                        color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                }

                Row {
                    Text{
                        text:"\nSilesian University of Technology\nRegulski Maciej\nDynus Wojciech\nPęśkiewicz Bartosz\n" +
                             "Magiera Krzysztof\nLubos Grzegorz\nBeczek Mateusz\nŻurawski Mariusz\nRaszka Andrzej\n" +
                             "Ślusarzyk Filip\nPaśdzior Michał\nKordys Michał"
                        color: Theme.primaryColor
                    }
                }
            }

            onClicked: console.log("Clicked " + index)
        }
    }
}
