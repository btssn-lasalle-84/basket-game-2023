/**
 * @file PartieSuivi.java
 * @brief Déclaration de la classe PartieSuivi
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * @class PartieSuivi
 * @brief L'activité qui permet de suivre la partie
 */
public class PartieSuivi extends AppCompatActivity {

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_suivi);
    }

    /**
     * @brief Méthode appelée pour arrêter la partie
     */
    private void arreterPartie()
    {
        Button boutonArreterPartie = findViewById(R.id.boutonArreterPartie);
        boutonArreterPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Intent intent = new Intent(PartieSuivi.this, PartieInterrompue.class);
                startActivity(intent);
            }
        });
    }
}