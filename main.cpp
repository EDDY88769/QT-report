#include "mainwindow.h"

#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "DuckGame/DuckGame.h"
#include "Racecar/Racecar.h"
#include "Gameee/fishinggame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::srand(std::time(nullptr));  // 初始化隨機數生成器

    int gameIndex = std::rand() % 3;  // 隨機選擇遊戲（0, 1 或 2）

    QWidget *gameWindow = nullptr;

    if (gameIndex == 0) {
        gameWindow = new DuckGame();
    } else if (gameIndex == 1) {
        gameWindow = new Racecar();
    } else {
        gameWindow = new FishingGame();
    }

    gameWindow->show();
    return app.exec();
}
