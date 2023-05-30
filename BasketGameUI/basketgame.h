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

class Puissance4;
class Communication;
class Equipe;
class Seance;
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
    void demarrerManche();
    void terminerManche();
    void jouerPion(int colonne);
    void afficherEcran(Basketgame::Ecran ecran);
    void afficherEcranAcceuil();
    void afficherEcranPartie();
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
    enum CouleurEquipe
    {
        Rouge = 0,
        Jaune,
        NbEquipes
    };

    Ui::basketgame*  ui;
    Puissance4*      puissance4;
    Communication*   communication;
    QVector<Equipe*> equipes;
    Seance*          seance;

    QTime*           tempsTour;
    QTimer*          minuteurTour;
    int              nbPionsJoues;
    bool             etatManche;
    bool             etatSeance;
    int              nombreManche;

    void initialiserIHM();
    void initialiserEvenements();
    void initialiserManche();
    void initialiserSeance();
    void initialiserDureeTour();
    void initialiserParametreEquipe();
    void initialiserCommunication();
    void demarrerChronometrageTour();
    void afficherPuissance4();
    void afficherUnJeton(int ligne, int colonne);
    void afficherTourEquipe();
    void afficherScorePanierEquipe();
    void afficherScoreMancheEquipe();

#ifdef TEST_BASKETGAME
    void attribuerRaccourcisClavier();
    int  randInt(int min, int max);
#endif
};

#endif // BASKETGAME_H
