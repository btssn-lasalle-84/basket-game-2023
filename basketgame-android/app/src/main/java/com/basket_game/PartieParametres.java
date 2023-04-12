/**
 * @file PartieParametres.java
 * @brief Déclaration de la classe PartieParametres
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * @class PartieParametres
 * @brief L'activité qui permet de paramétrer une partie
 */
public class PartieParametres extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_ParametresPartie"; //!< TAG pour les logs (cf. Logcat)

    /**
     * Attributs
     */

    /**
     * Ressources GUI
     */

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_parametres);

        afficherSuiviPartie();
    }

    /**
     * @brief Méthode appelée pour afficher le suivi de la partie
     */
    private void afficherSuiviPartie()
    {
        Button boutonLancerPartie = findViewById(R.id.boutonLancerPartie);
        boutonLancerPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Intent intent = new Intent(PartieParametres.this, PartieSuivi.class);
                startActivity(intent);
            }
        });
    }
}