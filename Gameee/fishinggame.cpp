#include "FishingGame.h"
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

FishingGame::FishingGame(QWidget* parent)
    : QGraphicsView(parent), score(0), timeLeft(60), gameStarted(false) {
    setupScene();
}

FishingGame::~FishingGame() {
    // 清理所有已經創建的物件
    delete scene;
    delete gameTimer;
    delete spawnTimer;
    delete scoreLabel;
    delete timerLabel;
    delete startButton;
    delete endButton;
}

void FishingGame::setupScene() {
    QPixmap backgroundPixmap(":/images/lake_background.jpg");
    if (backgroundPixmap.isNull()) {
        qDebug() << "Error: Failed to load background image.";
        return;
    }

    int width = backgroundPixmap.width();
    int height = backgroundPixmap.height();

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    setScene(scene);

    setFixedSize(width, height);
    setBackgroundBrush(QBrush(backgroundPixmap));

    startButton = new QPushButton("開始遊戲", this);
    startButton->setGeometry(width / 2 - 100, height / 2 - 25, 200, 50);
    startButton->setStyleSheet("font-size: 20px;");
    connect(startButton, &QPushButton::clicked, this, &FishingGame::startGame);

    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setGeometry(10, 10, 150, 50);
    scoreLabel->setStyleSheet("font-size: 24px; color: white; font-weight: bold;");

    timerLabel = new QLabel("Time: 60", this);
    timerLabel->setGeometry(width - 160, 10, 150, 50);
    timerLabel->setStyleSheet("font-size: 24px; color: white; font-weight: bold;");

    fishList = {
        {":/images/fish1.png", 30, 4.5f, 0.7f},
        {":/images/fish2.png", 80, 6.0f, 0.5f},
        {":/images/fish3.png", 100, 2.5f, 0.15f},
        {":/images/fish4.png", 250, 10.5f, 0.1f},
        {":/images/fish5.png", 30, 4.9f, 0.7f},
        {":/images/fish6.png", 500, 15.0f, 0.05f},
        {":/images/fish7.png", 90, 5.6f, 0.4f},
        {":/images/fish8.png", 250, 8.9f, 0.1f},
        {":/images/fish9.png", 1000, 20.0f, 0.05f},
        {":/images/fish10.png", 2500, 25.0f, 0.01f},
        {":/images/fish11.png", 1750, 12.5f, 0.025f},
    };
}

void FishingGame::startGame() {
    if (gameStarted) return;

    gameStarted = true;
    startButton->hide();
    scoreLabel->show();
    timerLabel->show();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &FishingGame::updateGame);
    gameTimer->start(1000);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &FishingGame::spawnFish);
    spawnTimer->start(100);

    spawnFish();
}

void FishingGame::spawnFish() {
    const FishData& fish = fishList[QRandomGenerator::global()->bounded(fishList.size())];

    if (QRandomGenerator::global()->bounded(1.0) > fish.spawnProbability) {
        return;
    }

    QGraphicsPixmapItem* fishItem = new QGraphicsPixmapItem(QPixmap(fish.imagePath));
    if (fishItem->pixmap().isNull()) {
        qDebug() << "Error: Failed to load fish image:" << fish.imagePath;
        delete fishItem;
        return;
    }

    int yPos = QRandomGenerator::global()->bounded(scene->height() - fishItem->pixmap().height());
    fishItem->setPos(scene->width(), yPos);

    scene->addItem(fishItem);

    QTimer* moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, [=]() {
        fishItem->setX(fishItem->x() - fish.speed);

        if (fishItem->x() < 0) {
            scene->removeItem(fishItem);
            delete fishItem;
            moveTimer->stop();
            moveTimer->deleteLater();
        }
    });

    moveTimer->start(15);
    fishItem->setData(0, QVariant::fromValue(moveTimer));
}

void FishingGame::updateGame() {
    timeLeft--;
    timerLabel->setText(QString("Time: %1").arg(timeLeft));

    if (timeLeft <= 0) {
        gameTimer->stop();
        spawnTimer->stop();
        spawnFish();
        QMessageBox::information(this, "遊戲結束", QString("遊戲結束！你的分數是：%1").arg(score));

        endButton = new QPushButton("退出遊戲", this);
        endButton->setGeometry(scene->width() / 2 - 100, scene->height() / 2 + 30, 200, 50);
        endButton->setStyleSheet("font-size: 20px;");
        connect(endButton, &QPushButton::clicked, this, &FishingGame::exitGame);

        score = 0;
        timeLeft = 60;
        scoreLabel->setText("Score: 0");
        timerLabel->setText("Time: 60");

        clearAllFish();
        gameStarted = false;
    }
}

void FishingGame::clearAllFish() {
    QList<QGraphicsItem*> allItems = scene->items();
    for (QGraphicsItem* item : allItems) {
        if (QGraphicsPixmapItem* fishItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
            QTimer* moveTimer = fishItem->data(0).value<QTimer*>();
            if (moveTimer) {
                moveTimer->stop();
                moveTimer->deleteLater();
            }
            scene->removeItem(fishItem);
            delete fishItem;
        }
    }
}

void FishingGame::updateScore(int points) {
    score += points;
    scoreLabel->setText(QString("Score: %1").arg(score));
}

void FishingGame::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QList<QGraphicsItem*> clickedItems = scene->items(mapToScene(event->pos()));
        for (QGraphicsItem* item : clickedItems) {
            if (QGraphicsPixmapItem* fishItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
                for (const FishData& fish : fishList) {
                    if (fishItem->pixmap().cacheKey() == QPixmap(fish.imagePath).cacheKey()) {
                        updateScore(fish.score);
                        break;
                    }
                }

                QTimer* moveTimer = fishItem->data(0).value<QTimer*>();
                if (moveTimer) {
                    moveTimer->stop();
                    moveTimer->deleteLater();
                }

                scene->removeItem(fishItem);
                delete fishItem;
                return;
            }
        }
    }
}

void FishingGame::exitGame() {
    close();
}
