#include "basketgame.h"
#include "ui_basketgame.h"

basketgame::basketgame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::basketgame)
{
    ui->setupUi(this);
    connect(timerPartie, SIGNAL(timeout()), this, SLOT(ticTempsPartie()));
    connect(timerManche, SIGNAL(timeout()), this, SLOT(ticTempsManche()));
}

basketgame::~basketgame()
{
    delete ui;
}

void basketgame::on_boutonDebutPartie_clicked()
{
    if(boutonCommencerPartie == true)
    {
        *afficherTimerPartie = ui->editerTempsPartie->time();
        ui->UiTempsPartie->setText(afficherTimerPartie->toString("hh:mm:ss"));
        timerPartie->start(1000);
    }
}

void basketgame::ticTempsPartie()
{
    QTime newTime = afficherTimerPartie->addSecs(-1);
    afficherTimerPartie->setHMS(newTime.hour(),newTime.minute(),newTime.second());
    ui->UiTempsPartie->setText(afficherTimerPartie->toString("hh:mm:ss"));
    if(afficherTimerPartie->hour() != 0 ||
            afficherTimerPartie->minute() != 0 ||
            afficherTimerPartie->second() != 0)
    {
        timerPartie->start(1000);
    }
    else
    {
        timerPartie->stop();
        timerManche->stop();
        ui->UiEquipe->setStyleSheet("background-color: white; color: black;");
        ui->UiEquipe->setText("FIN DE LA PARTIE");

    }
}

void basketgame::on_boutonDebutManche_clicked()
{
    if(boutonCommencerManche == true)
    {
        *afficherTimerManche = ui->editerTempsManche->time();
        ui->UiTempsManche->setText(afficherTimerManche->toString("hh:mm:ss"));
        timerManche->start(1000);
    }
}

void basketgame::ticTempsManche()
{
    QTime temps2 = afficherTimerManche->addSecs(-1);
    afficherTimerManche->setHMS(temps2.hour(),temps2.minute(),temps2.second());
    ui->UiTempsManche->setText(afficherTimerManche->toString("hh:mm:ss"));
    if(afficherTimerManche->hour() != 0 ||
            afficherTimerManche->minute() != 0 ||
            afficherTimerManche->second() != 0)
    {
        timerManche->start(1000);
    }
    else
    {
        timerManche->stop();

        *afficherTimerManche = ui->editerTempsManche->time();
        ui->UiTempsManche->setText(afficherTimerManche->toString("hh:mm:ss"));
        timerManche->start(1000);

        if(couleurEquipe == true)
        {
        couleurEquipe = false;
        ui->UiEquipe->setStyleSheet("background-color: red; color: black;");
        ui->UiEquipe->setText("EQUIPE A");
        }
        else
        {
            couleurEquipe = true;
            ui->UiEquipe->setStyleSheet("background-color: yellow; color: black;");
            ui->UiEquipe->setText("EQUIPE B");

        }
    }
}
