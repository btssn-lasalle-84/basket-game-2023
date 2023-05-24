/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#include "communication.h"

Communication::Communication(QObject* parent) :
    QObject(parent), serveur(nullptr), socket(nullptr), connecte(false)
{
    qDebug() << Q_FUNC_INFO;
    initialiser();
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
    if(serveur == nullptr)
    {
        qDebug() << Q_FUNC_INFO;
        serveur =
          new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouveauClient()));

        QBluetoothUuid uuid(QBluetoothUuid::Rfcomm);
        serviceInfo = serveur->listen(uuid, serviceNom);
        // qDebug() << "serviceInfo" << serviceInfo;
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

    if(serveur == nullptr)
        return;

    serviceInfo.unregisterService();

    if(socket != nullptr)
    {
        if(socket->isOpen())
            socket->close();
        delete socket;
        socket = nullptr;
    }

    delete serveur;
    serveur = nullptr;
}

/**
 * @brief Initialise la liaison Bluetooth en mode serveur
 *
 * @fn Communication::initialiser
 */
void Communication::initialiser()
{
    if(!peripheriqueLocal.isValid())
    {
        qDebug() << Q_FUNC_INFO << "isValid" << peripheriqueLocal.isValid();
        return;
    }

    qDebug() << Q_FUNC_INFO << "hostMode" << peripheriqueLocal.hostMode();

    peripheriqueLocal.powerOn();
    nomPeripheriqueLocal     = peripheriqueLocal.name();
    adressePeripheriqueLocal = peripheriqueLocal.address().toString();
    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);
    /**
     * @see les appareil qui ne sont pas appairés peuvent decouvrir la Raspberry
     * Pi
     */
    // peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    qDebug() << Q_FUNC_INFO << "nomPeripheriqueLocal" << nomPeripheriqueLocal
             << "adressePeripheriqueLocal" << adressePeripheriqueLocal
             << "hostMode" << peripheriqueLocal.hostMode();

    connect(&peripheriqueLocal,
            SIGNAL(deviceConnected(QBluetoothAddress)),
            this,
            SLOT(connecterTablette(QBluetoothAddress)));
    connect(&peripheriqueLocal,
            SIGNAL(deviceDisconnected(QBluetoothAddress)),
            this,
            SLOT(deconnecterTablette(QBluetoothAddress)));
    connect(&peripheriqueLocal,
            SIGNAL(error(QBluetoothLocalDevice::Error)),
            this,
            SLOT(recupererErreurBluetooth(QBluetoothLocalDevice::Error)));
}

/**
 * @brief Récupère et affiche l'erreur socket dans la console de débuguage
 *
 * @fn Communication::recupererErreurSocket
 */
void Communication::recupererErreurSocket(
  QBluetoothSocket::SocketError erreurSocket)
{
    qDebug() << Q_FUNC_INFO << erreurSocket;
}

/**
 * @brief Récupère et affiche l'etat de la socket dans la console de débuguage
 *
 * @fn Communication::recupererEtatSocket
 */
void Communication::recupererEtatSocket(
  QBluetoothSocket::SocketState etatSocket)
{
    qDebug() << Q_FUNC_INFO << etatSocket;
}

/**
 * @brief Récupère et affiche l'erreur Bluetooth dans la console de débuguage
 *
 * @fn Communication::recupererErreurBluetooth
 */
void Communication::recupererErreurBluetooth(
  QBluetoothLocalDevice::Error erreurBluetooth)
{
    qDebug() << Q_FUNC_INFO << erreurBluetooth;
}

/**
 * @brief Connecte la tablette
 *
 * @fn Communication::connecterTablette
 * @param adresse adresse MAC Bluetooth de la tablette
 */
void Communication::connecterTablette(const QBluetoothAddress& adresse)
{
    QString etatAppairage;
    if(peripheriqueLocal.pairingStatus(adresse) ==
         QBluetoothLocalDevice::Paired ||
       peripheriqueLocal.pairingStatus(adresse) ==
         QBluetoothLocalDevice::AuthorizedPaired)
        etatAppairage = "appairé";
    else
        etatAppairage = "non appairé";
    qDebug() << Q_FUNC_INFO << "adresse" << adresse << "pairingStatus"
             << peripheriqueLocal.pairingStatus(adresse) << etatAppairage;
    emit tabletteConnectee();
    /**
     * @todo Si on a le temps, on devrait pouvoir gérer les
     * connexions/déconnexions pendant une partie
     */
}

/**
 * @brief Déconnecte la tablette
 *
 * @fn Communication::deconnecterTablette
 * @param adresse adresse MAC Bluetooth de la tablette
 */
void Communication::deconnecterTablette(const QBluetoothAddress& adresse)
{
    qDebug() << Q_FUNC_INFO << "adresse" << adresse;
    emit tabletteDeconnectee();
    /**
     * @todo Si on a le temps on devrait pouvoir gérer les
     * connexions/déconnexions pendant une partie
     */
}

void Communication::connecterSocket()
{
    socket = serveur->nextPendingConnection();
    if(socket == nullptr)
        return;

    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecterSocket()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(recevoirDonnees()));
    connect(socket,
            SIGNAL(error(QBluetoothSocket::SocketError)),
            this,
            SLOT(recupererErreurSocket(QBluetoothSocket::SocketError)));
    connect(socket,
            SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
            this,
            SLOT(recupererEtatSocket(QBluetoothSocket::SocketState)));

    connecte = true;
    qDebug() << Q_FUNC_INFO << "client" << socket->peerName()
             << socket->peerAddress().toString();
    emit clientConnecte();
}

void Communication::deconnecterSocket()
{
    qDebug() << Q_FUNC_INFO;
    connecte = false;
    emit clientDeconnecte();
}

void Communication::recevoirDonnees()
{
    qDebug() << Q_FUNC_INFO;
    QByteArray donnees;

    donnees = socket->readAll();
    // qDebug() << Q_FUNC_INFO << "donnees" << donnees;

    trame += QString(donnees.data());
    qDebug() << Q_FUNC_INFO << "trame" << trame;

    /**
     * @todo Vérifier au moins que la trame est valide ? elle doit commencer par
     * le délimiteur de début et se terminer par le délimiteur de fin comme
     * défini dans le protocole. Remarque : on peut supprimer supprimer ces
     * délimiteurs une fois vérifiés.
     */
    /**
     * @todo Si la trame est valide alors : (pour commencer, on supposera que
     * l'on a reçu qu'une trame complète) extraire le type de trame et, en
     * fonction du type de trame, émettre un signal avec les données de la
     * trame. Les signaux devront être connectés à des slots de la classe
     * Basketgame. On n'oublie pas alors d'effacer le contenu de la trame pour
     * la prochaine réception.
     */
}

QString Communication::getNomPeripheriqueLocal()
{
    return nomPeripheriqueLocal;
}

QString Communication::getAdressePeripheriqueLocal()
{
    return adressePeripheriqueLocal;
}

bool Communication::estValide()
{
    return peripheriqueLocal.isValid();
}

bool Communication::estConnecte()
{
    return connecte;
}
