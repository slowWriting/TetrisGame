# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = Tetris

CONFIG += sailfishapp

SOURCES += src/Tetris.cpp \
    tetrisengine.cpp

OTHER_FILES += qml/Tetris.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/Tetris.spec \
    rpm/Tetris.yaml \
    Tetris.desktop \
    qml/images/rotate.png \
    qml/images/rightarrow.png \
    qml/images/leftarrow.png \
    qml/images/downarrow.png \
    qml/pages/About.qml \
    qml/Tile.qml \
    qml/Tile.qml \
    qml/pages/tile/Tile.qml \
    qml/pages/Tile.qml \
    qml/pages/Tetris.qml

HEADERS += \
    tetrisengine.h \
    tiledata.h \
    bricks/Brick.h \
    bricks/BrickI.h \
    bricks/BrickO.h \
    bricks/BrickS.h \
    bricks/BrickZ.h \
    bricks/BrickT.h \
    bricks/BrickJ.h \
    bricks/BrickL.h \
    bricks/BricksAll.h

