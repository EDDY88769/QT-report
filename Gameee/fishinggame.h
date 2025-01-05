#ifndef FISHINGGAME_H
#define FISHINGGAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>

struct FishData {
    QString imagePath;       // 魚的圖片路徑
    int score;               // 捕捉這條魚的分數
    float speed;             // 魚的移動速度
    float spawnProbability;  // 魚的生成機率（範圍：0到1）
};

class FishingGame : public QGraphicsView {
    Q_OBJECT

public:
    explicit FishingGame(QWidget* parent = nullptr);
    ~FishingGame();

private slots:
    void startGame();
    void spawnFish();
    void updateGame();
    void updateScore(int points);
    void exitGame();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    void setupScene();
    void clearAllFish();

    QGraphicsScene* scene;
    QPushButton* startButton;
    QPushButton* endButton;
    QLabel* scoreLabel;
    QLabel* timerLabel;
    QTimer* gameTimer;
    QTimer* spawnTimer;

    int score;
    int timeLeft;
    bool gameStarted;
    QList<FishData> fishList;
};

#endif // FISHINGGAME_H
