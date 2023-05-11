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
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.Serializable;
import java.util.Timer;
import java.util.TimerTask;

/**
 * @class PartieSuivi
 * @brief L'activité qui permet de suivre la partie
 */
public class PartieSuivi extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG      = "_PartieSuivi"; //!< TAG pour les logs (cf. Logcat)
    private static final String EQUIPE_1 = "Rouge";        //!< le nom par défaut pour l'équipe 1
    private static final String EQUIPE_2 = "Jaune";        //!< le nom par défaut pour l'équipe 2

    /**
     * Attributs
     */
    private Partie    partie;                   //!< la partie entre deux équipes
    private Intent    intentDonneesPartieSuivi; //!< les données de la partie
    private int       tempsRestantTour       = Partie.TEMPS_MAX_TOUR; //!< par défaut
    private Timer     compteurTempsTour      = null;
    private TimerTask tacheCompteurTempsTour = null;

    /**
     * Ressources GUI
     */
    ProgressBar progressBarTempsRestantTour;

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_suivi);
        Log.d(TAG, "onCreate()");

        intentDonneesPartieSuivi = getIntent();
        Equipe equipe1           = (Equipe)intentDonneesPartieSuivi.getSerializableExtra("equipe1");
        if(equipe1 != null)
        {
            Log.d(TAG, "onCreate() equipe1 = " + equipe1.getNomEquipe());
        }
        Equipe equipe2 = (Equipe)intentDonneesPartieSuivi.getSerializableExtra("equipe2");
        if(equipe2 != null)
        {
            Log.d(TAG, "onCreate() equipe2 = " + equipe2.getNomEquipe());
        }

        int tempsMaxTour =
          intentDonneesPartieSuivi.getIntExtra("tempsMaxTour", Partie.TEMPS_MAX_TOUR);
        Log.d(TAG, "onCreate() tempsMaxTour = " + tempsMaxTour);
        int nbPaniers = intentDonneesPartieSuivi.getIntExtra("nbPaniers", Partie.NB_PANIERS);
        Log.d(TAG, "onCreate() nbPaniers = " + nbPaniers);
        int nbManches =
          intentDonneesPartieSuivi.getIntExtra("nbManches", Partie.NB_MANCHES_GAGNANTES);
        Log.d(TAG, "onCreate() nbManches = " + nbManches);

        partie = new Partie(equipe1, equipe2);
        partie.setTempsMaxTour(tempsMaxTour);
        partie.setNbPaniers(nbPaniers);
        partie.setNbManchesGagnantes(nbManches);

        afficherNomEquipe1();
        afficherNomEquipe2();
        creerBoutonArreterPartie();

        initialiserCompteurTempsTour();
    }

    /**
     * @brief Méthode
     */
    private void initialiserCompteurTempsTour()
    {
        progressBarTempsRestantTour = (ProgressBar)findViewById(R.id.progressBarTempsRestantTour);
        compteurTempsTour           = new Timer();
        /**
         * @todo Initialiser les valeurs de comptage avec la valeur paramétrée dans l'IHM
         */
        /*
        tempsRestantTour = x;
        progressBarTempsRestantTour.setMax(x);
        progressBarTempsRestantTour.setProgress(x);
        */
        // démarrer le comptage d'un tour
        compterTempsRestantTour();
    }

    /**
     * @brief Assure la gestion du temps pour un tour
     */
    private void compterTempsRestantTour()
    {
        tacheCompteurTempsTour = new TimerTask() {
            public void run()
            {
                /**
                 * @todo Décrémenter d'une seconde le temps restant
                 */
                /**
                 * @todo Tester si le temps restant est arrivé à 0
                 */
                runOnUiThread(new Runnable() {
                    public void run()
                    {
                        /**
                         * @todo Mettre à jour la barre de progression dans l'IHM
                         */
                        // progressBarTempsRestantTour.setProgress(x);
                        /**
                         * @todo Bonus : changer la couleur lorsque il ne reste plus beaucoup de
                         * temps
                         * @see
                         *   progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.RED));
                         */
                    }
                });
            }
        };

        // tâche périodique
        compteurTempsTour.schedule(tacheCompteurTempsTour, 1000, 1000);
    }

    /**
     * @brief Méthode appelée pour afficher le nom de l'équipe 1
     */
    private void afficherNomEquipe1()
    {
        TextView affichageNomEquipe1 = findViewById(R.id.affichageNomEquipe1);
        if(!partie.getEquipe1().getNomEquipe().isEmpty())
        {
            Log.d(TAG, "afficherNomEquipe1() equipe1 = " + partie.getEquipe1().getNomEquipe());
            String nomEquipe1 = partie.getEquipe1().getNomEquipe();
            affichageNomEquipe1.setText(nomEquipe1);
        }
        else
        {
            String nomEquipe1 = EQUIPE_1; // Ajout de la valeur "Rouge" par défaut
            Log.d(TAG, "afficherNomEquipe1() equipe1 = " + nomEquipe1);
            affichageNomEquipe1.setText(nomEquipe1);
            partie.getEquipe1().setNomEquipe(nomEquipe1);
        }
    }

    /**
     * @brief Méthode appelée pour afficher le nom de l'équipe 2
     */
    private void afficherNomEquipe2()
    {
        TextView affichageNomEquipe2 = findViewById(R.id.affichageNomEquipe2);
        if(!partie.getEquipe2().getNomEquipe().isEmpty())
        {
            Log.d(TAG, "afficherNomEquipe1() equipe2 = " + partie.getEquipe2().getNomEquipe());
            String nomEquipe2 = partie.getEquipe2().getNomEquipe();
            affichageNomEquipe2.setText(nomEquipe2);
        }
        else
        {
            String nomEquipe2 = EQUIPE_2; // Ajout de la valeur "Jaune" par défaut
            Log.d(TAG, "afficherNomEquipe2() equipe2 = " + nomEquipe2);
            affichageNomEquipe2.setText(nomEquipe2);
            partie.getEquipe2().setNomEquipe(nomEquipe2);
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
    private void arreterPartie()
    {
        Log.d(TAG, "arreterPartie()");
        Intent intent = new Intent(PartieSuivi.this, PartieInterrompue.class);
        startActivity(intent);
    }
}