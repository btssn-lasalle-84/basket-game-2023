/**
 * @file basketgame.cpp
 *
 * @brief Définition de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.2
 */

#include "basketgame.h"
#include "puissance4.h"
#include "communication.h"
#include "equipe.h"
#include "seance.h"
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
    QMainWindow(parent), ui(new Ui::basketgame),
    puissance4(new Puissance4(this)), communication(new Communication(this)),
    seance(new Seance(this)),
    tempsTour(nullptr), minuteurTour(new QTimer),
    nbPionsJoues(0), etatManche(false)
{
    equipes.push_back(new Equipe("Rouge", this));
    equipes.push_back(new Equipe("Jaune", this));
    seance->setNombreManche(10
                            );
    qDebug() << Q_FUNC_INFO;
    initialiserIHM();
    initialiserEvenements();
    initialiserCommunication();
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
 * @fn Basketgame::fermerApplication()
 * @brief méthode pour fermer l'application
 */
void Basketgame::fermerApplication()
{
    this->close();
}

/**
 * @fn Basketgame::demarrerPartie
 * @brief
 */
void Basketgame::demarrerManche()
{
    if(etatSeance)
    {
        qDebug() << Q_FUNC_INFO;
        initialiserManche();
        initialiserDureeTour();
        demarrerChronometrageTour();
        afficherPuissance4();
    }
}

/**
 * @fn Basketgame::terminerManche()
 * @brief méthode pour détermiquer quand une manche est terminer
 */
void Basketgame::terminerManche()
{
    if(puissance4->estVainqueur())
    {
        initialiserDureeTour();
        ui->tempsTour->setText("00:00:00");
        minuteurTour->stop();
        qDebug() << Q_FUNC_INFO << "nbPionsJoues" << nbPionsJoues
                 << "estVainqueur" << puissance4->estVainqueur();
        demarrerManche();
        afficherScoreMancheEquipe();
        terminerSeance();
    }
    else if(nbPionsJoues == NB_PIONS)
    {
        etatSeance = false;
        initialiserDureeTour();
        ui->tempsTour->setText("00:00:00");
        minuteurTour->stop();
        afficherScoreMancheEquipe();
        terminerSeance();
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
 * @fn Basketgame::initialiserManche
 * @brief méthode pour initialiser les variables d'une manche
 */
void Basketgame::initialiserManche()
{
    nbPionsJoues = 0;
    equipes[Rouge]->setScorePanier(0);
    equipes[Jaune]->setScorePanier(0);
    ui->affichageTotalPanierE1->display(QString::number(0));
    ui->affichageTotalPanierE2->display(QString::number(0));
    puissance4->initialiserPlateau();
}

/**
 * @fn Basketgame::initialiserParametresEquipe()
 * @brief méthode pour initialiser les paramètres des équipes
 */

void Basketgame::initialiserParametreEquipe()
{
    ui->labelVisualisationEquipeRouge->setText(equipes[Rouge]->getNom());
    ui->labelVisualisationEquipeJaune->setText(equipes[Jaune]->getNom());
    ui->affichageTotalPanierE1->display(QString::number(0));
    ui->affichageTotalPanierE2->display(QString::number(0));
    ui->affichageTotalMancheE1->display(QString::number(0));
    ui->affichageTotalMancheE2->display(QString::number(0));
    ui->labelVisualisationEquipeJaune->setStyleSheet(
      "background-color: transparent; color: transparent;");
    ui->labelVisualisationEquipeRouge->setStyleSheet(
      "background-color: red; color: black;font: 20pt;");
}
/**
 * @fn Basketgame::initialiserEvenements()
 * @brief méthode pour initialiser les évenements et action du programme
 */
void Basketgame::initialiserEvenements()
{
    // le minuteur
    connect(minuteurTour, SIGNAL(timeout()), this, SLOT(chronometrerTour()));
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
 * @fn Basketgame::initialiserCommunication
 * @brief méthode pour initialiser la communication en mode serveur
 */
void Basketgame::initialiserCommunication()
{
    if(communication != nullptr)
    {
        connect(communication, SIGNAL(partieDemarree(int)), this, SLOT(demarrerSeance(int numeroPartie)));
        connect(communication, SIGNAL(partieArretee(int)), this, SLOT(terminerSeance(int numeroPartie)));
        connect(communication, SIGNAL(partieConfiguree(QString,QString,int,int,int)), this, SLOT(configuerSeance(QString nomEquipeRouge,QString nomEquipeJaune,int nombrePaniers,int tempsTour,int nbManches)));
        connect(communication, SIGNAL(partieReinitialisee()), this, SLOT(reinitialiseSeance()));

        communication->demarrer();
    }
}
/**
 * @fn Basketgame::demarrerSeance()
 * @brief méthode pour debuter une séance
 */
void Basketgame::demarrerSeance(int numeroPartie)
{
    if(ui->ecrans->currentIndex() == Basketgame::Ecran::Partie)
    {
        ui->messageAttente->hide();

    }
}

/**
 * @fn Basketgame::terminerSeance()
 * @brief méthode pour détermiquer quand la séance est terminer
 */
void Basketgame::terminerSeance(int numeroPartie)
{
    if(seance->getNombreManche() == 0)
    {
        afficherEcranAcceuil();
        if(equipes[Rouge]->getScoreManche() > equipes[Jaune]->getScoreManche())
        {
               ui->messageVainqueur->setText("Vainqueur est : " + equipes[Rouge]->getNom());
               ui->messageVainqueur->setStyleSheet(
                 "background-color: red; color: black; font: 20pt;");
        }
        else
        {
            ui->messageVainqueur->setText("Vainqueur est : " + equipes[Jaune]->getNom());
            ui->messageVainqueur->setStyleSheet(
              "background-color: yellow; color: black; font: 20pt;");
        }
        etatSeance = false;
        demarrerSeance();
    }
}

void Basketgame::reinitialiseSeance()
{
    qDebug() << Q_FUNC_INFO << etatManche ;
}

void Basketgame::configuerSeance(QString nomEquipeRouge,QString nomEquipeJaune,int nombrePaniers,int tempsTour,int nbManches)
{
    qDebug() << Q_FUNC_INFO << "information recus : " << nomEquipeRouge << nomEquipeJaune << nombrePaniers << tempsTour << nbManches;
}

/**
 * @fn Basketgame::jouerPion
 * @param colonne
 * @brief Joue un pion
 */
void Basketgame::jouerPion(int colonne)
{
    int ligne = puissance4->placerPion(colonne);
    if(ligne != -1)
    {
        afficherUnJeton(ligne, colonne);
        afficherScorePanierEquipe();
        puissance4->verifierPlateau();
        terminerManche();
        afficherTourEquipe();
    }
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
        minuteurTour->stop();
        initialiserDureeTour();
        ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));
        minuteurTour->start(TIC_HORLOGE);
        qDebug() << Q_FUNC_INFO << "estEquipeRouge"
                 << puissance4->estEquipeRouge();
        if(puissance4->estEquipeRouge())
        {
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: transparent; color: transparent;");
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: yellow; color: black;font: 20pt;");
            puissance4->setTourEquipe(false);
        }
        else
        {
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: transparent; color: transparent;");
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: red; color: black;font: 20pt;");
            puissance4->setTourEquipe(true);
        }
    }
}

/**
 * @fn Basketgame::demarrerChronometrageTour
 * @brief méthode pour démarrer le chronométrage d'un tour
 */
void Basketgame::demarrerChronometrageTour()
{
    ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));
    qDebug() << Q_FUNC_INFO << "tempsTour" << tempsTour->toString("hh:mm:ss");
    minuteurTour->start(TIC_HORLOGE);
}

/**
 * @fn Basketgame::afficherPuissance4()
 * @brief méthode pour afficher le puissance4 de puissance4
 */
void Basketgame::afficherPuissance4()
{
    qDebug() << Q_FUNC_INFO << "\"" << PLATEAU_7 << "\"";
    ui->labelVisualisationPlateau->setPixmap(QPixmap(PLATEAU_7));
}

/**
 * @fn Basketgame::afficherUnJeton(int ligne, int colonne)
 * @brief méthode pour afficher les jetons dans le puissance4 de puissance4
 */
void Basketgame::afficherUnJeton(int ligne, int colonne)
{
    if(ligne < 0 || ligne >= NB_LIGNES)
        return;
    if(colonne < 0 || colonne >= NB_COLONNES)
        return;

    QImage  jetonRouge(JETON_ROUGE);
    QImage  jetonJaune(JETON_JAUNE);
    QPixmap plateauPuissance4 = ui->labelVisualisationPlateau->pixmap()->copy();
    QPainter p(&plateauPuissance4);

    qDebug() << Q_FUNC_INFO << "rouge" << puissance4->estEquipeRouge()
             << "ligne" << ligne << "colonne" << colonne;
    if(puissance4->estEquipeRouge())
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne * TAILLE_JETON)),
                    jetonRouge);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(plateauPuissance4);
    }
    else
    {
        p.drawImage(QPoint(DEPLACEMENT_X + (colonne * TAILLE_JETON),
                           DEPLACEMENT_Y - (ligne * TAILLE_JETON)),
                    jetonJaune);
        p.end();
        ui->labelVisualisationPlateau->setPixmap(plateauPuissance4);
    }
}

/**
 * @fn Basketgame::afficherTourEquipe()
 * @brief méthode pour afficher quelle équipe est train de jouer
 */
void Basketgame::afficherTourEquipe()
{
    minuteurTour->stop();
    initialiserDureeTour();
    ui->tempsTour->setText(tempsTour->toString("hh:mm:ss"));

    if(!etatSeance)
        return;

    minuteurTour->start(TIC_HORLOGE);

    if(puissance4->estEquipeRouge())
    {
        ui->labelVisualisationEquipeRouge->setStyleSheet(
          "background-color: transparent; color: transparent;");
        ui->labelVisualisationEquipeJaune->setStyleSheet(
          "background-color: yellow; color: black; font: 20pt;");
        puissance4->setTourEquipe(false);
    }
    else
    {
        ui->labelVisualisationEquipeJaune->setStyleSheet(
          "background-color: transparent; color: transparent;");
        ui->labelVisualisationEquipeRouge->setStyleSheet(
          "background-color: red; color: black;font: 20pt;");
        puissance4->setTourEquipe(true);
    }
}

/**
 * @fn Basketgame::afficherScoreEquipe
 * @brief méthode pour afficher le nombre de paniers pour chaque équipe
 */
void Basketgame::afficherScorePanierEquipe()
{
    if(puissance4->estEquipeRouge())
    {
        equipes[Rouge]->incrementerScorePanier();
        ui->affichageTotalPanierE1->display(
        QString::number(equipes[Rouge]->getScorePanier()));
    }
    else
    {
        equipes[Jaune]->incrementerScorePanier();
        ui->affichageTotalPanierE2->display(
        QString::number(equipes[Jaune]->getScorePanier()));
    }
    nbPionsJoues++;
}
void Basketgame::afficherScoreMancheEquipe()
{
    if(puissance4->estEquipeRouge())
    {
        equipes[Rouge]->incrementerScoreManche();
        ui->affichageTotalMancheE1->display(
        QString::number(equipes[Rouge]->getScoreManche()));
    }
    else
    {
        equipes[Jaune]->incrementerScoreManche();
        ui->affichageTotalMancheE2->display(
        QString::number(equipes[Jaune]->getScoreManche()));
    }
    seance->decrementerNbManche();
}

#ifdef TEST_BASKETGAME
/**
 * @fn Basketgame::simulerPion()
 * @brief méthode pour simuler un coup de puissance4
 */
void Basketgame::simulerPion()
{
    if(seance->etatSeance)
        return;
    // simule un pion dans une colonne
    int colonne = randInt(0, NB_COLONNES - 1);
    // et le joue
    jouerPion(colonne);
}
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
    simulationConnexion->setShortcut(QKeySequence(Qt::Key_C));
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
#ifdef TEST_ALIGNEMENT
    QAction* verificationPuissance4 = new QAction(this);
    verificationPuissance4->setShortcut(QKeySequence(Qt::Key_V));
    addAction(verificationPuissance4);
    connect(verificationPuissance4,
            SIGNAL(triggered()),
            puissance4,
            SLOT(testUnitaireVerifierPlateau()));
#endif
}

/**
 * @fn Plateau::randInt(int min, int max)
 * @brief méthode qui retourne un nombre entier pseudo-aléatoire entre min et
 * max
 */
int Basketgame::randInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}
#endif
