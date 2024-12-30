#ifndef RACECAR_H
#define RACECAR_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox> // 用於顯示消息框
#include <QList>

class Racecar : public QMainWindow
{
    Q_OBJECT

public:
    Racecar(QWidget *parent = nullptr);
    ~Racecar();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createObstacle();
    void checkCollision(); // 新增檢查碰撞方法
    void gameOver(); // 新增遊戲結束方法
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsRectItem *player;
    QTimer *obstacleTimer;
    QTimer *collisionTimer; // 用於定期檢查碰撞
    int laneWidth; // 每條路的寬度
    QList<int> lanes; // 用於存放路的位置
    int score; // 記錄分數
};

#endif // RACECAR_H
