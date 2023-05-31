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
#define JETON_ROUGE ":/ressources/images/jetonRouge.png"

/**
 * @def JETON_JAUNE
 * @brief Définit l'image d'un jeton jaune
 */
#define JETON_JAUNE ":/ressources/images/jetonJaune.png"

/**
 * @def PLATEAU_7
 * @brief Définit l'image d'un plateau à 7 colonnes
 */
#define PLATEAU_7 ":/ressources/images/puissance4_7.png"

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
        Partie,
    };

    /**
     * @enum Etat
     * @brief Les différents états de la gestion du Basketgame
     */
    enum Etat
    {
        Attente,
        Configure,
        EnCours,
        Stop,
        Termine,
        NbEtats
    };

  public:
    explicit Basketgame(QWidget* parent = 0);
    ~Basketgame();
    int     getNombreManches() const ;

  public slots:
    void demarrerSeance();
    void terminerSeance();
    void reinitialiseSeance();
    void decrementerNbManches();
    void configurerSeance(QString nomEquipeRouge,QString nomEquipeJaune,
                         int nbPaniers,int tempsTour,int nbManches);
    void demarrerManche(int numeroManche);
    void terminerManche(int numeroManche);
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

    QTime*           tempsTour;
    QTimer*          minuteurTour;
    int              nbPionsJoues;
    int              nombreManches;
    int              nombrePaniers;
    int              tempsRecuperer;
    Etat             etatBasketgame;

    void initialiserIHM();
    void initialiserEvenements();
    void initialiserManche();
    void initialiserSeance();
    void initialiserDureeTour();
    void initialiserParametresEquipe();
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
