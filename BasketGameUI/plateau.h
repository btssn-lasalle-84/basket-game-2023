#ifndef _PLATEAU_H
#define _PLATEAU_H


struct Coup {
    int ligne;

    int colonne;

    int numero;

};
class Plateau : public typedef2 {
  Q_OBJECT
  public:
    explicit Plateau(QWidget * parent = 0);

  public slots:
  private:
    QVector<QVector<int> > plateau;

    void creerPlateau();

    void initialiserPlateau();

    void afficherPlateau();

    void attribuerRaccourcisClavier();

};
#endif
