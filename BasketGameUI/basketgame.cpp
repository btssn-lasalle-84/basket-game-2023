/**
 * @file basketgame.cpp
 *
 * @brief Définition de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.2
 */

#include "basketgame.h"
#include "ui_basketgame.h"
#include <QPainter>
#include <QAction>
#include <QDebug>

/**
 * @brief Constructeur de la classe Basketgame
 *
 * @fn Basketgame::Basketgame
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */
Basketgame::Basketgame(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::basketgame), plateau(NB_COLONNES),
    tempsTour(nullptr), timerTour(new QTimer), etatSeance(false),
    estVainqueur(false), estEquipeRouge(true), nbPionsJoues(0),
    nbPionsAlignes(NB_PIONS_ALIGNES), scoreEquipeRouge(0), scoreEquipeJaune(0)
{
    qDebug() << Q_FUNC_INFO;
    initialiserIHM();
    initialiserEvenements();
#ifdef TEST_BASKETGAME
    attribuerRaccourcisClavier();
#endif
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
    qDebug() << Q_FUNC_INFO;
    afficherEcran(Basketgame::Ecran::Accueil);
}

/**
 * @fn Basketgame::afficherEcranPartie()
 * @brief Affiche la fenêtre de partie
 */
void Basketgame::afficherEcranPartie()
{
    qDebug() << Q_FUNC_INFO;
    afficherEcran(Basketgame::Ecran::Partie);
}

/**
 * @fn Basketgame::chronometrerTour
 * @brief méthode pour chronometrer un tour de puissance4
 */
void Basketgame::chronometrerTour()
{
    // Voir aussi : QElapsedTimer
    QTime tempsEcoule = tempsTour->addSecs(-1);
    tempsTour->setHMS(tempsEcoule.hour(),
                      tempsEcoule.minute(),
                      tempsEcoule.second());
    // qDebug() << Q_FUNC_INFO << "tempsTours" <<
    // tempsTours->toString("hh:mm:ss");
    ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));

    if(*tempsTour == QTime(0, 0))
    {
        timerTour->stop();
        initialiserDureeTour();
        ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));
        timerTour->start(TIC_HORLOGE);
        qDebug() << Q_FUNC_INFO << "estEquipeRouge" << estEquipeRouge;
        if(estEquipeRouge)
        {
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: transparent; color: black;");
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: yellow; color: black;");
            estEquipeRouge = false;
        }
        else
        {
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: transparent; color: black;");
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: red; color: black;");
            estEquipeRouge = true;
        }
    }
}

/**
 * @fn Basketgame::simulerPion()
 * @brief méthode pour simuler un coup de puissance4
 */
void Basketgame::simulerPion()
{
    int colonne = randInt(0, NB_COLONNES - 1);
    int ligne   = 0;

    for(ligne = 0; ligne < NB_LIGNES; ++ligne)
    {
        if(colonne >= 0 && colonne < NB_COLONNES &&
           plateau[colonne][ligne] == CouleurJeton::AUCUNE)
        {
            // CouleurJeton couleurJeton =
            // (CouleurJeton)randInt(CouleurJeton::ROUGE, CouleurJeton::JAUNE);
            plateau[colonne][ligne] =
              (estEquipeRouge ? CouleurJeton::ROUGE : CouleurJeton::JAUNE);
            qDebug() << Q_FUNC_INFO << "pion"
                     << (estEquipeRouge ? CouleurJeton::ROUGE
                                        : CouleurJeton::JAUNE)
                     << "ligne" << ligne + 1 << "colonne" << colonne + 1;
            afficherUnJeton(ligne, colonne);
            break;
        }
    }
}

/**
 * @fn Basketgame::fermerApplication()
 * @brief méthode pour fermer l'application
 */
void Basketgame::fermerApplication()
{
    this->close();
}

/**
 * @fn Basketgame::demarrerSeance()
 * @brief méthode pour debuter une séance
 */
void Basketgame::demarrerSeance()
{
    if(!etatSeance && ui->ecrans->currentIndex() == Basketgame::Ecran::Partie)
    {
        etatSeance = true;
        demarrerPartie();
        ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));
        qDebug() << Q_FUNC_INFO << "tempsTour"
                 << tempsTour->toString("hh:mm:ss");
        timerTour->start(TIC_HORLOGE);
    }
}

/**
 * @fn Basketgame::terminerSeance()
 * @brief méthode pour détermiquer quand la séance est terminer
 */
void Basketgame::terminerSeance()
{
    qDebug() << Q_FUNC_INFO << "nbPionsJoues" << nbPionsJoues << "estVainqueur"
             << estVainqueur;
    if(estVainqueur)
    {
        etatSeance = false;
        initialiserDureeTour();
        ui->tempsTour->setText("00:00:00");
    }
    else if(nbPionsJoues == NB_PIONS)
    {
        etatSeance = false;
        initialiserDureeTour();
        ui->tempsTour->setText("00:00:00");
    }
}

/**
 * @fn Basketgame::demarrerPartie
 * @brief
 */
void Basketgame::demarrerPartie()
{
    if(etatSeance)
    {
        qDebug() << Q_FUNC_INFO;
        estVainqueur     = false;
        estEquipeRouge   = true;
        nbPionsJoues     = 0;
        nbPionsAlignes   = NB_PIONS_ALIGNES;
        scoreEquipeRouge = 0;
        scoreEquipeJaune = 0;
        initialiserDureeTour();
        initialiserPlateau();
    }
}

/**
 * @fn Basketgame::initialiserIHM()
 * @brief méthode pour initialiser l'IHM
 */
void Basketgame::initialiserIHM()
{
    ui->setupUi(this);

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
    // le minuteur
    connect(timerTour, SIGNAL(timeout()), this, SLOT(chronometrerTour()));
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

    qDebug() << Q_FUNC_INFO << "colonnes" << plateau.size() << "lignes"
             << plateau[0].size();

    for(int colonne = 0; colonne < plateau.size(); ++colonne)
    {
        for(int ligne = 0; ligne < plateau[colonne].size(); ++ligne)
        {
            plateau[colonne][ligne] = CouleurJeton::AUCUNE;
        }
    }
    afficherPlateau();
}

/**
 * @fn Basketgame::initialiserDureeTour()
 * @brief méthode pour initialiser la durée d'un tour pour une équipe
 */
void Basketgame::initialiserDureeTour()
{
    if(tempsTour != nullptr)
    {
        delete tempsTour;
        tempsTour = nullptr;
    }
    tempsTour = new QTime(0, 0, TEMPS_TOUR);
}

/**
 * @fn Basketgame::afficherPlateau()
 * @brief méthode pour afficher le plateau de puissance4
 */
void Basketgame::afficherPlateau()
{
    qDebug() << Q_FUNC_INFO << "\"" << PLATEAU_7 << "\"";
    ui->labelVisualisationPlateau->setPixmap(QPixmap(PLATEAU_7));
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

    QImage   jetonRouge(JETON_ROUGE);
    QImage   jetonJaune(JETON_JAUNE);
    QPixmap  puissance4 = ui->labelVisualisationPlateau->pixmap()->copy();
    QPainter p(&puissance4);

    qDebug() << Q_FUNC_INFO << "rouge" << estEquipeRouge << "ligne" << ligne
             << "colonne" << colonne;
    if(estEquipeRouge)
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne * TAILLE_JETON)),
                    jetonRouge);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(puissance4);
        afficherScoreEquipe();
        nbPionsJoues++;
        afficherTourEquipe();
    }
    else
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne * TAILLE_JETON)),
                    jetonJaune);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(puissance4);
        afficherScoreEquipe();
        afficherTourEquipe();
    }
    terminerSeance();
}

/**
 * @fn Basketgame::afficherTourEquipe()
 * @brief méthode pour afficher quelle équipe est train de jouer
 */
void Basketgame::afficherTourEquipe()
{
    qDebug() << Q_FUNC_INFO << estEquipeRouge;

    timerTour->stop();
    initialiserDureeTour();
    ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));
    timerTour->start(TIC_HORLOGE);

    if(estEquipeRouge)
    {
        ui->labelVisualisationEquipeRouge->setStyleSheet(
          "background-color: transparent; color: black;");
        ui->labelVisualisationEquipeJaune->setStyleSheet(
          "background-color: yellow; color: black;");
        estEquipeRouge = false;
    }
    else
    {
        ui->labelVisualisationEquipeJaune->setStyleSheet(
          "background-color: transparent; color: black;");
        ui->labelVisualisationEquipeRouge->setStyleSheet(
          "background-color: red; color: black;");
        estEquipeRouge = true;
    }
}

/**
 * @fn Basketgame::afficherScoreEquipe
 * @brief méthode pour afficher le nombre de paniers pour chaque équipe
 */
void Basketgame::afficherScoreEquipe()
{
    if(estEquipeRouge)
    {
        scoreEquipeRouge++;
        ui->affichageTotalPanierE1->display(QString::number(scoreEquipeRouge));
    }
    else
    {
        scoreEquipeJaune++;
        ui->affichageTotalPanierE2->display(QString::number(scoreEquipeJaune));
    }
    nbPionsJoues++;
}

/**
 * @fn Basketgame::randInt(int min, int max)
 * @brief méthode pour créer une variable aléatoire
 */
int Basketgame::randInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}

#ifdef TEST_BASKETGAME
/**
 * @fn Basketgame::attribuerRaccourcisClavier
 * @brief méthode pour créer les raccourcis clavier (pour les tests seulement)
 */
void Basketgame::attribuerRaccourcisClavier()
{
    // les touches pour "jouer" avec le clavier
    QAction* quitter = new QAction(this);
    quitter->setShortcut(QKeySequence(QKeySequence(Qt::Key_Q)));
    addAction(quitter);
    connect(quitter, SIGNAL(triggered()), this, SLOT(fermerApplication()));
    QAction* simulationConnexion = new QAction(this);
    simulationConnexion->setShortcut(QKeySequence(Qt::Key_D));
    addAction(simulationConnexion);
    connect(simulationConnexion,
            SIGNAL(triggered()),
            this,
            SLOT(afficherEcranPartie()));
    QAction* demarrageSeance = new QAction(this);
    demarrageSeance->setShortcut(QKeySequence(Qt::Key_S));
    addAction(demarrageSeance);
    connect(demarrageSeance, SIGNAL(triggered()), this, SLOT(demarrerSeance()));
    QAction* simulationPion = new QAction(this);
    simulationPion->setShortcut(QKeySequence(Qt::Key_Space));
    addAction(simulationPion);
    connect(simulationPion, SIGNAL(triggered()), this, SLOT(simulerPion()));
}
#endif
