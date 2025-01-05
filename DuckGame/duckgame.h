#ifndef DUCKGAME_H
#define DUCKGAME_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>

class DuckGame : public QMainWindow
{
    Q_OBJECT

public:
    DuckGame(QWidget *parent = nullptr);
    ~DuckGame();

private:
    QLabel *duckLabel;
    QLabel *scoreLabel;
    QLabel *lifeLabel;
    QTimer *duckTimer;
    QTimer *gameTimer;

    int score = 0;
    int life = 3;
    int duckSpeed = 2000; // 初始鴨子移動間隔（毫秒）
    bool duckHit = false;

    void setupUI();
    void updateDuckPosition();
    void checkGameOver();
    void animateDuck();

protected:
    void mousePressEvent(QMouseEvent *event) override;

};
#endif // DUCKGAME_H
