/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#include "communication.h"

Communication::Communication(QObject* parent) :
    QObject(parent), serveur(nullptr), socket(nullptr)
{
    qDebug() << Q_FUNC_INFO;
}

Communication::~Communication()
{
    qDebug() << Q_FUNC_INFO;
}
