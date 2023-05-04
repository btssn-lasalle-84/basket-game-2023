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
    nbPionsAlignes(NB_PIONS_ALIGNES),estEquipeRouge(true),
    scoreEquipeRouge(0) , scoreEquipeJaune(0),
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

/**
 * @fn Basketgame::afficherEcranAcceuil()
 * @brief Affiche la fenêtre d'accueil
 */
void Basketgame::afficherEcranAcceuil()
{
    afficherEcran(Basketgame::Ecran::Accueil);
}

/**
 * @fn Basketgame::afficherEcranPartie()
 * @brief Affiche la fenêtre de partie
 */
void Basketgame::afficherEcranPartie()
{
    ui->boutonDebutManche->setEnabled(true);
    ui->editionTempsTours->setEnabled(true);
    afficherEcran(Basketgame::Ecran::Partie);
}

/**
 * @fn Basketgame::demarrerSeance()
 * @brief méthode pour debuter une séance
 */
void Basketgame::demarrerSeance()
{
    if(!etatTours)
    {
        *tempsTours = ui->editionTempsTours->time();
        ui->editionTempsTours->setEnabled(false);
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "Debut Seance";
        timerTours->start(TIC_HORLOGE);

        etatTours = true;
        ui->boutonDebutManche->setEnabled(false);
        ui->editionTempsTours->setEnabled(false);

    }
}

/**
 * @fn Basketgame::terminerSeance()
 * @brief méthode pour détermiquer quand la séance est terminer
 */
void Basketgame::terminerSeance()
{
    if (estVainqueur == true)
    {
    etatTours = false;
    ui->tempsTours->setText("00:00:00");
    ui->boutonDebutManche->setEnabled(true);
    ui->editionTempsTours->setEnabled(true);
    }
    else if(ToursJoueurs == 42)
    {
        etatTours = false;
        ui->tempsTours->setText("00:00:00");
        ui->boutonDebutManche->setEnabled(true);
        ui->editionTempsTours->setEnabled(true);
        qDebug() << Q_FUNC_INFO << "Partie Nul";
        qDebug() << Q_FUNC_INFO << ToursJoueurs;
    }
}

/**
 * @fn Basketgame::chronometrerTours()
 * @brief méthode pour chronometrer un tour de puissance4
 */
void Basketgame::chronometrerTours()
{
    // Voir aussi : QElapsedTimer
    QTime tempsEcoule = tempsTours->addSecs(-1);
    tempsTours->setHMS(tempsEcoule.hour(),
                        tempsEcoule.minute(),
                        tempsEcoule.second());
   // qDebug() << Q_FUNC_INFO << "tempsTours" << tempsTours->toString("hh:mm:ss");
    ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));

    if(*tempsTours == QTime(0, 0))
    {
        timerTours->stop();
        *tempsTours = ui->editionTempsTours->time();
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        timerTours->start(1000);

    if( estEquipeRouge == true)
           {
            ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: transparent; color: black;");
            ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: yellow; color: black;");
            qDebug() << Q_FUNC_INFO << estEquipeRouge ;
            estEquipeRouge = false;
           }
     else
           {
            ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: transparent; color: black;");
            ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: red; color: black;");
            qDebug() << Q_FUNC_INFO << estEquipeRouge ;
            estEquipeRouge = true;
           }
    }
}

/**
 * @fn Basketgame::incrementerVisualisationLcdNumber()
 * @brief méthode pour incrementer l'UI en fonction du nombre de panier
 */
void Basketgame::incrementerVisualisationLcdNumber()
{
    if(ToursJoueurs % 2 == 0)
    {
        scoreEquipeRouge++;
        ui->afficherTotalPanierE1->display(QString::number(scoreEquipeRouge));
    }
    else if (ToursJoueurs % 2 == 1)
    {
        scoreEquipeJaune++;
        ui->afficherTotalPanierE2->display(QString::number(scoreEquipeJaune));
    }
}

/**
 * @fn Basketgame::initialiserIHM()
 * @brief méthode pour initialiser l'IHM
 */
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

/**
 * @fn Basketgame::initialiserEvenements()
 * @brief méthode pour initialiser les évenements et action du programme
 */
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
    QAction* quitterProgramme = new QAction(this);
    quitterProgramme->setShortcut((QKeySequence(Qt::Key_Q)));
    addAction(quitterProgramme);
    connect(quitterProgramme,
            SIGNAL(triggered()),
            this,
            SLOT(fermerApplication()));
}

/**
 * @fn Basketgame::afficherPlateau()
 * @brief méthode pour afficher le plateau de puissance4
 */
void Basketgame::afficherPlateau()
{
    ui->labelVisualisationPlateau->setPixmap(
    QPixmap(":/ressources/puissance4_7.png"));
}

/**
 * @fn Basketgame::initialiserPlateau()
 * @brief méthode pour initialiser le plateau de puissance4
 */
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

/**
 * @fn Basketgame::afficherUnJeton(int ligne, int colonne)
 * @brief méthode pour afficher les jetons dans le plateau de puissance4
 */
void Basketgame::afficherUnJeton(int ligne, int colonne)
{
    if(ligne < 0 || ligne >= NB_LIGNES)
        return;
    if(colonne < 0 || colonne >= NB_COLONNES)
        return;

    QImage  jetonRouge(":/ressources/jetonRouge.png");
    QImage  jetonJaune(":/ressources/jetonJaune.png");
    QPixmap puissance4 = ui->labelVisualisationPlateau->pixmap()->copy();
    QPainter p(&puissance4);

    if ( estEquipeRouge == true )
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne *   TAILLE_JETON)),
                           jetonRouge);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(puissance4);
        incrementerVisualisationLcdNumber();
        ToursJoueurs++;
        qDebug() << Q_FUNC_INFO << "Pion rouge placer" ;
        afficherToursEquipe();
    }
    else if ( estEquipeRouge == false)
    {

        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne *   TAILLE_JETON)),
                           jetonJaune);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(puissance4);
        incrementerVisualisationLcdNumber();
        ToursJoueurs++;
        qDebug() << Q_FUNC_INFO << "Pion jaune placer";
        afficherToursEquipe();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Erreur placement pion";
    }
    terminerSeance();
}

/**
 * @fn Basketgame::afficherToursEquipe()
 * @brief méthode pour afficher qu'elle équipe est train de jouer
 */
void Basketgame::afficherToursEquipe()
{
    if ( estEquipeRouge == true  )
    {
        timerTours->stop();
        *tempsTours = ui->editionTempsTours->time();
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        timerTours->start(1000);
        ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: transparent; color: black;");
        ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: yellow; color: black;");
        qDebug() << Q_FUNC_INFO << estEquipeRouge ;
        estEquipeRouge = false;
    }
    else
    {
        timerTours->stop();
        *tempsTours = ui->editionTempsTours->time();
        ui->tempsTours->setText(tempsTours->toString("hh:mm:ss"));
        timerTours->start(1000);
        ui->labelVisualisationEquipeJaune->setStyleSheet("background-color: transparent; color: black;");
        ui->labelVisualisationEquipeRouge->setStyleSheet("background-color: red; color: black;");
        qDebug() << Q_FUNC_INFO << estEquipeRouge ;
        estEquipeRouge = true;

    }
}

/**
 * @fn Basketgame::simulerPion()
 * @brief méthode pour simuler un coup de puissance4
 */
void Basketgame::simulerPion()
{
        int colonne = randInt(0, NB_COLONNES - 1);
        int ligne = 0 ;

        for (ligne = 0; ligne < NB_LIGNES; ++ligne)
        {
            if (colonne >= 0 && colonne < NB_COLONNES && plateau[colonne][ligne] == CouleurJeton::AUCUN)
            {
                plateau[colonne][ligne] = CouleurJeton::COULEUR;
                afficherUnJeton(ligne, colonne);
                break;
            }
        }
        qDebug() << Q_FUNC_INFO << "ligne" << ligne + 1 << "colonne" << colonne + 1;
}

/**
 * @fn Basketgame::randInt(int min, int max)
 * @brief méthode pour créer une variable aléatoire
 */
int Basketgame::randInt(int min, int max)
        {
            return qrand() % ((max + 1) - min) + min;
        }

/**
 * @fn Basketgame::fermerApplication()
 * @brief méthode pour fermer l'application
 */
void Basketgame::fermerApplication()
{
    this->close();
}
