/**
 * @file PartieSuivi.java
 * @brief Déclaration de la classe PartieSuivi
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.util.Arrays;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;
import android.widget.Toast;

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
    private Partie                 partie;                   //!< la partie entre deux équipes
    private Intent                 intentDonneesPartieSuivi; //!< les données de la partie
    private Intent                 intentDonneesPartieArretee; //!< les données de la fin de partie
    private int                    tempsRestantTour       = Partie.TEMPS_MAX_TOUR; //!< par défaut
    private Timer                  compteurTempsTour      = null;
    private TimerTask              tacheCompteurTempsTour = null;
    private CommunicationBluetooth communicationBluetooth = null;
    Handler                        handler                = null;
    private String numeroPanier;
    private String couleur;
    private boolean[] trameEnvoyeeSeance = new boolean[CommunicationBluetooth.NB_MODULES];
    private boolean[] trameEnvoyeeDebutPartie = new boolean[CommunicationBluetooth.NB_MODULES];
    private boolean[] trameEnvoyeeArretPartie = new boolean[CommunicationBluetooth.NB_MODULES];
    private boolean[] trameEnvoyeePausePartie = new boolean[CommunicationBluetooth.NB_MODULES];

    /**
     * Ressources GUI
     */
    ProgressBar progressBarTempsRestantTour;
    ImageButton boutonConnexionModuleDetection;
    ImageButton boutonConnexionModuleSignalisation;
    ImageButton boutonConnexionModuleEcran;
    TextView    affichageScoreEquipe1;
    TextView    affichageScoreEquipe2;
    TextView affichageNumeroPanier;
    TextView affichageCouleurEquipe;

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_suivi);
        Log.d(TAG, "onCreate()");

        initialiserActivite();
        recupererParametresPartie();
        afficherNomEquipe1();
        afficherNomEquipe2();
        creerBoutonArreterPartie();
        creerBoutonConnexionModuleDetection();
        creerBoutonConnexionModuleSignalisation();
        creerBoutonConnexionModuleEcran();
        initialiserHandler();
    }

    /**
     * @brief Méthode appelée au démarrage après le onCreate() ou un restart
     * après un onStop()
     */
    @Override
    protected void onStart()
    {
        super.onStart();
        Log.d(TAG, "onStart()");
        initialiserCompteursScoresPanier();
        initialiserCompteursScoresManche();
        initialiserCompteurTempsTour();
        initialiserAffichageNumeroPanier();
        initialiserAffichageCouleurEquipe();
        partie.reinitialiserNumeroManche();
        connecterModules();
        verifierConnexionModules();
        compterTempsRestantTour();
        Arrays.fill(trameEnvoyeeSeance, false);
        Arrays.fill(trameEnvoyeeDebutPartie, false);
        Arrays.fill(trameEnvoyeeArretPartie, false);
        Arrays.fill(trameEnvoyeePausePartie, false);
    }

    /**
     * @brief Méthode appelée après onStart() ou après onPause()
     */
    @Override
    protected void onResume()
    {
        super.onResume();
        Log.d(TAG, "onResume()");
    }

    /**
     * @brief Méthode appelée après qu'une boîte de dialogue s'est affichée (on
     * reprend sur un onResume()) ou avant onStop() (activité plus visible)
     */
    @Override
    protected void onPause()
    {
        super.onPause();
        Log.d(TAG, "onPause()");
    }

    /**
     * @brief Méthode appelée lorsque l'activité n'est plus visible
     */
    @Override
    protected void onStop()
    {
        communicationBluetooth.seDeconnecter(CommunicationBluetooth.ID_MODULE_DETECTION);
        super.onStop();
        Log.d(TAG, "onStop()");
    }

    /**
     * @brief Méthode qui intialise la vue de l'activité
     */
    private void initialiserActivite()
    {
        intentDonneesPartieArretee = new Intent(PartieSuivi.this, PartieArretee.class);
    }

    /**
     * @brief Méthode permettant d'initialiser les compteurs du nombre de panier des équipes
     */
    private void initialiserCompteursScoresPanier()
    {
        partie.getEquipe1().reinitialiserScorePanier();
        partie.getEquipe2().reinitialiserScorePanier();

        affichageScoreEquipe1 = findViewById(R.id.affichageScoreEquipe1);
        String scoreEquipe1   = String.valueOf(partie.getEquipe1().getScorePanier());
        affichageScoreEquipe1.setText(scoreEquipe1);
        affichageScoreEquipe2 = findViewById(R.id.affichageScoreEquipe2);
        String scoreEquipe2   = String.valueOf(partie.getEquipe2().getScorePanier());
        affichageScoreEquipe2.setText(scoreEquipe2);
    }

    /**
     * @brief Méthode permettant d'initialiser les compteurs des scores des manches gagnées des équipes
     */
    private void initialiserCompteursScoresManche()
    {
        partie.getEquipe1().reinitialiserScoreManche();
        partie.getEquipe2().reinitialiserScoreManche();
    }

    /**
     * @brief Méthode permettant d'initialiser le TextView permettant l'affichage du numéro de panier
     */
    private void initialiserAffichageNumeroPanier()
    {
        affichageNumeroPanier = findViewById(R.id.affichageNumeroPanier);
        String numeroPanier = "";
        affichageNumeroPanier.setText(numeroPanier);
    }

    /**
     * @brief Méthode permettant d'initialiser le TextView permettant l'affichage du numéro de panier
     */
    private void initialiserAffichageCouleurEquipe()
    {
        affichageCouleurEquipe = findViewById(R.id.affichageCouleurEquipe);
        String couleur = "";
        affichageCouleurEquipe.setText(couleur);
    }

    /**
     * @brief Méthode permettant d'initialiser le compteur du temps
     */
    private void initialiserCompteurTempsTour()
    {
        progressBarTempsRestantTour = (ProgressBar)findViewById(R.id.progressBarTempsRestantTour);
        compteurTempsTour           = new Timer();

        tempsRestantTour = partie.getTempsMaxTour();
        Log.d(TAG, "initialiserCompteurTempsTour() tempsRestantTour = " + tempsRestantTour);
        progressBarTempsRestantTour.setMax(tempsRestantTour);
        progressBarTempsRestantTour.setProgress(tempsRestantTour);
        progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.GREEN));

    }

    /**
     * @brief Assure la gestion du temps pour un tour
     */
    private void compterTempsRestantTour()
    {
        tacheCompteurTempsTour = new TimerTask() {
            public void run()
            {
                tempsRestantTour--;
                if(tempsRestantTour <= 0)
                {
                    tempsRestantTour = partie.getTempsMaxTour();
                    runOnUiThread(new Runnable() {
                        public void run()
                        {
                            progressBarTempsRestantTour.setProgressTintList(
                              ColorStateList.valueOf(Color.GREEN));
                        }
                    });
                }
                runOnUiThread(new Runnable() {
                    public void run()
                    {
                        progressBarTempsRestantTour.setProgress(tempsRestantTour);

                        if(tempsRestantTour <= Partie.SEUIL_TEMPS_RESTANT)
                        {
                            progressBarTempsRestantTour.setProgressTintList(
                              ColorStateList.valueOf(Color.RED));
                        }
                    }
                });
            }
        };

        // tâche périodique
        compteurTempsTour.schedule(tacheCompteurTempsTour, 1000, 1000);
    }

    /**
     * @brief Méthode appelée pour récupérer les paramètres de la partie
     */
    private void recupererParametresPartie()
    {
        intentDonneesPartieSuivi = getIntent();
        Equipe equipe1 = (Equipe) intentDonneesPartieSuivi.getSerializableExtra("equipe1");
        if(equipe1 != null)
        {
            Log.d(TAG, "onCreate() equipe1 = " + equipe1.getNomEquipe());
        }
        Equipe equipe2 = (Equipe) intentDonneesPartieSuivi.getSerializableExtra("equipe2");
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
     * @brief Méthode appelée pour afficher le score de l'équipe 1
     */
    private void afficherScoreEquipe1()
    {
        affichageScoreEquipe1 = findViewById(R.id.affichageScoreEquipe1);
        String scoreEquipe1   = String.valueOf(partie.getEquipe1().getScorePanier());
        Log.d(TAG, "afficherScoreEquipe1() score Equipe1 = " + partie.getEquipe1().getScorePanier());
        affichageScoreEquipe1.setText(scoreEquipe1);
    }

    /**
     * @brief Méthode appelée pour afficher le score de l'équipe 2
     */
    private void afficherScoreEquipe2()
    {
        TextView affichageScoreEquipe2 = findViewById(R.id.affichageScoreEquipe2);
        String   scoreEquipe2          = String.valueOf(partie.getEquipe2().getScorePanier());
        Log.d(TAG, "afficherScoreEquipe2() score Equipe2 = " + partie.getEquipe2().getScorePanier());
        affichageScoreEquipe2.setText(scoreEquipe2);
    }

    /**
     * @brief Méthode appelée pour afficher le numéro de panier
     */
    private void afficherNumeroPanier()
    {
        affichageNumeroPanier = findViewById(R.id.affichageNumeroPanier);
        Log.d(TAG, "afficherNumeroPanier() numero panier = " + numeroPanier);
        String panierNumero = "Panier N°" + numeroPanier;
        if(Objects.equals(couleur, ProtocoleBasket.COULEUR_EQUIPE_ROUGE))
        {
            affichageNumeroPanier.setTextColor(Color.parseColor("#D00000"));
        }
        else if(Objects.equals(couleur, ProtocoleBasket.COULEUR_EQUIPE_JAUNE))
        {
            affichageNumeroPanier.setTextColor(Color.parseColor("#FFBE0B"));
        }
        affichageNumeroPanier.setText(panierNumero);
    }

    /**
     * @brief Méthode appelée pour afficher la couleur de l'équipe ayant mis le panier
     */
    private void afficherCouleurEquipe()
    {
        affichageCouleurEquipe = findViewById(R.id.affichageCouleurEquipe);
        Log.d(TAG, "affichageCouleurEquipe() couleur Equipe = " + couleur);
        if(Objects.equals(couleur, ProtocoleBasket.COULEUR_EQUIPE_ROUGE))
        {
            affichageCouleurEquipe.setTextColor(Color.parseColor("#D00000"));
        }
        else if(Objects.equals(couleur, ProtocoleBasket.COULEUR_EQUIPE_JAUNE))
        {
            affichageCouleurEquipe.setTextColor(Color.parseColor("#FFBE0B"));
        }
        affichageCouleurEquipe.setText(couleur);
    }

    /**
     * @brief Méthode appelée pour afficher la boite de dialogue
     */
    private void afficherBoiteDialogue() {
        Log.d(TAG, "afficherBoiteDialogue() Manche suivante = " + partie.getNumeroManche());
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Prochaine manche");
        builder.setMessage("Voulez-vous lancer la manche " + partie.getNumeroManche() + " ?");
        for (int i = 0; i < CommunicationBluetooth.NB_MODULES; i++) {
            trameEnvoyeeDebutPartie[i] = false;
        }
        builder.setPositiveButton("Lancer", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                envoyerTrameDebutPartie();
                compterTempsRestantTour();
            }
        });

        builder.setNegativeButton("Annuler", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        AlertDialog dialog = builder.create();
        dialog.show();
    }

    /**
     * @brief Méthode appelée pour afficher la connexion Bluetooth
     */
    private void afficherConnexionBluetooth(String trame)
    {
        if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_DETECTION))
        {
            boutonConnexionModuleDetection.setBackgroundTintList(
                    ColorStateList.valueOf(Color.GREEN));

        }
        else if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_SIGNALISATION))
        {
            boutonConnexionModuleSignalisation.setBackgroundTintList(
                    ColorStateList.valueOf(Color.GREEN));
        }
        else if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_ECRAN))
        {
            boutonConnexionModuleEcran.setBackgroundTintList(
                    ColorStateList.valueOf(Color.GREEN));
        }
        else
        {
            return;
        }
    }

    /**
     * @brief Méthode appelée pour gérer la connexion Bluetooth
     */
    private void gererConnexionBluetooth(String trame) {
        // modules minimum pour jouer
        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION) && (communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN))) {
            demarrerPartie();
        }
    }

    /**
     * @brief Méthode appelée pour afficher la déconnexion Bluetooth
     */
    private void afficherDeconnexionBluetooth(String trame)
    {
        if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_DETECTION))
        {
            boutonConnexionModuleDetection.setBackgroundTintList(
                    ColorStateList.valueOf(Color.RED));
        }
        else if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_SIGNALISATION))
        {
            boutonConnexionModuleSignalisation.setBackgroundTintList(
                    ColorStateList.valueOf(Color.RED));
        }
        else if(trame.equals(
                CommunicationBluetooth.NOM_MODULE_ECRAN))
        {
            boutonConnexionModuleEcran.setBackgroundTintList(
                    ColorStateList.valueOf(Color.RED));
        }
        else
        {
            return;
        }
    }

    /**
     * @brief Méthode appelée pour afficher la déconnexion Bluetooth
     */
    private void gererDeconnexionBluetooth(String trame) {
        // au moins un module déconnecté
        if(!(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION)) || (!(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN)))) {
            arreterCompteurTemps();
            envoyerTramePausePartie();
        }
    }

    /**
     * @brief Méthode appelée pour créer le bouton pour arrêter la partie
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
     * @brief Méthode appelée pour créer le bouton pour se connecter au module de détection
     */
    private void creerBoutonConnexionModuleDetection()
    {
        boutonConnexionModuleDetection = findViewById(R.id.boutonConnexionModuleDetection);
        // par défaut
        boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        boutonConnexionModuleDetection.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION))
                {
                    if(!communicationBluetooth.seConnecter(
                         CommunicationBluetooth.NOM_MODULE_DETECTION,
                         CommunicationBluetooth.ID_MODULE_DETECTION))
                    {
                        Toast toast =
                          Toast.makeText(getApplicationContext(),
                                         "Impossible de se connecter au module " +
                                           CommunicationBluetooth.NOM_MODULE_DETECTION + " !",
                                         Toast.LENGTH_SHORT);
                        toast.show();
                    }
                }
                else
                {
                    communicationBluetooth.seDeconnecter(
                      CommunicationBluetooth.ID_MODULE_DETECTION);
                }
            }
        });
    }

    /**
     * @brief Méthode appelée pour créer le bouton pour se connecter au module de signalisation
     */
    private void creerBoutonConnexionModuleSignalisation()
    {
        boutonConnexionModuleSignalisation = findViewById(R.id.boutonConnexionModuleSignalisation);
        // par défaut
        boutonConnexionModuleSignalisation.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        boutonConnexionModuleSignalisation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_SIGNALISATION))
                {
                    if(!communicationBluetooth.seConnecter(
                            CommunicationBluetooth.NOM_MODULE_SIGNALISATION,
                            CommunicationBluetooth.ID_MODULE_SIGNALISATION))
                    {
                        Toast toast =
                                Toast.makeText(getApplicationContext(),
                                        "Impossible de se connecter au module " +
                                                CommunicationBluetooth.NOM_MODULE_SIGNALISATION + " !",
                                        Toast.LENGTH_SHORT);
                        toast.show();
                    }
                }
                else
                {
                    communicationBluetooth.seDeconnecter(
                            CommunicationBluetooth.ID_MODULE_SIGNALISATION);
                }
            }
        });
    }

    /**
     * @brief Méthode appelée pour créer le bouton pour se connecter au module écran
     */
    private void creerBoutonConnexionModuleEcran()
    {
        boutonConnexionModuleEcran = findViewById(R.id.boutonConnexionModuleEcran);
        // par défaut
        boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        boutonConnexionModuleEcran.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN))
                {
                    if(!communicationBluetooth.seConnecter(
                            CommunicationBluetooth.NOM_MODULE_ECRAN,
                            CommunicationBluetooth.ID_MODULE_ECRAN))
                    {
                        Toast toast =
                                Toast.makeText(getApplicationContext(),
                                        "Impossible de se connecter au module " +
                                                CommunicationBluetooth.NOM_MODULE_ECRAN + " !",
                                        Toast.LENGTH_SHORT);
                        toast.show();
                    }
                }
                else
                {
                    communicationBluetooth.seDeconnecter(
                            CommunicationBluetooth.ID_MODULE_ECRAN);
                }
            }
        });
    }

    /**
     * @brief Méthode appelée pour vérifier la connexion aux modules
     */
    private void verifierConnexionModules()
    {
        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION))
        {
            boutonConnexionModuleDetection.setBackgroundTintList(
              ColorStateList.valueOf(Color.GREEN));
        }
        else
        {
            boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        }

        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_SIGNALISATION))
        {
            boutonConnexionModuleSignalisation.setBackgroundTintList(
              ColorStateList.valueOf(Color.GREEN));
        }
        else
        {
            boutonConnexionModuleSignalisation.setBackgroundTintList(
              ColorStateList.valueOf(Color.RED));
        }

        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN))
        {
            boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
        }
        else
        {
            boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        }
    }

    /**
     * @brief Méthode appelée pour arrêter le compteur
     */
    private void arreterCompteurTemps()
    {
        if(tacheCompteurTempsTour != null)
        {
            tacheCompteurTempsTour.cancel();
            tacheCompteurTempsTour = null;
        }
    }

    /**
     * @brief Méthode appelée pour réinitialiser le compteur
     */
    private void reinitialiserCompteurTemps()
    {
        tempsRestantTour = partie.getTempsMaxTour();
        progressBarTempsRestantTour.setProgress(tempsRestantTour);
        progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.GREEN));
    }

    /**
     * @brief Méthode appelée pour connecter les modules Bluetooth
     */
    private void connecterModules()
    {
        Log.d(TAG, "connecterModules()");
        if(communicationBluetooth == null)
        {
            communicationBluetooth = CommunicationBluetooth.getInstance(handler);
        }
        if(communicationBluetooth != null)
        {
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_DETECTION,
                                               CommunicationBluetooth.ID_MODULE_DETECTION);
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_SIGNALISATION,
                                               CommunicationBluetooth.ID_MODULE_SIGNALISATION);
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_ECRAN,
                                               CommunicationBluetooth.ID_MODULE_ECRAN);
        }
    }

    /**
     * @brief Méthode appelée pour démarrer la partie
     */
    private void demarrerPartie()
    {
        Log.d(TAG, "demarrerPartie()");
        envoyerTrameSeance();
        envoyerTrameDebutPartie();
    }

    /**
     * @brief Méthode appelée pour arrêter la partie
     */
    private void arreterPartie()
    {
        arreterCompteurTemps();
        reinitialiserCompteurTemps();
        envoyerTrameArretPartie();

        Log.d(TAG, "arreterPartie()");
        Intent intent = new Intent(PartieSuivi.this, PartieArretee.class);
        startActivity(intent);
    }

    /**
     * @brief Méthode appelée pour récupérer le vainqueur de la partie
     */
    private void recupererVainqueurPartie()
    {
        int scoreMancheEquipe1 = partie.getEquipe1().getScoreManche();
        int scoreMancheEquipe2 = partie.getEquipe2().getScoreManche();

        if (scoreMancheEquipe1 > scoreMancheEquipe2) {
            intentDonneesPartieArretee.putExtra("equipeGagnante", 1);
        } else if (scoreMancheEquipe1 < scoreMancheEquipe2) {
            intentDonneesPartieArretee.putExtra("equipeGagnante", 2);
        } else {
            intentDonneesPartieArretee.putExtra("equipeGagnante", 0); // Egalité
        }
    }

    /**
     * @brief Méthode appelée pour initialiser la Handler
     */
    @SuppressLint("HandlerLeak")
    private void initialiserHandler()
    {
        Log.d(TAG, "initialiserHandler()");
        handler = new Handler() {
            public void handleMessage(Message message)
            {
                super.handleMessage(message);
                // Log.d(TAG, "handleMessage() what = " + message.what);

                switch(message.what)
                {
                    case CommunicationBluetooth.CONNEXION_BLUETOOTH:
                        Log.d(TAG, "handleMessage() CONNEXION_BLUETOOTH " + message.obj.toString());
                        afficherConnexionBluetooth(message.obj.toString());
                        gererConnexionBluetooth(message.obj.toString());
                        break;
                    case CommunicationBluetooth.RECEPTION_BLUETOOTH:
                        Log.d(TAG, "handleMessage() RECEPTION_BLUETOOTH " + message.obj.toString());
                        String trame = message.obj.toString();
                        if(verifierTrame(trame))
                        {
                            ProtocoleBasket.Type typeTrame = identifierTypeTrame(trame);
                            switch (typeTrame)
                            {
                                case TIR:
                                    gererTrameTir(trame);
                                    break;
                                case FIN:
                                    gererTrameFin(trame);
                                    break;
                            }
                        }
                        break;
                    case CommunicationBluetooth.DECONNEXION_BLUETOOTH:
                        Log.d(TAG,
                              "handleMessage() DECONNEXION_BLUETOOTH " + message.obj.toString());
                        afficherDeconnexionBluetooth(message.obj.toString());
                        gererDeconnexionBluetooth(message.obj.toString());
                        break;
                    default:
                        Log.e(TAG, "handleMessage() what = " + message.what + " !!!");
                }
            }
        };
        Log.d(TAG, "initialiserHandler() handler = " + handler);
    }

    private void gererTrameTir(String trame) {
        String[] champs = trame.split(ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME);
        Log.d(TAG, "gererTrameTir() champs " + champs);
        communicationBluetooth.envoyer(
          trame,
          CommunicationBluetooth.ID_MODULE_ECRAN);
        communicationBluetooth.envoyer(
          trame,
          CommunicationBluetooth.ID_MODULE_SIGNALISATION);

        couleur = champs[ProtocoleBasket.CHAMP_COULEUR_EQUIPE];
        numeroPanier = champs[ProtocoleBasket.CHAMP_NUMERO_PANIER];
        Log.d(TAG, "gererTrameTir() couleur = " + couleur + " - numeroPanier = " + numeroPanier);
        if(couleur.equals(
                ProtocoleBasket.COULEUR_EQUIPE_ROUGE))
        {
            partie.getEquipe1().incrementerScorePanier();
            afficherScoreEquipe1();
        }
        else if(couleur.equals(
                ProtocoleBasket.COULEUR_EQUIPE_JAUNE))
        {
            partie.getEquipe2().incrementerScorePanier();
            afficherScoreEquipe2();
        }
        afficherNumeroPanier();
        afficherCouleurEquipe();

        reinitialiserCompteurTemps();
    }

    private void gererTrameFin(String trame) {
        String[] champs = trame.split(ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME);
        Log.d(TAG, "gererTrameFin() champs " + champs);
        communicationBluetooth.envoyer(
                trame,
                CommunicationBluetooth.ID_MODULE_DETECTION);
        communicationBluetooth.envoyer(
                trame,
                CommunicationBluetooth.ID_MODULE_SIGNALISATION);
        arreterCompteurTemps();
        reinitialiserCompteurTemps();
        initialiserCompteursScoresPanier();
        initialiserAffichageNumeroPanier();
        initialiserAffichageCouleurEquipe();

        couleur = champs[ProtocoleBasket.CHAMP_COULEUR_EQUIPE];
        Log.d(TAG, "gererTrameFin() couleur = " + couleur);
        if(couleur.equals(
                ProtocoleBasket.COULEUR_EQUIPE_ROUGE))
        {
            partie.getEquipe1().incrementerScoreManche();
            Log.d(TAG, "gererTrameFin() Score Manche Equipe1 = " + partie.getEquipe1().getScoreManche());
        }
        else if(couleur.equals(
                ProtocoleBasket.COULEUR_EQUIPE_JAUNE))
        {
            partie.getEquipe2().incrementerScoreManche();
            Log.d(TAG, "gererTrameFin() Score Manche Equipe2 = " + partie.getEquipe2().getScoreManche());
        }

        Log.d(TAG, "gererTrameFin() Manche " + partie.getNumeroManche() + "/" + partie.getNbManchesGagnantes());
        if(partie.getNumeroManche() == partie.getNbManchesGagnantes())
        {
            recupererVainqueurPartie();
            intentDonneesPartieArretee.putExtra("nomEquipe1", partie.getEquipe1().getNomEquipe());
            intentDonneesPartieArretee.putExtra("nomEquipe2", partie.getEquipe2().getNomEquipe());
            startActivity(intentDonneesPartieArretee);
        }
        partie.incrementerNumeroManche();
        if(partie.getNumeroManche() <= partie.getNbManchesGagnantes())
        {
            afficherBoiteDialogue();
        }
    }

    /**
     * @brief Méthode appelée pour vérifier si la trame est valide (délimiteurs)
     */
    private boolean verifierTrame(String trame)
    {
        if(trame.startsWith(ProtocoleBasket.DELIMITEUR_DEBUT_TRAME) && trame.endsWith(ProtocoleBasket.DELIMITEUR_FIN_TRAME ))
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Méthode appelée pour identifier le type de trame
     */
    private ProtocoleBasket.Type identifierTypeTrame(String trame) {
        String[] champs = trame.split(ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME);
        Log.d(TAG, "identifierTypeTrame() champs " + champs);
        if(champs.length >= ProtocoleBasket.NB_CHAMPS_TRAME_TIR &&
                champs[ProtocoleBasket.CHAMP_TYPE_TRAME].equals(
                        ProtocoleBasket.TYPE_TRAME_TIR))
        {
            Log.d(TAG, "identifierTypeTrame() ProtocoleBasket.Type.TIR");
            return ProtocoleBasket.Type.TIR;
        }
        else if(champs.length >= ProtocoleBasket.NB_CHAMPS_TRAME_FIN &&
                champs[ProtocoleBasket.CHAMP_TYPE_TRAME].equals(
                        ProtocoleBasket.TYPE_TRAME_FIN))
        {
            Log.d(TAG, "identifierTypeTrame() ProtocoleBasket.Type.FIN");
            return ProtocoleBasket.Type.FIN;
        }
        else {
            Log.d(TAG, "identifierTypeTrame() ProtocoleBasket.Type.INCONNU");
            return ProtocoleBasket.Type.INCONNU;
        }
    }

    /**
     * @brief Méthode appelée pour fabriquer et envoyer la trame contenant les paramètres de la
     * partie
     */
    private void envoyerTrameSeance() {
        for (int i = 0; i < CommunicationBluetooth.NB_MODULES; i++) {
            if (!trameEnvoyeeSeance[i]) {
                communicationBluetooth.envoyer(
                        ProtocoleBasket.DELIMITEUR_DEBUT_TRAME + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                ProtocoleBasket.TYPE_TRAME_SEANCE + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                partie.getEquipe1().getNomEquipe() + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                partie.getEquipe2().getNomEquipe() + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                partie.getTempsMaxTour() + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                partie.getNbPaniers() + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                partie.getNbManchesGagnantes() + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                ProtocoleBasket.DELIMITEUR_FIN_TRAME,
                        i);

                trameEnvoyeeSeance[i] = true; // Met à jour l'état d'envoi de la trame
            }
        }
    }

    /**
     * @brief Méthode appelée pour fabriquer et envoyer la trame de début de partie
     */
    private void envoyerTrameDebutPartie() {
        for (int i = 0; i < CommunicationBluetooth.NB_MODULES; i++) {
            if (!trameEnvoyeeDebutPartie[i]) {
                communicationBluetooth.envoyer(
                        ProtocoleBasket.DELIMITEUR_DEBUT_TRAME + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                ProtocoleBasket.TYPE_TRAME_START + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + partie.getNumeroManche() +
                                ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + ProtocoleBasket.DELIMITEUR_FIN_TRAME,
                        i);

                trameEnvoyeeDebutPartie[i] = true; // Met à jour l'état d'envoi de la trame début partie
            }
        }
    }

    /**
     * @brief Méthode appelée pour fabriquer et envoyer la trame d'arrêt de partie
     */
    private void envoyerTrameArretPartie()
    {
        for(int i = 0; i < CommunicationBluetooth.NB_MODULES; i++)
        {
            if(!trameEnvoyeeArretPartie[i]) {
                communicationBluetooth.envoyer(
                        ProtocoleBasket.DELIMITEUR_DEBUT_TRAME + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                ProtocoleBasket.TYPE_TRAME_STOP + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + partie.getNumeroManche() +
                                ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + ProtocoleBasket.DELIMITEUR_FIN_TRAME,
                        i);

                trameEnvoyeeArretPartie[i] = true; // Met à jour l'état d'envoi de la trame
            }
        }
    }

    /**
     * @brief Méthode appelée pour fabriquer et envoyer la trame pour reset la partie
     */
    private void envoyerTrameReset()
    {
        for(int i = 0; i < CommunicationBluetooth.NB_MODULES; i++)
        {
            communicationBluetooth.envoyer(
              ProtocoleBasket.DELIMITEUR_DEBUT_TRAME + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                ProtocoleBasket.TYPE_TRAME_RESET + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                ProtocoleBasket.DELIMITEUR_FIN_TRAME,
              i);
        }
    }

    /**
     * @brief Méthode appelée pour fabriquer et envoyer la trame de pause de partie (si un des modules se déconnecte)
     */
    private void envoyerTramePausePartie()
    {
        for(int i = 0; i < CommunicationBluetooth.NB_MODULES; i++)
        {
            if(!trameEnvoyeePausePartie[i]) {
                communicationBluetooth.envoyer(
                        ProtocoleBasket.DELIMITEUR_DEBUT_TRAME + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME +
                                ProtocoleBasket.TYPE_TRAME_PAUSE + ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + partie.getNumeroManche() +
                                ProtocoleBasket.DELIMITEUR_CHAMPS_TRAME + ProtocoleBasket.DELIMITEUR_FIN_TRAME,
                        i);

                trameEnvoyeePausePartie[i] = true; // Met à jour l'état d'envoi de la trame
            }
        }
    }
}