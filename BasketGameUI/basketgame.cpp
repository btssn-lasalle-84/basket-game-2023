/**
 * @file basketgame.cpp
 *
 * @brief Définition de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.1
 */

#include "basketgame.h"
#include "ui_basketgame.h"
#include <QPainter>
#include <QDebug>

#include <QAction>

/**
 * @brief Constructeur de la classe Basketgame
 *
 * @fn Basketgame::Basketgame
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */
Basketgame::Basketgame(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::basketgame),
    tempsTours(new QTime),timerTours(new QTimer),
    plateau(NB_COLONNES),etatTours(false),ToursJoueurs(0),
    nombreColonnes(NB_COLONNES),nbPionsAlignes(NB_PIONS_ALIGNES),
    couleurEquipe(true), scoreEquipeRouge(0) , scoreEquipeJaune(0),
    estVainqueur(false)
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
    ui->boutonDebutManche->setEnabled(true);
    ui->editionTempsTours->setEnabled(true);
    afficherEcran(Basketgame::Ecran::Partie);
}
void Basketgame::demarrerSeance()
{
    if(!etatTours)
    {
        *tempsTours = ui->editionTempsTours->time();
        ui->editionTempsTours->setEnabled(false);
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsManche"
                 << tempsTours->toString("hh:mm:ss");
        /**
         * @fixme Si le temps est 00:00:00 ?
         */
        timerTours->start(TIC_HORLOGE);
        etatTours = true;
        ui->boutonDebutManche->setEnabled(false);
        ui->editionTempsTours->setEnabled(false);

    }
}
void Basketgame::terminerPartie()
{
    /**
     * @todo Gérer la fin d'une partie
     */
    etatTours = false;
    ui->tempsTours->setText("00:00:00");
    ui->boutonDebutManche->setEnabled(false);
    ui->editionTempsTours->setEnabled(false);

}
void Basketgame::chronometrerTours()
{
    // Voir aussi : QElapsedTimer
    QTime tempsEcoule = tempsTours->addSecs(-1);
    tempsTours->setHMS(tempsEcoule.hour(),
                        tempsEcoule.minute(),
                        tempsEcoule.second());
    qDebug() << Q_FUNC_INFO << "tempsManche"
             << tempsTours->toString("hh:mm:ss");
    ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));

    if(*tempsTours == QTime(0, 0))
    {
        timerTours->stop();
        *tempsTours = ui->editionTempsTours->time();
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        timerTours->start(1000);

        if(couleurEquipe == true)
        {
            ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: transparent; color: black;");
            ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: yellow; color: black;");
            couleurEquipe = false;
        }
        else
        {
            ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: transparent; color: black;");
            ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: red; color: black;");
            couleurEquipe = true;
        } 
    }
    else if (estVainqueur = true)
    {
        terminerPartie();
        timerTours->stop();
    }
}
void Basketgame::incrementerLcdNumberPointsEquipeRouge() {
    scoreEquipeRouge++;
        ui->afficherTotalPanierE1->display(QString::number(scoreEquipeRouge));
    }
void Basketgame::incrementerLcdNumberPointsEquipeJaune() {
    scoreEquipeJaune++;
        ui->afficherTotalPanierE2->display(QString::number(scoreEquipeJaune));
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
    connect(ui->boutonDebutManche,
            SIGNAL(clicked(bool)),
            this,
            SLOT(demarrerSeance()));
    connect(ui->boutonDemarrer,
            SIGNAL(clicked()),
            this,
            SLOT(initialiserPlateau()));

    // les Minuteurs

    connect(timerTours, SIGNAL(timeout()), this, SLOT(chronometrerTours()));

    // les raccourcis clavier

    QAction* actionPion1 = new QAction(this);
    actionPion1->setShortcut(QKeySequence(Qt::Key_A));
    addAction(actionPion1);
    connect(actionPion1, SIGNAL(triggered()), this, SLOT(simulerPion()));

}

void Basketgame::afficherPlateau()
{
    ui->labelVisualisationPlateau->setPixmap(
    QPixmap(":/ressources/puissance4_7.png"));
}

void Basketgame::initialiserPlateau()
{
    for(int i = 0; i < plateau.size(); ++i)
    {
        plateau[i].resize(NB_LIGNES);
    }
    qDebug() << Q_FUNC_INFO << plateau.size() << plateau[0].size();

        for(int colonne = 0; colonne < plateau.size(); ++colonne)
        {
            for(int ligne = 0; ligne < plateau[colonne].size(); ++ligne)
            {
                plateau[colonne][ligne] = CouleurJeton::AUCUN;
            }
        }
        afficherPlateau();
}

void Basketgame::afficherUnJeton(int ligne, int colonne)
{
    if(ligne < 0 || ligne >= NB_LIGNES)
        return;
    if(colonne < 0 || colonne >= NB_COLONNES)
        return;

    qDebug() << Q_FUNC_INFO << ligne << colonne;

    QImage  jetonRouge(":/ressources/jetonRouge.png");
    QImage  jetonJaune(":/ressources/jetonJaune.png");
    QPixmap puissance4 = ui->labelVisualisationPlateau->pixmap()->copy();
    // on récupère l'image précédente
    QPainter p(&puissance4);

    if (ToursJoueurs % 2 == 0)
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne *   TAILLE_JETON)),
                           jetonRouge);
        qDebug() << Q_FUNC_INFO << DEPLACEMENT_X << DEPLACEMENT_Y << TAILLE_JETON;
    p.end();
    ui->labelVisualisationPlateau->setPixmap(puissance4);
    incrementerLcdNumberPointsEquipeRouge();
    ToursJoueurs++;
    }
    else if (ToursJoueurs % 2 == 1)
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne *   TAILLE_JETON)),
                           jetonJaune);
        qDebug() << Q_FUNC_INFO << DEPLACEMENT_X << DEPLACEMENT_Y << TAILLE_JETON;
    p.end();
    ui->labelVisualisationPlateau->setPixmap(puissance4);
    incrementerLcdNumberPointsEquipeJaune();
    ToursJoueurs++;
    }
    else
    {
        qDebug() << Q_FUNC_INFO << DEPLACEMENT_X << DEPLACEMENT_Y << TAILLE_JETON;
    }
}

void Basketgame::simulerPion()
{
    qDebug() << Q_FUNC_INFO << plateau.size() << plateau[0].size();
        int ligne   = randInt(0, NB_LIGNES - 1);
        int colonne = randInt(0, NB_COLONNES - 1);
        // case libre ?

        if(plateau[colonne][ligne] == CouleurJeton::AUCUN)
        {
            plateau[colonne][ligne] = CouleurJeton::ROUGE;
            afficherUnJeton(ligne, colonne);
        }

}
int Basketgame::randInt(int min, int max)
        {
            return qrand() % ((max + 1) - min) + min;
        }
