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
/**
 * @fn Seance::setNombreManche()
 * @brief Modifie le nombre de manche
 */
void Seance::setNombreManches(int nbManches)
{
    this->nbManches = nbManches;
}

/**
 * @fn Seance::getNombreManche()
 * @brief Retourne le nombre de manche
 */
int Seance::getNombreManches() const
{
    return nbManches;
}


/**
 * @fn Seance::decrementerNbManche
 * @brief méthode qui décremente le nombre de manche joué
 */
void Seance::decrementerNbManches()
{
    nbManches--;
    qDebug() << Q_FUNC_INFO << nbManches ;
}

