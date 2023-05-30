#ifndef SEANCE_H
#define SEANCE_H

#include <QObject>

class Seance : public QObject
{
    Q_OBJECT
public:
    explicit Seance(QObject *parent = 0);
    ~Seance();

    int     getNombreManche() const ;
    void    setNombreManche(int nbManche);
    void    decrementerNbManche();

private:
        int          nbManche;
};

#endif // SEANCE_H
