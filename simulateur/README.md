# Simulateur B4SKET-GAME 2023

## Présentation du protocole implanté dans le simulateur ESP'ACE

Ce document présente rapidement le fonctionnement du simulateur ainsi que le protocole implémenté. Le protocole complet est disponible dans Google Drive. Actuellement, la version du protocole est la **0.1**.

## Configuration du simulateur

Valeur par défaut :

```cpp
#define NB_PANIERS    7
#define NB_MANCHES    10
#define MAX_TEMPS_TIR 45
```

```cpp
#define GPIO_SW1         12   //!< Pour simuler un tir
#define GPIO_SW2         14   //!< Pour simuler joueur suivant
```

## Fonctionnement

- Trame de début de séance :

```
$BASKET;SEANCE;NOM_EQUIPE1;NOM_EQUIPE2;TEMPS;PANIERS;MANCHES;\r\n
$BASKET;SEANCE;Rouge;Jaune;45;7;5;\r\n
```

- Trame de début de partie :

```
$BASKET;START;NUMERO_PARTIE;\r\n
$BASKET;START;1;\r\n
```

- Trames de détection :

```
$BASKET;TIR;COULEUR;NUMERO_PANIER;\r\n
$BASKET;TIR;ROUGE;3;\r\n
```

- Trame de fin de partie :

```
$BASKET;STOP;NUMERO_PARTIE;\r\n
$BASKET;STOP;1;\r\n
```

- Trame de réinitialisation :

```
$BASKET;RESET;\r\n
```

## platform.ini

```ini
[env:lolin32]
platform = espressif32
board = lolin32
framework = arduino
lib_deps =
  thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays @ ^4.2.0
upload_port = /dev/ttyUSB0
upload_speed = 115200
monitor_port = /dev/ttyUSB0
monitor_speed = 115200
```

## Auteur

- Thierry Vaira <<tvaira@free.fr>>
