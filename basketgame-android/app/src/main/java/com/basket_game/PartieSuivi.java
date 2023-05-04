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

        afficherNomEquipe1();
        afficherNomEquipe2();
        creerBoutonArreterPartie();
    }

    /**
     * @brief Méthode appelée pour afficher le nom de l'équipe 1
     */
    private void afficherNomEquipe1()
    {
        Equipe equipe1 = (Equipe) getIntent().getSerializableExtra("equipe1");
        if(equipe1.getNomEquipe() != "")
        {
            Log.d(TAG, "afficherNomEquipe1() equipe1 = " + equipe1.getNomEquipe());
            String nomEquipe1 = equipe1.getNomEquipe();
            TextView affichageNomEquipe1 = findViewById(R.id.affichageNomEquipe1);
            affichageNomEquipe1.setText(nomEquipe1);
        }
        else
        {
            String nomEquipe1 = "Rouge"; // Ajout de la valeur "Rouge" par défaut
            TextView affichageNomEquipe1 = findViewById(R.id.affichageNomEquipe1);
            Log.d(TAG, "afficherNomEquipe1() equipe1 = " + nomEquipe1);
            affichageNomEquipe1.setText(nomEquipe1);
        }
    }

    /**
     * @brief Méthode appelée pour afficher le nom de l'équipe 2
     */
    private void afficherNomEquipe2()
    {
        Equipe equipe2 = (Equipe) getIntent().getSerializableExtra("equipe2");
        if(equipe2.getNomEquipe() != "")
        {
            Log.d(TAG, "afficherNomEquipe1() equipe2 = " + equipe2.getNomEquipe());
            String nomEquipe2 = equipe2.getNomEquipe();
            TextView affichageNomEquipe2 = findViewById(R.id.affichageNomEquipe2);
            affichageNomEquipe2.setText(nomEquipe2);
        }
        else
        {
            String nomEquipe2 = "Jaune"; // Ajout de la valeur "Jaune" par défaut
            TextView affichageNomEquipe2 = findViewById(R.id.affichageNomEquipe2);
            Log.d(TAG, "afficherNomEquipe2() equipe2 = " + nomEquipe2);
            affichageNomEquipe2.setText(nomEquipe2);
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