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

class Communication : public QObject
{
    Q_OBJECT
  public:
    Communication(QObject* parent = 0);
    ~Communication();

  private:
    QBluetoothServer* serveur; //!< Le serveur Bluetooth
    QBluetoothSocket* socket;  //!< La socket de communication Bluetooth

  public slots:
};

#endif
