/**
 * @file basketgame.cpp
 *
 * @brief Définition de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.1
 */

#include "basketgame.h"
#include "ui_basketgame.h"

/**
 * @brief Constructeur de la classe Basketgame
 *
 * @fn Basketgame::Basketgame
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */
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

/**
 * @brief Destructeur de la classe Basketgame
 *
 * @fn Basketgame::~Basketgame
 * @details Libère les ressources de l'application
 */
Basketgame::~Basketgame()
{
    delete ui;
    qDebug() << Q_FUNC_INFO;
}

/**
 * @fn Basketgame::afficherEcran(Basketgame::Ecran ecran)
 * @brief Selectionne la fenêtre et l'affiche
 */
void Basketgame::afficherEcran(Basketgame::Ecran ecran)
{
    ui->ecrans->setCurrentIndex(ecran);
}

void Basketgame::afficherEcranAcceuil()
{
    afficherEcran(Basketgame::Ecran::Accueil);
}

void Basketgame::afficherEcranPartie()
{
    ui->boutonDebutPartie->setEnabled(true);
    ui->editionTempsPartie->setEnabled(true);
    ui->boutonDebutManche->setEnabled(false);
    ui->editionTempsManche->setEnabled(false);
    afficherEcran(Basketgame::Ecran::Partie);
}

void Basketgame::afficherEcranManche()
{
    afficherEcran(Basketgame::Ecran::Manche);
}

void Basketgame::demarrerPartie()
{
    if(!etatPartie)
    {
        *tempsPartie = ui->editionTempsPartie->time();
        ui->boutonDebutPartie->setEnabled(false);
        ui->editionTempsPartie->setEnabled(false);
        ui->tempsPartie->setText(tempsPartie->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsPartie"
                 << tempsPartie->toString("hh:mm:ss");
        /**
         * @fixme Si le temps est 00:00:00 ?
         */
        timerPartie->start(TIC_HORLOGE);
        etatPartie = true;
        ui->boutonDebutManche->setEnabled(true);
        ui->editionTempsManche->setEnabled(true);
    }
}

void Basketgame::demarrerManche()
{
    if(!etatManche)
    {
        *tempsManche = ui->editionTempsManche->time();
        ui->editionTempsManche->setEnabled(false);
        ui->tempsManche->setText(tempsManche->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsManche"
                 << tempsManche->toString("hh:mm:ss");
        /**
         * @fixme Si le temps est 00:00:00 ?
         */
        timerManche->start(TIC_HORLOGE);
        etatManche = true;
        ui->boutonDebutManche->setEnabled(false);
        ui->editionTempsManche->setEnabled(false);
    }
}

void Basketgame::arreterPartie()
{
    /**
     * @todo Gérer la fin d'une partie
     */
    timerManche->stop();
    arreterManche();

    timerPartie->stop();
    ui->UiEquipe->setStyleSheet("background-color: white; color: black;");
    ui->UiEquipe->setText("FIN DE LA PARTIE");
    ui->boutonDebutPartie->setEnabled(true);
    ui->editionTempsPartie->setEnabled(true);
    etatPartie = false;
}

void Basketgame::arreterManche()
{
    /**
     * @todo Gérer la fin d'une manche
     */
    etatManche = false;
    ui->tempsManche->setText("00:00:00");
    ui->boutonDebutManche->setEnabled(false);
    ui->editionTempsManche->setEnabled(false);
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
    /**
     * @fixme Vous confondez manche et tour !
     */
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
    ui->boutonDemarrer->setEnabled(true);

#ifdef PLEIN_ECRAN_RPI
    showFullScreen();
#else
    resize(qApp->desktop()->availableGeometry(this).width(),
           qApp->desktop()->availableGeometry(this).height());
    setMinimumSize(qApp->desktop()->availableGeometry(this).width(),
                   qApp->desktop()->availableGeometry(this).height());
    // showMaximized();
#endif
    afficherEcranAcceuil();
}

void Basketgame::initialiserEvenements()
{
    // les boutons
    connect(ui->boutonDemarrer,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherEcranPartie()));
    connect(ui->boutonDebutPartie,
            SIGNAL(clicked(bool)),
            this,
            SLOT(demarrerPartie()));
    connect(ui->boutonDebutManche,
            SIGNAL(clicked(bool)),
            this,
            SLOT(demarrerManche()));
    // les minuteurs
    connect(timerPartie, SIGNAL(timeout()), this, SLOT(chronometrerPartie()));
    connect(timerManche, SIGNAL(timeout()), this, SLOT(chronometrerManche()));
}
