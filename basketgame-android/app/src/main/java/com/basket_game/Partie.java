/**
 * @file Partie.java
 * @brief Déclaration de la classe Partie
 * @author Guillaumet Florent
 */

package com.basket_game;

/**
 * @class Partie
 * @brief Définit une partie entre deux équipes
 */
public class Partie
{
    /**
     * @brief Attributs
     */
    private Equipe  equipe1            = null;  //!< l'équipe n°1
    private Equipe  equipe2            = null;  //!< l'équipe n°2
    private int     nbManchesGagnantes = 1;     //!<
    private int     tempsMaxTir        = 0;     //!<
    private int     nbPaniers          = 4;     //!<
    private Boolean estFinie           = false; //!<
    // horodatage

    /**
     * @brief Constructeur de la classe Partie
     * @param equipe1
     * @param equipe2
     */
    public Partie(Equipe equipe1, Equipe equipe2)
    {
        this.equipe1 = equipe1;
        this.equipe2 = equipe2;
    }

    /**
     *
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
     *
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
     *
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
     *
     * @brief Mutateur equipe2
     * @fn setEquipe2
     * @details Modifie l'equipe2
     * @param equipe2
     */
    public void setEquipe2(Equipe equipe2)
    {
        this.equipe2 = equipe2;
    }
}
