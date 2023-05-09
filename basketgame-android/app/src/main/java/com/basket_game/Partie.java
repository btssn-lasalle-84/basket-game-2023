/**
 * @file Partie.java
 * @brief Déclaration de la classe Partie
 * @author Guillaumet Florent
 */

package com.basket_game;

import android.util.Log;

/**
 * @class Partie
 * @brief Définit une partie entre deux équipes
 */
public class Partie
{
    /**
     * Constantes
     */
    private static final String TAG = "_Partie"; //!< TAG pour les logs (cf. Logcat)
    // Du système
    public static final int NB_PANIERS_MIN = 2; //!< le nombre min de paniers gérables
    public static final int NB_PANIERS_MAX = 7; //!< le nombre max de paniers gérables
    // Valeurs par défaut pour une Partie
    public static final int NB_MANCHES_GAGNANTES = 1; //!< le nombre de manches gagnantes par défaut
    public static final int TEMPS_MAX_TOUR =
      45; //!< le temps max pour d'un tour pour une équipe en secondes par défaut
    public static final int NB_PANIERS = 4; //!< le nombre de paniers installés par défaut

    /**
     * @brief Attributs
     */
    /**
     * @see Notion de conteneur !?
     */
    private Equipe equipe1            = null;                 //!< l'équipe n°1
    private Equipe equipe2            = null;                 //!< l'équipe n°2
    private int    nbManchesGagnantes = NB_MANCHES_GAGNANTES; //!< le nombre de manches gagnantes
    private int    tempsMaxTour =
      TEMPS_MAX_TOUR; //!< le temps max pour d'un tour pour une équipe en secondes
    private int     nbPaniers = NB_PANIERS; //!< le nombre de paniers installés
    private Boolean estFinie  = false;      //!< l'état de la partie
    // horodatage

    /**
     * @brief Constructeur de la classe Partie
     * @param equipe1
     * @param equipe2
     */
    public Partie(Equipe equipe1, Equipe equipe2)
    {
        Log.d(TAG,
              "Partie(\"" + equipe1.getNomEquipe() + "\", \"" + equipe2.getNomEquipe() + "\")");
        this.equipe1 = equipe1;
        this.equipe2 = equipe2;
    }

    /**
     * @brief Accesseur equipe1
     * @fn getEquipe1
     * @details Retourne l'equipe1
     * @return equipe1
     */
    public Equipe getEquipe1()
    {
        return equipe1;
    }

    /**
     * @brief Mutateur equipe1
     * @fn setEquipe1
     * @details Modifie l'equipe1
     * @param equipe1
     */
    public void setEquipe1(Equipe equipe1)
    {
        this.equipe1 = equipe1;
    }

    /**
     * @brief Accesseur equipe2
     * @fn getEquipe2
     * @details Retourne l'equipe2
     * @return equipe2
     */
    public Equipe getEquipe2()
    {
        return equipe2;
    }

    /**
     * @brief Mutateur equipe2
     * @fn setEquipe2
     * @details Modifie l'equipe2
     * @param equipe2
     */
    public void setEquipe2(Equipe equipe2)
    {
        this.equipe2 = equipe2;
    }

    /**
     * @brief Accesseur nbManchesGagnantes
     * @fn getNbManchesGagnantes
     * @details Retourne le nombre de manches gagnantes
     * @return nbManchesGagnantes
     */
    public int getNbManchesGagnantes()
    {
        return nbManchesGagnantes;
    }

    /**
     * @brief Mutateur nbManchesGagnantes
     * @fn setNbManchesGagnantes
     * @details Modifie le nombre de manches gagnantes
     * @param nbManchesGagnantes
     */
    public void setNbManchesGagnantes(int nbManchesGagnantes)
    {
        this.nbManchesGagnantes = nbManchesGagnantes;
    }

    /**
     * @brief Accesseur tempsMaxTour
     * @fn getTempsMaxTour
     * @details Retourne le temps maximum par tour
     * @return tempsMaxTour
     */
    public int getTempsMaxTour()
    {
        return tempsMaxTour;
    }

    /**
     * @brief Mutateur tempsMaxTour
     * @fn setTempsMaxTour
     * @details Modifie le temps maximum par tour
     * @param tempsMaxTour
     */
    public void setTempsMaxTour(int tempsMaxTour)
    {
        this.tempsMaxTour = tempsMaxTour;
    }

    /**
     * @brief Accesseur nbPaniers
     * @fn getNbPaniers
     * @details Retourne le nombre de paniers
     * @return nbPaniers
     */
    public int getNbPaniers()
    {
        return nbPaniers;
    }

    /**
     * @brief Mutateur nbPaniers
     * @fn setNbPaniers
     * @details Modifie le nombre de paniers
     * @param nbPaniers
     */
    public void setNbPaniers(int nbPaniers)
    {
        this.nbPaniers = nbPaniers;
    }
}
