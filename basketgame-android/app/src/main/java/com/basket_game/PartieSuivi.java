/**
 * @file PartieSuivi.java
 * @brief Déclaration de la classe PartieSuivi
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * @class PartieSuivi
 * @brief L'activité qui permet de suivre la partie
 */
public class PartieSuivi extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_PartieSuivi"; //!< TAG pour les logs (cf. Logcat)

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_suivi);
        Log.d(TAG, "onCreate()");

        Intent intentDonneesPartieSuivi = getIntent();
        Equipe equipe1 = (Equipe) intentDonneesPartieSuivi.getSerializableExtra("equipe1");
        Log.d(TAG, "onCreate() equipe1 = " + equipe1.getNomEquipe());
        Equipe equipe2 = (Equipe) intentDonneesPartieSuivi.getSerializableExtra("equipe2");
        Log.d(TAG, "onCreate() equipe2 = " + equipe2.getNomEquipe());

        //afficherNomEquipe1();
        creerBoutonArreterPartie();
    }

    /**
     * @brief Méthode appelée pour afficher le nom de l'équipe 1
     */
    private void afficherNomEquipe1()
    {
        Equipe equipe1 = (Equipe) getIntent().getSerializableExtra("equipe1");
        if(equipe1 != null) {
            Log.d(TAG, "afficherNomEquipe1() equipe1 = " + equipe1.getNomEquipe());
            String nomEquipe1 = equipe1.getNomEquipe();
            TextView affichageNomEquipe1 = findViewById(R.id.affichageNomEquipe1);
            affichageNomEquipe1.setText(nomEquipe1);
        }
        else
        {
            /**
             * @todo Prévoir un nom par défaut pour l'équipe
             */
        }
    }

    /**
     * @brief Méthode appelée pour arrêter la partie
     */
    private void creerBoutonArreterPartie()
    {
        Button boutonArreterPartie = findViewById(R.id.boutonArreterPartie);
        boutonArreterPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                arreterPartie();
            }
        });
    }

    /**
     * @brief Méthode appelée pour arrêter la partie
     */
    private void arreterPartie() {
        Intent intent = new Intent(PartieSuivi.this, PartieInterrompue.class);
        startActivity(intent);
    }
}