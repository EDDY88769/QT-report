#include "FishingGame.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    FishingGame game;
    game.showFullScreen(); // 根據需求設為全螢幕
    return app.exec();
}
