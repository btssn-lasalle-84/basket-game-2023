#ifndef BASKETGAME_H
#define BASKETGAME_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

namespace Ui {
class basketgame;
}

class basketgame : public QMainWindow
{
    Q_OBJECT

public:
    explicit basketgame(QWidget *parent = 0);
    ~basketgame();

    QTime *afficherTimerPartie = new QTime;
    QTime *afficherTimerManche = new QTime;
    QTimer *timerPartie = new QTimer;
    QTimer *timerManche = new QTimer;
    bool boutonCommencerPartie = true;
    bool boutonCommencerManche = true;
    bool couleurEquipe = true;

public slots:
    void ticTempsPartie();
    void ticTempsManche();

private slots:
    void on_boutonDebutManche_clicked();
    void on_boutonDebutPartie_clicked();

private:
    Ui::basketgame *ui;
};

#endif // BASKETGAME_H
