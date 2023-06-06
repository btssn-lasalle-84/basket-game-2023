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
        connect(serveur,
                SIGNAL(newConnection()),
                this,
                SLOT(connecterSocket()));

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
    peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    /**
     * @see les appareil qui ne sont pas appairés peuvent decouvrir la Raspberry
     * Pi
     */
    // peripheriqueLocal.setHostMode(QBluetoothLocalDevice::HostConnectable);
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

/**
 * @brief Connecte la Socket
 *
 * @fn Communication::connecterSocket
 */
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

/**
 * @brief Deconnecte la Socket
 *
 * @fn Communication::deconnecterSocket()
 */
void Communication::deconnecterSocket()
{
    qDebug() << Q_FUNC_INFO;
    connecte = false;
    emit clientDeconnecte();
}

/**
 * @brief Récuperer les données de la trame
 *
 * @fn Communication::recevoirDonnees()
 */
void Communication::recevoirDonnees()
{
    qDebug() << Q_FUNC_INFO;
    QByteArray donnees;

    donnees = socket->readAll();
    // qDebug() << Q_FUNC_INFO << "donnees" << donnees;

    trame += QString(donnees.data());
    qDebug() << Q_FUNC_INFO << "trame" << trame;

    if(trame.contains(ENTETE_DEBUT) && trame.endsWith(ENTETE_FIN))
    {
        QStringList champsTrame;
        champsTrame = trame.split(DELIMITEUR_CHAMP);
        traiterTrame(champsTrame);
        trame.clear();
    }
}

/**
 * @brief Recupère le type de trame
 *
 * @fn Communication::recupererTypeTrame()
 */
Communication::TypeTrame Communication::recupererTypeTrame(QString champType)
{
    QVector<QString> typesTrame = { "SEANCE", "START", "TIR", "STOP", "RESET" };
    for(int i = 0; i < typesTrame.size(); i++)
    {
        if(typesTrame[i] == champType)
            return TypeTrame(i);
    }
    return TypeTrame::Inconnu;
}

/**
 * @brief Traiter les données d'une trame
 *
 * @fn Communication::traiterTrame()
 */
void Communication::traiterTrame(const QStringList& champsTrame)
{
    qDebug() << Q_FUNC_INFO << "champsTrame" << champsTrame;

    switch(recupererTypeTrame(champsTrame[TYPE_TRAME]))
    {
        case TypeTrame::Seance:
            qDebug() << Q_FUNC_INFO << "SEANCE";
            emit seanceConfiguree(champsTrame[NOM_EQUIPE1],
                                  champsTrame[NOM_EQUIPE2],
                                  champsTrame[NB_PANIERS].toInt(),
                                  champsTrame[TEMPS_TOUR_MAX].toInt(),
                                  champsTrame[NB_MANCHES].toInt());

            break;
        case TypeTrame::Start:
            qDebug() << Q_FUNC_INFO << "START";
            emit mancheDemarree(champsTrame[NUMERO_Seance].toInt());
            break;
        case TypeTrame::Tir:
            qDebug() << Q_FUNC_INFO << "TIR";
            emit tirPanier(champsTrame[COULEUR_EQUIPE],
                           champsTrame[NUMERO_PANIER].toInt());
            break;
        case TypeTrame::Stop:
            qDebug() << Q_FUNC_INFO << "STOP";
            emit mancheArretee(champsTrame[NUMERO_Seance].toInt());
            break;
        case TypeTrame::Reset:
            qDebug() << Q_FUNC_INFO << "RESET";
            emit SeanceReinitialisee();
            break;
        default:
            qDebug() << Q_FUNC_INFO << "type trame inconnu !"
                     << champsTrame[TYPE_TRAME];
            break;
    }
}

/**
 * @fn Communication::getNomPeripheriqueLocal()
 * @brief Retourne le nom du peripherique local
 */
QString Communication::getNomPeripheriqueLocal()
{
    return nomPeripheriqueLocal;
}

/**
 * @fn Communication::getAdressePeripheriqueLocal()
 * @brief Retourne l'adresse du peripherique local
 */
QString Communication::getAdressePeripheriqueLocal()
{
    return adressePeripheriqueLocal;
}

/**
 * @fn Communication::estValide()
 * @brief Retourne si le peripherique local est valide
 */
bool Communication::estValide()
{
    return peripheriqueLocal.isValid();
}

/**
 * @fn Communication::estConnecte()
 * @brief Retourne si le peripherique est connecté
 */
bool Communication::estConnecte()
{
    return connecte;
}
void Communication::envoyer(QString trame)
{
    if (socket == NULL || !socket->isOpen())
    {
        return;
    }
    trame += "\r\n";
    socket->write(trame.toLatin1());

}
