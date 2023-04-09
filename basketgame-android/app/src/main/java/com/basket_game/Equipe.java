/**
 * @file Equipe.java
 * @brief Déclaration de la classe Equipe
 * @author Guillaumet Florent
 */

package com.basket_game;

/**
 * @class Equipe
 * @brief Définit une équipe
 */
public class Equipe
{
    /**
     * @brief Attributs
     */
    private String nom; //!< le nom de l'équipe

    /**
     * @brief Constructeur de la classe Equipe
     * @param nom
     */
    public Equipe(String nom)
    {
        this.nom = nom;
    }

    /**
     *
     * @brief Accesseur Equipe1
     * @fn getNom
     * @details Retourne le nom
     * @return String
     */
    public String getNom()
    {
        return nom;
    }

    /**
     *
     * @brief Mutateur Equipe1
     * @fn setEquipe1
     * @details Modifie le nom de l'Equipe1
     * @param equipe1
     */
    public void setNom(String nom)
    {
        this.nom = nom;
    }
}