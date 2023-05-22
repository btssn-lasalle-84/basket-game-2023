#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QWidget>
#include <QString>
#include <QtBluetooth>

/**
 * @def TYPE_TRAME
 * @brief Définition de l'entête de trame
 */
#define ENTETE_TRAME "$basket"

/**
 * @def DELIMITEUR_FIN
 * @brief Délimiteur de fin de trame
 */
#define DELIMITEUR_FIN "\r\n"

/**
 * @def DELIMITEUR_CHAMP
 * @brief Délimiteur de champs
 */
#define DELIMITEUR_CHAMP ";"


static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("ModuleEcran"));

class Communication : public QWidget
{
    Q_OBJECT
public:
    explicit Communication(QWidget *parent = 0);
    ~Communication();


    void deconnecter();
    void connecter(const QBluetoothDeviceInfo device);
    void rechercher();
    bool estConnecter();

public slots:
    void rechercherPeripherique(const QBluetoothDeviceInfo &device);

private :

QBluetoothSocket                *socket;
QBluetoothDeviceDiscoveryAgent  *discoveryAgent;
QBluetoothLocalDevice           peripheriqueLocal;
bool                            estPeripheriqueTrouver;

signals:

void peripheriqueConnecter();
void peripheriqueDeconnecter();
void arreterRecherche();
};

#endif // COMMUNICATION_H
