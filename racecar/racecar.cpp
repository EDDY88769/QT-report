#include "racecar.h"
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <random>

Racecar::Racecar(QWidget *parent)
    : QMainWindow(parent), laneWidth(1600 / 3), score(0), obstacleSpeed(10), obstacleInterval(1750) // 设置窗口宽度，并缩短障碍物生成间隔时间
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    QPixmap bgPixmap(":/images/background.jpg");
    background1 = new QGraphicsPixmapItem(bgPixmap);
    background2 = new QGraphicsPixmapItem(bgPixmap);
    background1->setPos(0, 0);
    background2->setPos(0, -bgPixmap.height());

    // 将背景图片设置为底层
    background1->setZValue(-1);
    background2->setZValue(-1);

    scene->addItem(background1);
    scene->addItem(background2);

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *sideLayout = new QVBoxLayout();

    speedometer = new QDial(this);
    speedometer->setRange(60, 400);
    speedometer->setValue(60);

    speedLabel = new QLabel(QString::number(60), this);
    speedLabel->setAlignment(Qt::AlignCenter);

    sideLayout->addWidget(speedometer);
    sideLayout->addWidget(speedLabel);

    mainLayout->addLayout(sideLayout);
    mainLayout->addWidget(view);

    setCentralWidget(centralWidget);
    resize(1600, 800); // 调整窗口大小为1600宽度
    scene->setSceneRect(0, 0, 1600, 800);

    lanes << laneWidth / 2 << 3 * laneWidth / 2 << 5 * laneWidth / 2;

    QPixmap carPixmap(":/images/car.png");
    QPixmap scaledCarPixmap = carPixmap.scaled(100, 100, Qt::KeepAspectRatio);
    player = new QGraphicsPixmapItem(scaledCarPixmap);
    player->setPos(lanes[1] - player->boundingRect().width() / 2, scene->height() - scaledCarPixmap.height() - 50); // 汽车初始位置改为屏幕底部
    scene->addItem(player);

    obstacleTimer = new QTimer(this);
    connect(obstacleTimer, &QTimer::timeout, this, &Racecar::createObstacle);
    obstacleTimer->start(obstacleInterval); // 调整障碍物生成间隔时间

    collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &Racecar::checkCollision);
    collisionTimer->start(50);

    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, &QTimer::timeout, this, &Racecar::moveBackground);
    backgroundTimer->start(50);

    QTimer *scoreTimer = new QTimer(this);
    connect(scoreTimer, &QTimer::timeout, [=]() {
        score++;
        qDebug() << "Score:" << score;

        if (score % 10 == 0) {
            obstacleSpeed += 2;
            obstacleInterval = qMax(500, obstacleInterval - 100);
            obstacleTimer->start(obstacleInterval);
            qDebug() << "Obstacle speed increased to:" << obstacleSpeed;
            qDebug() << "Obstacle interval decreased to:" << obstacleInterval;

            int displaySpeed = qMin(400, 60 + obstacleSpeed * 10);
            speedometer->setValue(displaySpeed);
            speedLabel->setText(QString::number(displaySpeed));
        }
    });
    scoreTimer->start(1000);
}

Racecar::~Racecar()
{
    if (obstacleTimer) {
        obstacleTimer->stop();
        delete obstacleTimer;
    }
    if (collisionTimer) {
        collisionTimer->stop();
        delete collisionTimer;
    }
    if (backgroundTimer) {
        backgroundTimer->stop();
        delete backgroundTimer;
    }
}

void Racecar::createObstacle()
{
    QList<int> lanesToUse = {0, 1, 2};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lanesToUse.begin(), lanesToUse.end(), g);
    lanesToUse.pop_back();

    for (int i : lanesToUse) {
        QPixmap obstaclePixmap(":/images/triangle.png");
        QPixmap scaledObstaclePixmap = obstaclePixmap.scaled(100, 75, Qt::KeepAspectRatio);
        QGraphicsPixmapItem *obstacle = new QGraphicsPixmapItem(scaledObstaclePixmap);
        obstacle->setPos(lanes[i] - obstacle->boundingRect().width() / 2, 0); // 障碍物从顶部生成
        scene->addItem(obstacle);

        QTimer *moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, [obstacle, moveTimer, this]() {
            obstacle->moveBy(0, obstacleSpeed);
            if (obstacle->y() > scene->height()) { // 超出屏幕底部时删除障碍物
                scene->removeItem(obstacle);
                delete obstacle;
                moveTimer->deleteLater();
            }
        });
        moveTimer->start(50);
    }
}

void Racecar::checkCollision()
{
    QList<QGraphicsItem *> collidingItems = player->collidingItems();
    foreach (QGraphicsItem *item, collidingItems) {
        if (dynamic_cast<QGraphicsPixmapItem *>(item) && item != player && item != background1 && item != background2) {
            gameOver();
            return;
        }
    }
}

void Racecar::gameOver()
{
    obstacleTimer->stop();
    collisionTimer->stop();
    backgroundTimer->stop();
    QMessageBox::information(this, "Game Over", QString("Game Over!\nYour Score: %1").arg(score));
    qApp->exit();
}

void Racecar::moveBackground()
{
    int moveSpeed = 2;
    background1->moveBy(0, moveSpeed);
    background2->moveBy(0, moveSpeed);

    if (background1->y() >= scene->height()) {
        background1->setPos(0, -background1->boundingRect().height());
    }
    if (background2->y() >= scene->height()) {
        background2->setPos(0, -background2->boundingRect().height());
    }
}

void Racecar::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A && player->x() > laneWidth / 2) {
        player->moveBy(-laneWidth, 0);
    } else if (event->key() == Qt::Key_D && player->x() < (5 * laneWidth / 2) - laneWidth) {
        player->moveBy(laneWidth, 0);
    }
    qDebug() << "Player position:" << player->pos();
}
