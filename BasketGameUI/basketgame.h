#ifndef BASKETGAME_H
#define BASKETGAME_H

#include <QtWidgets>

/**
 * @def TIC_HORLOGE
 * @brief Définit le tic d'horloge à 1s
 */
#define TIC_HORLOGE 1000

namespace Ui
{
class basketgame;
}

class Basketgame : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Basketgame(QWidget* parent = 0);
    ~Basketgame();

    void arreterPartie();
    void arreterManche();

  public slots:
    void chronometrerPartie();
    void chronometrerManche();

  private slots:
    void demarrerManche();
    void demarrerPartie();

  private:
    Ui::basketgame* ui;
    QTime*          tempsPartie   = new QTime;
    QTime*          tempsManche   = new QTime;
    QTimer*         timerPartie   = new QTimer;
    QTimer*         timerManche   = new QTimer;
    bool            etatPartie    = true;
    bool            etatManche    = true;
    bool            couleurEquipe = true;

    void initialiserIHM();
    void initialiserEvenements();
};

#endif // BASKETGAME_H
