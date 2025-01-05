#include "duckgame.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QSoundEffect>

DuckGame::DuckGame(QWidget *parent)
    : QMainWindow(parent), duckLabel(new QLabel(this)), scoreLabel(new QLabel(this)), lifeLabel(new QLabel(this)),
    duckTimer(new QTimer(this)), gameTimer(new QTimer(this)) {
    setupUI();

    connect(duckTimer, &QTimer::timeout, this, &DuckGame::updateDuckPosition);
    connect(gameTimer, &QTimer::timeout, [this]() {
        duckSpeed = qMax(duckSpeed - 50, 200); // 鴨子速度隨時間增加
        duckTimer->start(duckSpeed);
    });

    duckTimer->start(duckSpeed);
    gameTimer->start(3000); // 每5秒加速一次
}

DuckGame::~DuckGame() {}

void DuckGame::setupUI() {
    setFixedSize(1380, 720);

    // 背景設定
    setStyleSheet("background-color: lightblue; background-image: url(:/images/background.jpg); background-repeat: no-repeat; background-position: center;");
    setAutoFillBackground(true);


    // 鴨子
    duckLabel->setPixmap(QPixmap(":/images/duck.png").scaled(50, 50));
    duckLabel->setGeometry(0, 100, 50, 50);
    duckLabel->setStyleSheet("background: transparent;");

    // 分數和生命值顯示
    scoreLabel->setText("Score: 0");
    scoreLabel->setGeometry(10, 10, 100, 30);

    lifeLabel->setText("Life: 3");
    lifeLabel->setGeometry(10, 50, 100, 30);

    scoreLabel->setStyleSheet("background: transparent; color: black; font-size: 24px;");
    lifeLabel->setStyleSheet("background: transparent; color: red; font-size: 24px;");
}

void DuckGame::updateDuckPosition() {
    if (!duckHit) {
        life--; // 減少生命值
        lifeLabel->setText(QString("Life: %1").arg(life));
    }

    if (life <= 0) {
        checkGameOver();
    } else {
        // 隨機位置更新鴨子
        int x = QRandomGenerator::global()->bounded(width() - duckLabel->width());
        int y = QRandomGenerator::global()->bounded(height() - duckLabel->height());
        duckLabel->move(x, y);
    }

    duckHit = false; // 更新位置後重設擊中標誌
}


void DuckGame::mousePressEvent(QMouseEvent *event) {
    if (duckLabel->geometry().contains(event->pos())) {
        ++score;
        scoreLabel->setText(QString("Score: %1").arg(score));
        duckHit = true;  // 設置擊中標誌為true

        QTimer::singleShot(100, this, &DuckGame::updateDuckPosition);

        duckTimer->stop();
        duckTimer->start(duckSpeed);

        QSoundEffect *hitSound = new QSoundEffect(this);
        hitSound->setSource(QUrl("qrc:/sounds/hit.wav"));
        hitSound->play();
    }
}

void DuckGame::checkGameOver() {
    duckTimer->stop();
    gameTimer->stop();
    QMessageBox::information(this, "Game Over", QString("Final Score: %1").arg(score));
    close();
}
