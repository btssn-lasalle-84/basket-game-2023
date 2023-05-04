#ifndef BASKETGAME_H
#define BASKETGAME_H

/**
 * @file basketgame.h
 *
 * @brief Déclaration de la classe Basketgame
 * @author Nathanael CHANSARD
 * @version 0.1
 */
#include <QtWidgets>

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
#define TEMPS_TOUR 15 // en s

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
    void afficherEcran(Basketgame::Ecran ecran);
    void afficherEcranAcceuil();
    void afficherEcranPartie();
    void afficherPlateau();
    void chronometrerTours();
    void incrementerVisualisationLcdNumber();
    void simulerPion();
    void initialiserPlateau();
    void fermerApplication();

  private slots:
    void demarrerSeance();
    void terminerSeance();

  private:

    /**
     * @enum CouleurEquipe
     * @brief Les différentes couleur d'equipe
     */
    enum CouleurEquipe
        {
            Rouge = 0,
            Jaune,
        };

    /**
     * @enum CouleurJeton
     * @brief Les différents couleur de jeton
     */
    enum CouleurJeton
       {
           COULEUR = 0,
           AUCUN,
       };

    Ui::basketgame* ui;
    QVector<QVector<CouleurJeton> > plateau;
    QTime*          tempsTours;
    QTimer*         timerTours;

    bool            etatTours;
    bool            estVainqueur;
    bool            estEquipeRouge;

    int             ToursJoueurs;
    int             nbPionsAlignes;
    int             scoreEquipeRouge;
    int             scoreEquipeJaune;
    int             randInt(int min, int max);
    void            afficherUnJeton(int ligne, int colonne);
    void            afficherToursEquipe();
    void            initialiserIHM();
    void            initialiserEvenements();

};

#endif // BASKETGAME_H
