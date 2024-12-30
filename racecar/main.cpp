#include "racecar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Racecar w;
    w.show();
    return a.exec();
}
