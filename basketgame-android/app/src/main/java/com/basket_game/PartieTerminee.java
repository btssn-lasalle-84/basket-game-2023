/**
 * @file PartieTerminee.java
 * @brief Déclaration de la classe PartieTerminee
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * @class PartieTerminee
 * @brief L'activité qui indique la fin de la partie
 */
public class PartieTerminee extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_PartieTerminee"; //!< TAG pour les logs (cf. Logcat)

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_terminee);

        recommencerPartie();
        afficherMenuPrincipal();
    }

    /**
     * @brief Méthode appelée pour recommencer une partie avec les mêmes paramètres
     */
    private void recommencerPartie()
    {
        Button boutonRecommencerPartie = findViewById(R.id.boutonRecommencerPartie);
        boutonRecommencerPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Intent intent = new Intent(PartieTerminee.this, PartieSuivi.class);
                startActivity(intent);
            }
        });
    }

    /**
     * @brief Méthode appelée pour retourner au menu principal
     */
    private void afficherMenuPrincipal()
    {
        Button boutonMenuPrincipal = findViewById(R.id.boutonMenuPrincipal);
        boutonMenuPrincipal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Intent intent = new Intent(PartieTerminee.this, BasketGame.class);
                startActivity(intent);
            }
        });
    }
}