#include "racecar.h"
#include <QGraphicsRectItem>
#include <QTimer>
#include <QtGlobal>
#include <cstdlib>
#include <QDebug>
#include <QMessageBox>
#include <algorithm>
#include <random> // 用於 std::shuffle
#include <QApplication>

Racecar::Racecar(QWidget *parent)
    : QMainWindow(parent), laneWidth(800 / 3), score(0)
{
    // 初始化視圖和場景
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // 設定主窗口大小
    setCentralWidget(view);
    resize(800, 600);

    // 設定場景大小
    scene->setSceneRect(0, 0, 800, 600);

    // 初始化路的位置
    lanes << laneWidth / 2 << 3 * laneWidth / 2 << 5 * laneWidth / 2;

    // 添加玩家車輛
    player = new QGraphicsRectItem(0, 0, 50, 100);
    player->setBrush(Qt::cyan);
    player->setPos(lanes[1] - 25, 500); // 初始位置在中間路
    scene->addItem(player);

    // 設定背景顏色
    scene->setBackgroundBrush(Qt::black);

    // 設定障礙物生成
    obstacleTimer = new QTimer(this);
    connect(obstacleTimer, &QTimer::timeout, this, &Racecar::createObstacle);
    obstacleTimer->start(1500); // 每1.5秒生成一個障礙物

    // 設定碰撞檢查計時器
    collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &Racecar::checkCollision);
    collisionTimer->start(50); // 每50毫秒檢查一次碰撞

    // 設定分數增加計時器
    QTimer *scoreTimer = new QTimer(this);
    connect(scoreTimer, &QTimer::timeout, [=]() {
        score++; // 每1毫秒增加一次分數
        qDebug() << "Score:" << score;
    });
    scoreTimer->start(1); // 每1毫秒計算一次分數
}

Racecar::~Racecar()
{
    // 銷毀計時器，避免內存洩漏
    if (obstacleTimer) {
        obstacleTimer->stop();
        delete obstacleTimer;
    }
    if (collisionTimer) {
        collisionTimer->stop();
        delete collisionTimer;
    }
}

void Racecar::createObstacle()
{
    // 隨機選擇兩條路來生成障礙物
    QList<int> lanesToUse = {0, 1, 2};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lanesToUse.begin(), lanesToUse.end(), g);
    lanesToUse.pop_back(); // 移除一個元素，確保只在兩條路上生成障礙物

    for (int i : lanesToUse) {
        QGraphicsRectItem *obstacle = new QGraphicsRectItem(0, 0, 50, 100);
        obstacle->setBrush(Qt::red);
        obstacle->setPos(lanes[i] - 25, 0);
        scene->addItem(obstacle);

        // 設置障礙物移動
        QTimer *moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, [obstacle, moveTimer, this]() {
            obstacle->moveBy(0, 10);
            if (obstacle->y() > 600) {
                scene->removeItem(obstacle);
                delete obstacle; // 使用 deleteLater 確保安全刪除
                moveTimer->deleteLater(); // 確保計時器也被安全刪除
                qDebug() << "Obstacle removed, score increment.";
            }
        });
        moveTimer->start(50);
    }
}

void Racecar::checkCollision()
{
    QList<QGraphicsItem *> collidingItems = player->collidingItems();
    foreach (QGraphicsItem *item, collidingItems) {
        if (dynamic_cast<QGraphicsRectItem *>(item)) {
            gameOver();
            return;
        }
    }
}

void Racecar::gameOver()
{
    obstacleTimer->stop();
    collisionTimer->stop();
    QMessageBox::information(this, "Game Over", QString("Game Over!\nYour Score: %1").arg(score));
    qApp->exit();
}

void Racecar::keyPressEvent(QKeyEvent *event)
{
    // 確保玩家車輛只能在三條路之間移動
    if (event->key() == Qt::Key_A && player->x() > laneWidth / 2) {
        player->moveBy(-laneWidth, 0);
    } else if (event->key() == Qt::Key_D && player->x() < (5 * laneWidth / 2) - laneWidth) {
        player->moveBy(laneWidth, 0);
    }
    qDebug() << "Player position:" << player->pos(); // 調試鍵盤事件
}
