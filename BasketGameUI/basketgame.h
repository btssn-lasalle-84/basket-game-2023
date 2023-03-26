#ifndef BASKETGAME_H
#define BASKETGAME_H

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

class Basketgame : public QMainWindow
{
    Q_OBJECT

    /**
     * @enum Ecran
     * @brief Les différentes écran de l'affichage
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
    void afficherPage(Basketgame::Ecran ecran);
    void afficherPageAcceuil();
    void afficherPagePartie();
    void afficherPageManche();
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
