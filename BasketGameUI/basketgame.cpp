#include "basketgame.h"
#include "ui_basketgame.h"

Basketgame::Basketgame(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::basketgame), tempsPartie(new QTime),
    tempsManche(new QTime), timerPartie(new QTimer(this)),
    timerManche(new QTimer), etatPartie(false), etatManche(false),
    couleurEquipe(true)
{
    qDebug() << Q_FUNC_INFO;
    initialiserIHM();
    initialiserEvenements();
}

Basketgame::~Basketgame()
{
    delete ui;
    qDebug() << Q_FUNC_INFO;
}

void Basketgame::demarrerPartie()
{
    if(!etatPartie)
    {
        *tempsPartie = ui->editionTempsPartie->time();
        ui->tempsPartie->setText(tempsPartie->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsPartie"
                 << tempsPartie->toString("hh:mm:ss");
        /**
         * @fixme Si le temps est 00:00:00 ?
         */
        timerPartie->start(TIC_HORLOGE);
        etatPartie = true;
        ui->boutonDebutPartie->setEnabled(false);
        ui->boutonDebutManche->setEnabled(true);
    }
}

void Basketgame::demarrerManche()
{
    if(!etatManche)
    {
        *tempsManche = ui->editionTempsManche->time();
        ui->tempsManche->setText(tempsManche->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsManche"
                 << tempsManche->toString("hh:mm:ss");
        /**
         * @fixme Si le temps est 00:00:00 ?
         */
        timerManche->start(TIC_HORLOGE);
        etatManche = true;
        ui->boutonDebutManche->setEnabled(true);
    }
}

void Basketgame::arreterPartie()
{
    /**
     * @todo Gérer la fin d'une partie
     */
    timerManche->stop();
    timerPartie->stop();
    ui->UiEquipe->setStyleSheet("background-color: white; color: black;");
    ui->UiEquipe->setText("FIN DE LA PARTIE");
    ui->boutonDebutPartie->setEnabled(true);
    ui->boutonDebutManche->setEnabled(false);
    etatPartie = false;
}

void Basketgame::arreterManche()
{
    /**
     * @todo Gérer la fin d'une manche
     */
    etatManche = false;
}

void Basketgame::chronometrerPartie()
{
    // Voir aussi : QElapsedTimer
    QTime tempsEcoule = tempsPartie->addSecs(-1);
    tempsPartie->setHMS(tempsEcoule.hour(),
                        tempsEcoule.minute(),
                        tempsEcoule.second());
    qDebug() << Q_FUNC_INFO << "tempsPartie"
             << tempsPartie->toString("hh:mm:ss");
    ui->tempsPartie->setText(tempsPartie->toString("hh:mm:ss"));
    if(*tempsPartie == QTime(0, 0))
    {
        arreterPartie();
    }
}

void Basketgame::chronometrerManche()
{
    // Voir aussi : QElapsedTimer
    QTime tempsEcoule = tempsManche->addSecs(-1);
    tempsManche->setHMS(tempsEcoule.hour(),
                        tempsEcoule.minute(),
                        tempsEcoule.second());
    qDebug() << Q_FUNC_INFO << "tempsManche"
             << tempsManche->toString("hh:mm:ss");
    ui->tempsManche->setText(tempsManche->toString("hh:mm:ss"));
    if(*tempsManche == QTime(0, 0))
    {
        timerManche->stop();
        *tempsManche = ui->editionTempsManche->time();
        ui->tempsManche->setText(tempsManche->toString("hh:mm:ss"));
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
            ui->UiEquipe->setStyleSheet(
              "background-color: yellow; color: black;");
            ui->UiEquipe->setText("EQUIPE B");
        }
    }
}

void Basketgame::initialiserIHM()
{
    ui->setupUi(this);
    ui->boutonDebutPartie->setEnabled(true);
    ui->boutonDebutManche->setEnabled(false);
}

void Basketgame::initialiserEvenements()
{
    // les boutons
    connect(ui->boutonDebutManche,
            SIGNAL(clicked(bool)),
            this,
            SLOT(demarrerManche()));
    connect(ui->boutonDebutPartie,
            SIGNAL(clicked(bool)),
            this,
            SLOT(demarrerPartie()));

    // les minuteurs
    connect(timerPartie, SIGNAL(timeout()), this, SLOT(chronometrerPartie()));
    connect(timerManche, SIGNAL(timeout()), this, SLOT(chronometrerManche()));
}
