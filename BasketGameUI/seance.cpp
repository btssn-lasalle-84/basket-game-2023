#include "seance.h"
#include <QDebug>
Seance::Seance(QObject *parent) : QObject(parent) ,
  nbManche(0)
{

}

Seance::~Seance()
{
    qDebug() << Q_FUNC_INFO;

}

void Seance::setNombreManche(int nbManche)
{
    this->nbManche = nbManche;
}

int Seance::getNombreManche() const
{
    return nbManche;
}

void Seance::decrementerNbManche()
{
    nbManche--;
    qDebug() << Q_FUNC_INFO << nbManche ;
}

