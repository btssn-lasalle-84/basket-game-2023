#ifndef BASKETGAME_H
#define BASKETGAME_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QWidget>
#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui { class BasketGame; }
QT_END_NAMESPACE

class BasketGame : public QMainWindow
{
    Q_OBJECT

public:
    BasketGame(QWidget *parent = nullptr);
    ~BasketGame();
    void paintEvent(QPaintEvent *event);
private:
    Ui::BasketGame *ui;
};
#endif // BASKETGAME_H
