QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 遊戲代碼文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    DuckGame/DuckGame.cpp \
    Racecar/Racecar.cpp \
    Gameee/fishinggame.cpp

HEADERS += \
    mainwindow.h \
    DuckGame/DuckGame.h \
    Racecar/Racecar.h \
    Gameee/fishinggame.h

# 資源文件
RESOURCES += \
    CombinedGames.qrc \
    DuckGame/duckgame.qrc \
    Racecar/racecar.qrc \
    Gameee/gameee.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
