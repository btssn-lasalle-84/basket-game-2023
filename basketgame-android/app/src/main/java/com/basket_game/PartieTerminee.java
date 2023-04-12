/**
 * @file PartieTerminee.java
 * @brief Déclaration de la classe PartieTerminee
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

/**
 * @class PartieTerminee
 * @brief L'activité qui indique la fin de la partie
 */
public class PartieTerminee extends AppCompatActivity {

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_terminee);
    }
}