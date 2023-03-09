#include "basketgame.h"
#include "ui_basketgame.h"

BasketGame::BasketGame(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::BasketGame)
{
    ui->setupUi(this);
}

BasketGame::~BasketGame()
{
    delete ui;
}

void BasketGame::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QBrush   background(QColor(23, 23, 34));
    painter.setBrush(background);
    painter.setPen(Qt::NoPen); // No stroke
    painter.drawRect(0, 0, width(), height());
}
