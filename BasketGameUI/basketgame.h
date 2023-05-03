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

#define NB_COLONNES 7
#define NB_LIGNES 6

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
    void incrementerLcdNumberPointsEquipeRouge();
    void incrementerLcdNumberPointsEquipeJaune();
    void simulerPion();
    void initialiserPlateau();

  private slots:
    void demarrerSeance();
    void terminerPartie();
  private:
    enum CouleurEquipe
        {
            Rouge = 0,
            Jaune,
            NbEquipes
        };

    enum CouleurJeton
       {
           ROUGE = -1,
           AUCUN = 0,
           JAUNE = 1,
           NbJetons /* = 2 */
       };

    Ui::basketgame* ui;
    QVector<QVector<CouleurJeton> > plateau;
    QTime*          tempsTours;
    QTimer*         timerTours;

    bool            etatTours;
    bool            etatPartie;
    bool            estVainqueur;
    bool            couleurEquipe;

    int             ToursJoueurs;
    int             nbPionsAlignes;
    int             nombreColonnes;
    int             scoreEquipeRouge;
    int             scoreEquipeJaune;
    int             randInt(int min, int max);
    void            afficherUnJeton(int ligne, int colonne);
    void            initialiserIHM();
    void            initialiserEvenements();

};

#endif // BASKETGAME_H
