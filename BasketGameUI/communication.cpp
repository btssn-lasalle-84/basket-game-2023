/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Nathanael CHANSARD
 * @version 0.2
 */

#include "communication.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe Communication
 *
 * @fn Communication::Communication
 *
 */
Communication::Communication(QWidget* parent) :
    QWidget(parent), socket(nullptr), serveur(nullptr)
{
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Destructeur de la classe Communication
 *
 * @fn Communication::~Communication
 *
 */
Communication::~Communication()
{
    qDebug() << Q_FUNC_INFO;
}
