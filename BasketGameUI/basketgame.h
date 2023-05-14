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

/**
 * @def PLEIN_ECRAN_RPI
 * @brief Pour le mode plein écran sur la Raspberry Pi
 */
//#define PLEIN_ECRAN_RPI

/**
 * @def TEST_BASKETGAME
 * @brief Pour les tests avec le clavier
 */
#define TEST_BASKETGAME

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
 * @def NB_COLONNES
 * @brief Définit le nombre de colonne par défaut
 */
#define NB_COLONNES 7

/**
 * @def NB_LIGNES
 * @brief Définit le nombre de ligne par défaut
 */
#define NB_LIGNES 6

/**
 * @def NB_PIONS
 * @brief Définit le nombre de pions max
 */
#define NB_PIONS (NB_LIGNES * NB_COLONNES)

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
 * @def DEPLACEMENT_Y
 * @brief Définit le nombre de pions pour gagner une partie
 */
#define NB_PIONS_ALIGNES 4

/**
 * @def JETON_ROUGE
 * @brief Définit l'image d'un jeton rouge
 */
#define JETON_ROUGE ":/ressources/jetonRouge.png"

/**
 * @def JETON_JAUNE
 * @brief Définit l'image d'un jeton jaune
 */
#define JETON_JAUNE ":/ressources/jetonJaune.png"

/**
 * @def PLATEAU_7
 * @brief Définit l'image d'un plateau à 7 colonnes
 */
#define PLATEAU_7 ":/ressources/puissance4_7.png"

namespace Ui
{
class basketgame;
}

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
        Partie,
    };

  public:
    explicit Basketgame(QWidget* parent = 0);
    ~Basketgame();

  public slots:
    void demarrerSeance();
    void terminerSeance();
    void demarrerPartie();
    void afficherEcran(Basketgame::Ecran ecran);
    void afficherEcranAcceuil();
    void afficherEcranPartie();
    void chronometrerTour();
#ifdef TEST_BASKETGAME
    void simulerPion();
#endif
    void fermerApplication();

  private:
    /**
     * @enum CouleurEquipe
     * @brief Les différentes couleur d'equipe
     */
    enum CouleurEquipe
    {
        Rouge = 0,
        Jaune,
        NbEquipes
    };

    /**
     * @enum CouleurJeton
     * @brief Les différents couleur de jeton
     */
    enum CouleurJeton
    {
        AUCUNE = 0,
        ROUGE,
        JAUNE,
        NB_COULEURS
    };

    Ui::basketgame*                 ui;
    QVector<QVector<CouleurJeton> > plateau;
    QTime*                          tempsTour;
    QTimer*                         timerTour;
    bool                            etatSeance;
    bool                            estVainqueur;
    bool                            estEquipeRouge;
    int                             nbPionsJoues;
    int                             nbPionsAlignes;
    int                             scoreEquipeRouge;
    int                             scoreEquipeJaune;

    void initialiserIHM();
    void initialiserEvenements();
    void initialiserEtatPartie();
    void initialiserPlateau();
    void initialiserDureeTour();
    void demarrerChronometrageTour();
    void afficherPlateau();
    void placerPion(int colonne);
    void afficherUnJeton(int ligne, int colonne);
    void afficherTourEquipe();
    void afficherScoreEquipe();

#ifdef TEST_BASKETGAME
    void attribuerRaccourcisClavier();
    int  randInt(int min, int max);
#endif
};

#endif // BASKETGAME_H
