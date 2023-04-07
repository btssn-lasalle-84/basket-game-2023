package com.basket_game;

/**
 * @file Equipe.java
 * @brief Déclaration de la classe Equipe
 * @author Guillaumet Florent
 */

/**
 * @class Equipe
 */
public class Equipe
{
    /**
     * @brief Attributs
     */
    private String equipe1;
    private String equipe2;

    /**
     * @brief Constructeur de la classe Equipe
     * @param equipe1
     * @param equipe2
     */
    public Equipe(String equipe1, String equipe2)
    {
        this.equipe1 = equipe1;
        this.equipe2 = equipe2;
    }

    /**
     *
     * @brief Accesseur Equipe1
     * @fn getEquipe1
     * @details Retourne le nom de l'Equipe1
     * @return equipe1
     */
    public String getEquipe1()
    {
        return equipe1;
    }

    /**
     *
     * @brief Mutateur Equipe1
     * @fn setEquipe1
     * @details Modifie le nom de l'Equipe1
     * @param equipe1
     */
    public void setEquipe1(String equipe1)
    {
        this.equipe1 = equipe1;
    }

    /**
     *
     * @brief Accesseur Equipe2
     * @fn getEquipe2
     * @details Retourne le nom de l'Equipe2
     * @return equipe2
     */
    public String getEquipe2()
    {
        return equipe2;
    }

    /**
     *
     * @brief Mutateur Equipe2
     * @fn setEquipe2
     * @details Modifie le nom de l'Equipe2
     * @param equipe2
     */
    public void setEquipe2(String equipe2)
    {
        this.equipe2 = equipe2;
    }
}