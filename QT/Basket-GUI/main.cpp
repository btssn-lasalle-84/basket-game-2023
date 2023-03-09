#include "basketgame.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BasketGame w;
    w.showFullScreen();
    return a.exec();
}
