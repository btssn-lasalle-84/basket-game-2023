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
        Manche,
        NbEcrans
    };

  public:
    explicit Basketgame(QWidget* parent = 0);
    ~Basketgame();

    void arreterPartie();
    void arreterManche();

  public slots:
    void afficherEcran(Basketgame::Ecran ecran);
    void afficherEcranAcceuil();
    void afficherEcranPartie();
    void afficherEcranManche();
    void chronometrerPartie();
    void chronometrerManche();

  private slots:
    void demarrerManche();
    void demarrerPartie();

  private:
    Ui::basketgame* ui;
    QTime*          tempsPartie;
    QTime*          tempsManche;
    QTimer*         timerPartie;
    QTimer*         timerManche;
    bool            etatPartie;
    bool            etatManche;
    bool            couleurEquipe;

    void initialiserIHM();
    void initialiserEvenements();
};

#endif // BASKETGAME_H
