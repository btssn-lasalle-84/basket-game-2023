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
#include "ui_basketgame.h"
#include <QPainter>
#include <QAction>
#include <QDebug>
#define TEST_BASKETGAME
/**
 * @brief Constructeur de la classe Basketgame
 *
 * @fn Basketgame::Basketgame
 * @param parent nullptr pour définir la fenêtre principale de l'application
 */
Basketgame::Basketgame(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::basketgame),
    puissance4(new Puissance4(this)), communication(new Communication(this)),
    tempsTour(nullptr), minuteurTour(new QTimer), nbPionsJoues(0),
    numeroManche(0), nombreManches(NB_MANCHES_MIN),
    nombrePaniers(NB_COLONNES), tempsTourConfigure(TEMPS_TOUR),
    etatBasketgame(Etat::Attente)
{
    puissance4->initialiserPlateau();

    // finManche = new QSound(SONS_FIN_MANCHE, this);
    // finSeance = new QSound(SONS_FIN_SEANCE, this);
    // tirReussi = new QSound(SONS_TIR_REUSSI, this);

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
 * @fn Basketgame::afficherEcranSeance()
 * @brief Affiche la fenêtre de Seance
 */
void Basketgame::afficherEcranSeance()
{
    qDebug() << Q_FUNC_INFO;
    afficherEcran(Basketgame::Ecran::Seance);
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
 * @fn Basketgame::ws()
 * @brief méthode pour debuter une séance
 */
void Basketgame::demarrerSeance()
{
#ifdef TEST_BASKETGAME
    configurerSeance("Avignon", "Sorgues", 3 , 15, 4);
#endif
    if((etatBasketgame == Etat::Configure || etatBasketgame == Etat::Termine) &&
       ui->ecrans->currentIndex() == Basketgame::Ecran::Seance)
    {
        ui->messageAttente->hide();
        demarrerManche(numeroManche);
    }
}

/**
 * @fn Basketgame::terminerSeance()
 * @brief méthode pour mettre fin à une séance
 */
void Basketgame::terminerSeance()
{
    afficherEcranAcceuil();
    etatBasketgame = Etat::Termine;
}

/**
 * @fn Basketgame::evaluerSeance()
 * @brief méthode pour détermiquer quand la séance est terminée
 */
void Basketgame::evaluerSeance()
{
    if(getNombreManches() == 1)
    {
        if(equipes[Rouge]->getScoreManche() > equipes[Jaune]->getScoreManche())
        {
            ui->messageVainqueur->setText("Bravo " +
                                          equipes[Rouge]->getNom() + " !");
            ui->messageVainqueur->setStyleSheet(
              "background-color: red; color: black; font: 20pt;");
        }
        else if (equipes[Rouge]->getScoreManche() < equipes[Jaune]->getScoreManche())
        {
            ui->messageVainqueur->setText("Bravo " +
                                          equipes[Jaune]->getNom() + " !");
            ui->messageVainqueur->setStyleSheet(
              "background-color: yellow; color: black; font: 20pt;");
        }
        else
        {
            ui->messageVainqueur->setText("Egalité !");
            ui->messageVainqueur->setStyleSheet(
                "background-color: white; color: black; font: 20pt;");
        }
        terminerSeance();
        ui->messageAttente->setText("");
        // finSeance->play();
        // qDebug() << Q_FUNC_INFO << "\"" << SONS_FIN_SEANCE << "\"";
    }
    else
    {
        ui->messageVainqueur->setText("");
    }
}

/**
 * @fn Basketgame::demarrerManche
 * @brief
 */
void Basketgame::demarrerManche(int numeroManche)
{
    qDebug() << Q_FUNC_INFO << "etatBasketgame" << etatBasketgame;
    if(etatBasketgame != Etat::Attente || etatBasketgame != Etat::EnCours)
    {
        qDebug() << Q_FUNC_INFO << "numeroManche" << numeroManche;
        initialiserManche();
        initialiserDureeTour();
        demarrerChronometrageTour();
        afficherPuissance4();
        etatBasketgame = Etat::EnCours;
    }
}

/**
 * @fn Basketgame::terminerManche()
 * @brief méthode pour terminer une manche
 */
void Basketgame::terminerManche(int numeroManche)
{
    qDebug() << Q_FUNC_INFO << "numeroManche" << numeroManche;
    if(etatBasketgame == Etat::EnCours)
    {
        initialiserDureeTour();
        ui->tempsTour->setText("00:00:00");
        minuteurTour->stop();
        evaluerSeance();
        decrementerNbManches();
        // finManche->play();
        // qDebug() << Q_FUNC_INFO << "\"" << SONS_FIN_MANCHE << "\"";
    }
}

/**
 * @fn Basketgame::evaluerManche()
 * @brief méthode pour déterminer quand une manche est terminée
 */
void Basketgame::evaluerManche(int numeroManche)
{
    qDebug() << Q_FUNC_INFO << "numeroManche" << numeroManche;
    qDebug() << Q_FUNC_INFO << "nbPionsJoues" << nbPionsJoues << "estVainqueur";
    if(etatBasketgame == Etat::EnCours)
    {
        if(puissance4->estVainqueur())
        {
            envoyerVainqueurManche(numeroManche, true);
            terminerManche(numeroManche);
            afficherScoreMancheEquipe();
        }
        else if(nbPionsJoues == ( NB_LIGNES * nombrePaniers ))
        {
            qDebug() << Q_FUNC_INFO << "Egalité" << nbPionsJoues;
            envoyerVainqueurManche(numeroManche, false);
            terminerManche(numeroManche);
        }
    }
}

/**
 * @fn Basketgame::envoyerVainqueurManche()
 * @brief méthode pour envoyer une trame qui contient le vainqueur d'une manche
 */
void Basketgame::envoyerVainqueurManche(int numeroManche, bool vainqueur)
{
    QString couleurVainqueur = "JAUNE";

    if(vainqueur)
    {
        if(puissance4->estEquipeRouge())
        {
            couleurVainqueur = "ROUGE";
        }
    }
    else
    {
        couleurVainqueur = "NUL";
    }
    QString envoyerTrame = QString(DELIMITEUR_DEBUT) + QString(DELIMITEUR_CHAMP) + TYPE_TRAME_FIN
            + QString(DELIMITEUR_CHAMP) + couleurVainqueur + QString(DELIMITEUR_CHAMP) + QString::number(numeroManche) + QString(DELIMITEUR_CHAMP) + QString(DELIMITEUR_FIN);
    qDebug() << Q_FUNC_INFO << envoyerTrame;
    communication->envoyer(envoyerTrame);
    etatBasketgame = Etat::FinManche;
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

void Basketgame::initialiserParametresEquipe()
{
    ui->labelVisualisationEquipeRouge->setText(equipes[Rouge]->getNom());
    ui->labelVisualisationEquipeJaune->setText(equipes[Jaune]->getNom());
    equipes[Rouge]->setScoreManche(0);
    equipes[Jaune]->setScoreManche(0);
    ui->affichageTotalPanierE1->display(QString::number(0));
    ui->affichageTotalPanierE2->display(QString::number(0));
    ui->affichageTotalMancheE1->display(QString::number(0));
    ui->affichageTotalMancheE2->display(QString::number(0));
    ui->labelVisualisationEquipeJaune->setStyleSheet(
      "background-color: white; color: black;font: 20pt");
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
    tempsTour = new QTime(0, 0, tempsTourConfigure);
}

/**
 * @fn Basketgame::initialiserCommunication
 * @brief méthode pour initialiser la communication en mode serveur
 */
void Basketgame::initialiserCommunication()
{
    if(communication != nullptr)
    {
        connect(communication,
                SIGNAL(clientConnecte()),
                this,
                SLOT(afficherEcranSeance()));
        connect(communication,
                SIGNAL(clientDeconnecte()),
                this,
                SLOT(afficherEcranAcceuil()));
        connect(communication,
                SIGNAL(mancheDemarree(int)),
                this,
                SLOT(demarrerManche(int)));
        connect(communication,
                SIGNAL(mancheTerminee(int)),
                this,
                SLOT(terminerManche(int)));
        connect(communication,
                SIGNAL(seanceConfiguree(QString, QString, int, int, int)),
                this,
                SLOT(configurerSeance(QString, QString, int, int, int)));
        connect(communication,
                SIGNAL(seanceReinitialisee()),
                this,
                SLOT(reinitialiserSeance()));
        connect(communication,
                SIGNAL(tirPanier(QString, int)),
                this,
                SLOT(gererTir(QString, int)));
        communication->demarrer();
    }
}

/**
 * @fn Basketgame::gererTir
 * @brief méthode pour gerer les tir reçu d'une trame
 */
void Basketgame::gererTir(QString couleurEquipe, int numeroPanier)
{
    if(etatBasketgame == Etat::EnCours)
    {
        convertirCouleurRecue(couleurEquipe);
        jouerPion(numeroPanier - 1);
        qDebug() << Q_FUNC_INFO << "numeroPanier" << numeroPanier
                 << "couleurEquipe" << couleurEquipe;
    }
}

/**
 * @fn Basketgame::reinitialiserSeance
 * @brief méthode pour reinitialiser la seance
 */
void Basketgame::reinitialiserSeance()
{
    qDebug() << Q_FUNC_INFO << "etatBasketgame" << etatBasketgame;
    etatBasketgame = Etat::Attente;
    afficherEcranAcceuil();
}

/**
 * @fn Basketgame::configurerSeance
 * @brief méthode pour configurer la seance
 */
void Basketgame::configurerSeance(QString nomEquipeRouge,
                                  QString nomEquipeJaune,
                                  int     nbPaniers,
                                  int     tempsTour,
                                  int     nbManches)
{
    if(etatBasketgame == Etat::Attente || etatBasketgame == Etat::Termine)
    {
        qDebug() << Q_FUNC_INFO << "nomEquipeRouge" << nomEquipeRouge
                 << "nomEquipeJaune" << nomEquipeJaune << "nombrePaniers"
                 << nbPaniers << "tempsTour" << tempsTour << "nbManches"
                 << nbManches;
        supprimerEquipes();
        equipes.push_back(new Equipe(nomEquipeRouge, this));
        equipes.push_back(new Equipe(nomEquipeJaune, this));
        nombrePaniers      = nbPaniers;
        nombreManches      = nbManches;
        tempsTourConfigure = tempsTour;
        etatBasketgame     = Etat::Configure;
        initialiserParametresEquipe();
    }
}

/**
 * @fn Basketgame::CouleurEquipe Basketgame::convertirCouleurRecue(
 * @brief méthode convertir le QString reçu de la trame tir en Enum CouleurEquipe
 */
Basketgame::CouleurEquipe Basketgame::convertirCouleurRecue(
  QString couleurEquipe)
{
    if(couleurEquipe == "Rouge")
    {
        return CouleurEquipe::Rouge;
    }
    else if(couleurEquipe == "Jaune")
    {
        return CouleurEquipe::Jaune;
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "couleurEquipe" << couleurEquipe
                 << "non reconnue";
        return CouleurEquipe::Aucune;
    }
}
/**
 * @fn Basketgame::jouerPion
 * @param colonne
 * @brief Joue un pion
 */
void Basketgame::jouerPion(int colonne)
{
    int ligne = puissance4->placerPion(colonne);
    if(etatBasketgame != Etat::FinManche)
    {
        if(ligne != -1)
        {
            afficherUnJeton(ligne, colonne);
            afficherScorePanierEquipe();
            puissance4->verifierPlateau();
            int numeroManche = getNombreManches();
            evaluerManche(numeroManche);
            afficherTourEquipe();
        }
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
              "background-color: white; color: black;font: 20pt");
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: yellow; color: black;font: 20pt;");
            puissance4->setTourEquipe(false);
        }
        else
        {
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: white; color: black;font: 20pt");
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
    qDebug() << Q_FUNC_INFO << "\"" << PLATEAU << "\"";

    ui->labelVisualisationPlateau->setPixmap(
          QPixmap(PLATEAU + QString::number(nombrePaniers) + QString(".png")));
}

/**
 * @fn Basketgame::afficherUnJeton(int ligne, int colonne)
 * @brief méthode pour afficher les jetons dans le puissance4 de puissance4
 */
void Basketgame::afficherUnJeton(int ligne, int colonne)
{
    if(ligne < 0 || ligne >= NB_LIGNES)
        return;
    if(colonne < 0 || colonne >= nombrePaniers)
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

    if(etatBasketgame == Etat::EnCours)
    {
        minuteurTour->start(TIC_HORLOGE);

        if(puissance4->estEquipeRouge())
        {
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: white; color: black; font: 20pt;");
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: yellow; color: black; font: 20pt;");
            puissance4->setTourEquipe(false);
        }
        else
        {
            ui->labelVisualisationEquipeJaune->setStyleSheet(
              "background-color: white; color: black;font: 20pt");
            ui->labelVisualisationEquipeRouge->setStyleSheet(
              "background-color: red; color: black;font: 20pt;");
            puissance4->setTourEquipe(true);
        }
    }
    else
        return;
}

/**
 * @fn Basketgame::afficherScoreEquipe
 * @brief méthode pour afficher le nombre de paniers pour chaque équipe
 */
void Basketgame::afficherScorePanierEquipe()
{
    if(etatBasketgame == Etat::EnCours)
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
        // tirReussi->play();
        // qDebug() << Q_FUNC_INFO << "\"" << SONS_TIR_REUSSI << "\"";
        nbPionsJoues++;
    }
}

/**
 * @fn Basketgame::afficherScoreMancheEquipe
 * @brief méthode pour afficher le score de manche remportée
 */
void Basketgame::afficherScoreMancheEquipe()
{
    if(etatBasketgame == Etat::EnCours)
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
    }
}

/**
 * @fn Basketgame::getNombreManche()
 * @brief Retourne le nombre de manche
 */
int Basketgame::getNombreManches() const
{
    return nombreManches;
}

/**
 * @fn Basketgame::decrementerNbManche
 * @brief méthode qui décremente le nombre de manche joué
 */
void Basketgame::decrementerNbManches()
{
    if(etatBasketgame != Etat::EnCours)
        return;
    nombreManches--;
    qDebug() << Q_FUNC_INFO << "nombreManches" << nombreManches;
}

/**
 * @fn Basketgame::supprimerEquipes
 * @brief méthode qui détruit les équipes déjà créées
 */
void Basketgame::supprimerEquipes()
{
    for(int i = 0; i < equipes.size(); ++i)
    {
        delete equipes[i];
    }
    equipes.clear();
}

#ifdef TEST_BASKETGAME
/**
 * @fn Basketgame::simulerPion()
 * @brief méthode pour simuler un coup de puissance4
 */
void Basketgame::simulerPion()
{
    if(etatBasketgame != Etat::EnCours)
        return;
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
            SLOT(afficherEcranSeance()));
    QAction* demarrageSeance = new QAction(this);
    demarrageSeance->setShortcut(QKeySequence(Qt::Key_S));
    addAction(demarrageSeance);
    connect(demarrageSeance, SIGNAL(triggered()), this, SLOT(demarrerSeance()));
    QAction* simulationPion = new QAction(this);
    simulationPion->setShortcut(QKeySequence(Qt::Key_Space));
    addAction(simulationPion);
    connect(simulationPion, SIGNAL(triggered()), this, SLOT(simulerPion()));
    QAction* simulationReinitialiser = new QAction(this);
    simulationReinitialiser->setShortcut(QKeySequence(Qt::Key_P));
    addAction(simulationReinitialiser);
    connect(simulationReinitialiser,
            SIGNAL(triggered()),
            this,
            SLOT(reinitialiserSeance()));

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
    return qrand() % ((max) - min) + min;
}
#endif
