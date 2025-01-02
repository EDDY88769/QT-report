#ifndef RACECAR_H
#define RACECAR_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDial>
#include <QLabel>

class Racecar : public QMainWindow
{
    Q_OBJECT

public:
    Racecar(QWidget *parent = nullptr);
    ~Racecar();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void createObstacle();
    void checkCollision();
    void gameOver();
    void moveBackground();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *player;
    QGraphicsPixmapItem *background1;
    QGraphicsPixmapItem *background2;
    QList<int> lanes;
    int laneWidth;
    int score;
    QTimer *obstacleTimer;
    QTimer *collisionTimer;
    QTimer *backgroundTimer;
    int obstacleSpeed;
    int obstacleInterval;
    QDial *speedometer;
    QLabel *speedLabel;
};

#endif // RACECAR_H
