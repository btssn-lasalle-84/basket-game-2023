#ifndef BASKETGAME_H
#define BASKETGAME_H

/**
 * @file basketgame.h
 *
 * @brief Déclaration de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.2
 */
#include <QtWidgets>
#include <QVector>

/**
 * @def TEST_BASKETGAME
 * @brief Pour les tests avec le clavier
 */
#define TEST_BASKETGAME

/**
 * @def PLEIN_ECRAN_RPI
 * @brief Pour le mode plein écran sur la Raspberry Pi
 */
//#define PLEIN_ECRAN_RPI

/**
 * @def TIC_HORLOGE
 * @brief Définit le tic d'horloge à 1s
 */
#define TIC_HORLOGE 1000

/**
 * @def TEMPS_TOUR
 * @brief Le temps pour un tour d'une équipe
 */
#define TEMPS_TOUR 30 // en s

/**
 * @def NB_PANIERS
 * @brief Le nombre de colonne maximum
 */
#define NB_PANIERS_MAX 7

/**
 * @def NB_MANCHES
 * @brief Le nombre de manches par défaut
 */
#define NB_MANCHES_MIN 1

/**
 * @def TAILLE_JETON
 * @brief Définit la taille de l'affichage du jeton
 */
#define TAILLE_JETON 60

/**
 * @def DEPLACEMENT_X
 * @brief Définit la constante de déplacement en x
 */
#define DEPLACEMENT_X 42

/**
 * @def DEPLACEMENT_Y
 * @brief Définit la constante de déplacement en y
 */
#define DEPLACEMENT_Y 321

/**
 * @def NB_PIONS_MAX
 * @brief Définit le nombre maximum de pions possible
 */
#define NB_PIONS  42
/**
 * @def JETON_ROUGE
 * @brief Définit l'image d'un jeton rouge
 */
#define JETON_ROUGE ":/ressources/images/jetonRouge.png"

/**
 * @def JETON_JAUNE
 * @brief Définit l'image d'un jeton jaune
 */
#define JETON_JAUNE ":/ressources/images/jetonJaune.png"

/**
 * @def PLATEAU
 * @brief Définit l'image d'un plateau de 2 à 7 colonnes
 */
#define PLATEAU ":/ressources/images/puissance4_"

/**
 * @def SONS_FIN_SEANCE
 * @brief Définit le sons d'une fin de séance
 */
#define SONS_FIN_SEANCE ":/ressources/sons/finSeance.wav"

/**
 * @def SONS_FIN_MANCHE
 * @brief Définit le sons d'une fin de manche
 */
#define SONS_FIN_MANCHE ":/ressources/sons/finManche.wav"

/**
 * @def SONS_TIR_REUSSI
 * @brief Définit le sons d'un tir réussi
 */
#define SONS_TIR_REUSSI ":/ressources/sons/tirReussi.wav"

/**
 * @def NB_PIONS
 * @brief Définit le nombre de pions max
 */
namespace Ui
{
class basketgame;
}
using namespace std;
class Puissance4;
class Communication;
class Equipe;
/**
 * @class Basketgame
 * @brief La GUI de l'application Basketgame
 */
class Basketgame : public QMainWindow
{
    Q_OBJECT

    /**
     * @enum Ecran
     * @brief Les différents écrans de l'affichage
     */
    enum Ecran
    {
        Accueil,
        Seance,
    };

    /**
     * @enum Etat
     * @brief Les différents états de la gestion du Basketgame
     */
    enum Etat
    {
        Deconnecte,
        Attente,
        Configure,
        Demarre,
        EnCours,
        Sauvegarde,
        FinManche,
        Termine,
        NbEtats
    };
    enum CouleurEquipe
    {
        Rouge = 0,
        Jaune,
        Aucune,
        NbEquipes
    };
  public:
    explicit Basketgame(QWidget* parent = 0);
    ~Basketgame();

  public slots:    
    void demarrerSeance();
    void terminerSeance();
    void evaluerSeance();
    void reinitialiserSeance();
    void gererTir(QString couleurEquipe, int numeroPanier);
    void configurerSeance(QString nomEquipeRouge,
                          QString nomEquipeJaune,
                          int     nbPaniers,
                          int     tempsTour,
                          int     nbManches);
    void demarrerManche(int numeroManche);
    void terminerManche(int numeroManche);
    void evaluerManche(int numeroManche);
    void envoyerVainqueurManche(int numeroManche, bool vainqueur);
    void jouerPion(int colonne);
    void afficherEcran(Basketgame::Ecran ecran);
    void afficherEcranAcceuil();
    void afficherEcranSeance();
    void chronometrerTour();
    void fermerApplication();

#ifdef TEST_BASKETGAME
    void simulerPion();
#endif

  private:
    /**
     * @enum CouleurEquipe
     * @brief Les différentes couleur d'equipe
     */

    Ui::basketgame*  ui;
    Puissance4*      puissance4;
    Communication*   communication;
    QVector<Equipe*> equipes;

    QTime*  tempsTour;
    QTimer* minuteurTour;
    int     nbPionsJoues;
    int     numeroManche;
    int     numeroPanier;
    int     nombreManches;
    int     nombrePaniers;
    int     tempsTourConfigure;
    Etat    etatBasketgame;

    // QSound      *finSeance;
    // QSound      *finManche;
    // QSound      *tirReussi;

    void                 initialiserIHM();
    void                 initialiserEvenements();
    void                 initialiserManche();
    void                 initialiserSeance();
    void                 initialiserDureeTour();
    void                 initialiserParametresEquipe();
    void                 initialiserCommunication();
    void                 demarrerChronometrageTour();
    void                 afficherPuissance4();
    void                 afficherUnJeton(int ligne, int colonne);
    void                 afficherTourEquipe();
    void                 afficherScorePanierEquipe();
    void                 afficherScoreMancheEquipe();
    int                  getNombreManches() const;
    void                 decrementerNbManches();
    void                 supprimerEquipes();
    static CouleurEquipe convertirCouleurRecue(QString couleurEquipe);

#ifdef TEST_BASKETGAME
    void attribuerRaccourcisClavier();
    int  randInt(int min, int max);
#endif
};

#endif // BASKETGAME_H
