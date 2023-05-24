/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#include "communication.h"
#include <unistd.h>"

Communication::Communication(QObject* parent) :
    QObject(parent), serveur(nullptr), socket(nullptr),
    estConnecter(false)
{
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Destructeur de la classe Communication
 *
 * @fn Communication::~Communication
 * @details Libère les ressources de l'application
 */
Communication::~Communication()
{
    arreter();
    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Démarre la communication Bluetooth en mode serveur
 *
 * @fn Communication::demarrer
 */
void Communication::demarrer()
{
    if (serveur == NULL)
    {
        qDebug() << Q_FUNC_INFO;
        serveur = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouveauClient()));

        QBluetoothUuid uuid(QBluetoothUuid::Rfcomm);
        serviceInfo = serveur->listen(uuid, serviceNom);
        //qDebug() << serviceInfo;
    }
}

/**
 * @brief Arrête la communication Bluetooth en mode serveur
 *
 * @fn Communication::arreter
 */
void Communication::arreter()
{
    qDebug() << Q_FUNC_INFO;

    if (!serveur)
        return;

    serviceInfo.unregisterService();

    if (socket)
    {
        if (socket->isOpen())
           socket->close();
        delete socket;
        socket = NULL;
    }

    delete serveur;
    serveur = NULL;
}

/**
 * @brief Initialise la liaison Bluetooth en mode serveur
 *
 * @fn Communication::initialiser
 */
void Communication::initialiser()
{
    qDebug() << Q_FUNC_INFO;
    if (!peripheriqueLocal.isValid())
    {
        qDebug() << Q_FUNC_INFO << peripheriqueLocal.isValid();
        return;
    }

    qDebug() << peripheriqueLocal.hostMode();

    peripheriqueLocal.powerOn();

    qDebug() << peripheriqueLocal.hostMode();

    nomPeripheriqueLocal = peripheriqueLocal.name();
    adressePeripheriqueLocal = peripheriqueLocal.address().toString();

    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);
    connect(&peripheriqueLocal, SIGNAL(deviceConnected(QBluetoothAddress)), this, SLOT(connecterTablette(QBluetoothAddress)));
    connect(&peripheriqueLocal, SIGNAL(deviceDisconnected(QBluetoothAddress)), this, SLOT(deconnecterTablette(QBluetoothAddress)));
    connect(&peripheriqueLocal, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(recupererErreurBluetooth(QBluetoothLocalDevice::Error)));
}

void Communication::recupererErreurSocket(QBluetoothSocket::SocketError erreurSocket)
{
    qDebug() << Q_FUNC_INFO << erreurSocket;
}

void Communication::recupererEtatSocket(QBluetoothSocket::SocketState etatSocket)
{
    qDebug() << Q_FUNC_INFO << etatSocket;
}

void Communication::recupererErreurBluetooth(QBluetoothLocalDevice::Error erreurBluetooth)
{
    qDebug() << Q_FUNC_INFO << erreurBluetooth;
}

void Communication::connecterTablette(const QBluetoothAddress &adresse)
{
    qDebug() << Q_FUNC_INFO << adresse << peripheriqueLocal.pairingStatus(adresse);
    QString message = QString::fromUtf8("Demande connexion du client ") + adresse.toString();
    if (peripheriqueLocal.pairingStatus(adresse) == QBluetoothLocalDevice::Paired || peripheriqueLocal.pairingStatus(adresse) == QBluetoothLocalDevice::AuthorizedPaired)
        message += " [" + QString::fromUtf8("appairé") + "]";
    else
        message += " [" + QString::fromUtf8("non appairé") + "]" ;
    emit afficherMessage(message);
}

void Communication::deconnecterTablette(const QBluetoothAddress &adresse)
{
    qDebug() << Q_FUNC_INFO << adresse;
    QString message = QString::fromUtf8("Déconnexion du client ") + adresse.toString();
    emit afficherMessage(message);
}



void Communication::connecterSocket()
{
    qDebug() << Q_FUNC_INFO;
    socket = serveur->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecterSocket()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(recupererDonneesSocket()));
    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(recupererErreurSocket(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)), this, SLOT(recupererEtatSocket(QBluetoothSocket::SocketState)));

    estConnecter = true;
    emit clientConnecter();
    QString message = QString::fromUtf8("Périphérique ") + socket->peerName() + " [" + socket->peerAddress().toString() + "] " + QString::fromUtf8("connecté ");
    emit afficherMessage(message);
    //envoyer("OK");
}

void Communication::deconnecterSocket()
{
    qDebug() << Q_FUNC_INFO;
    estConnecter = false;
    emit clientDeconnecter();
}

void Communication::recupererDonneesSocket()
{
    qDebug() << Q_FUNC_INFO;
    QByteArray donnees;

    while (socket->bytesAvailable())
    {
        donnees += socket->readAll();
        usleep(150000); // cf. timeout
    }
    emit afficherMessage(QString::fromUtf8("Données reçues : ") + QString(donnees));
}

QString Communication::getNomPeripheriqueLocal()
{
    return nomPeripheriqueLocal;
}

QString Communication::getAdressePeripheriqueLocal()
{
    return adressePeripheriqueLocal;
}

