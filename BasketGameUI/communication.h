#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */
#include <QObject>
#include <QtBluetooth>

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("BasketGame"));

class Communication : public QObject
{
    Q_OBJECT
  public:
    Communication(QObject* parent = 0);
    ~Communication();

    void initialiser();
    void demarrer();
    void arreter();
    void deconnecter();
    bool estValide();
    bool estConnecte();

    QString getNomPeripheriqueLocal();
    QString getAdressePeripheriqueLocal();

  public slots:

  private:
    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer*     serveur;     //!< Le serveur Bluetooth
    QBluetoothSocket*     socket;      //!< La socket de communication Bluetooth
    QBluetoothServiceInfo serviceInfo; //!< Information sur le service Bluetooth
    bool                  connecte; //!< Etat de connexion de la socket client
    QString               nomPeripheriqueLocal;
    QString               adressePeripheriqueLocal;
    QString trame; //!< Le contenu des données reçues sur la socket

  private slots:
    void connecterTablette(const QBluetoothAddress& adresse);
    void deconnecterTablette(const QBluetoothAddress& adresse);
    void connecterSocket();
    void deconnecterSocket();
    void recevoirDonnees();

    void recupererErreurSocket(QBluetoothSocket::SocketError erreurBluetooth);
    void recupererErreurBluetooth(QBluetoothLocalDevice::Error erreurBluetooth);
    void recupererEtatSocket(QBluetoothSocket::SocketState etatBluetooth);

  signals:
    void clientConnecte();
    void clientDeconnecte();
    void tabletteConnectee();
    void tabletteDeconnectee();
    void trameRecue(QString message);
};

#endif
