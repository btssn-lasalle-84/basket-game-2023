#include "communication.h"


#include <QDebug>


Communication::Communication(QWidget *parent) : QWidget(parent) ,
    socket(nullptr) , discoveryAgent(nullptr) , estPeripheriqueTrouver(false)
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

/**
 * @brief Méthode de recherche des périphériques bluetooth
 *
 * @fn Communication::rechercher
 *
 */
void Communication::rechercher()
{
    if(discoveryAgent == nullptr)
    {
        return;
    }

    qDebug() << Q_FUNC_INFO;

    // démarre la recherche de périphériques bluetooth
    estPeripheriqueTrouver = false;
    discoveryAgent->start();
}

/**
 * @brief Méthode pour rechercher un périphérique distant
 *
 * @fn Communication::rechercherPeripherique
 *
 */
void Communication::rechercherPeripherique(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
}


/**
 * @brief Méthode pour se connecter au périphérique
 *
 * @fn Communication::connecter
 *
 */
void Communication::connecter(const QBluetoothDeviceInfo device)
{
     socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
     connect(socket, SIGNAL(connected()), this, SLOT(peripheriqueConnecter()));
     connect(socket, SIGNAL(disconnected()), this, SLOT(peripheriqueDeconnecter()));
     connect(socket, SIGNAL(readyRead()), this, SLOT(recupererTrame()));

     QBluetoothAddress adresse = QBluetoothAddress(device.address());
     QBluetoothUuid uuid = QBluetoothUuid(QBluetoothUuid::SerialPort);
     socket->connectToService(adresse, uuid);
     socket->open(QIODevice::ReadWrite);
}

/**
 * @brief  Méthode pour se déconnecter au périphérique
 *
 * @fn Communication::deconnecter
 *
 */
void Communication::deconnecter()
{
    if (socket->isOpen())
    {
        socket->close();
    }
}

/**
 * @brief Méthode pour retourner l'état de connexion
 *
 * @fn Communication::estConnecte
 *
 */
bool Communication::estConnecter()
{
    qDebug() << Q_FUNC_INFO << socket->isOpen();
    return socket->isOpen();
}

