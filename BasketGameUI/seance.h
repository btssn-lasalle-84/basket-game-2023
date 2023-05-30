#ifndef SEANCE_H
#define SEANCE_H

#include <QObject>

class Seance : public QObject
{
    Q_OBJECT
public:
    explicit Seance(QObject *parent = 0);
    ~Seance();

    int     getNombreManches() const ;
    void    setNombreManches(int nbManches);
    void    decrementerNbManche();

private:
        int          nbManches;
        bool         etatSeance;
};

#endif // SEANCE_H
