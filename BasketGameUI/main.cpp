#include "basketgame.h"
#include <QApplication>

/**
 * @file main.cpp
 * @brief Programme principal
 * @details Crée et affiche la fenêtre principale de l'application Basketgame
 * @author Nathanael CHANSARD
 * @version 1.0
 *
 * @param argc
 * @param argv[]
 * @return int
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Basketgame   basketgame;

    basketgame.showFullScreen();

    return a.exec();
}
