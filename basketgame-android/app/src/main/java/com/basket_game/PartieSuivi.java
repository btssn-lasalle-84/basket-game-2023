/**
 * @file PartieSuivi.java
 * @brief Déclaration de la classe PartieSuivi
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
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
    private CommunicationBluetooth communicationBluetooth = null;
    Handler handler = null;


    /**
     * Ressources GUI
     */
    ProgressBar progressBarTempsRestantTour;
    ImageButton boutonConnexionModuleDetection;
    ImageButton boutonConnexionModuleSignalisation;
    ImageButton boutonConnexionModuleEcran;

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_suivi);
        Log.d(TAG, "onCreate()");

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
        initialiserCompteurTempsTour();
        connecterModules();
        verfierConnexionModules();
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
     * @brief Méthode
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
                tempsRestantTour--;
                if (tempsRestantTour <= 0) {
                    tempsRestantTour = partie.getTempsMaxTour();
                    runOnUiThread(new Runnable() {
                        public void run() {
                            progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.GREEN));
                        }
                    });
                }
                runOnUiThread(new Runnable() {
                    public void run()
                    {
                        progressBarTempsRestantTour.setProgress(tempsRestantTour);

                        if (tempsRestantTour <= Partie.SEUIL_TEMPS_RESTANT) {
                            progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.RED));
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
        boutonConnexionModuleDetection.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION)) {
                    communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_DETECTION, CommunicationBluetooth.ID_MODULE_DETECTION);
                    boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
                } else {
                    communicationBluetooth.seDeconnecter(CommunicationBluetooth.ID_MODULE_DETECTION);
                    boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
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
        boutonConnexionModuleSignalisation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_SIGNALISATION)) {
                        communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_SIGNALISATION, CommunicationBluetooth.ID_MODULE_SIGNALISATION);
                        boutonConnexionModuleSignalisation.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
                    } else {
                        communicationBluetooth.seDeconnecter(CommunicationBluetooth.ID_MODULE_SIGNALISATION);
                        boutonConnexionModuleSignalisation.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
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
        boutonConnexionModuleEcran.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if(!communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN)) {
                    communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_ECRAN, CommunicationBluetooth.ID_MODULE_ECRAN);
                    boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
                } else {
                    communicationBluetooth.seDeconnecter(CommunicationBluetooth.ID_MODULE_ECRAN);
                    boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
                }
            }
        });
    }

    /**
     * @brief Méthode appelée pour vérifier la connexion aux modules
     */
    private void verfierConnexionModules()
    {
        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_DETECTION)) {
            boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
        } else {
            boutonConnexionModuleDetection.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        }

        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_SIGNALISATION)) {
            boutonConnexionModuleSignalisation.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
        } else {
            boutonConnexionModuleSignalisation.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        }

        if(communicationBluetooth.estConnecte(CommunicationBluetooth.ID_MODULE_ECRAN)) {
            boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));
        } else {
            boutonConnexionModuleEcran.setBackgroundTintList(ColorStateList.valueOf(Color.RED));
        }
    }

    /**
     * @brief Méthode appelée pour arrêter le compteur
     */
    private void arreterCompteur()
    {
        if (tacheCompteurTempsTour != null)
        {
            tacheCompteurTempsTour.cancel();
            tacheCompteurTempsTour = null;
        }
    }

    /**
     * @brief Méthode appelée pour réinitialiser le compteur
     */
    private void reinitialiserCompteur()
    {
        tempsRestantTour = partie.getTempsMaxTour();
        progressBarTempsRestantTour.setProgress(tempsRestantTour);
        progressBarTempsRestantTour.setProgressTintList(ColorStateList.valueOf(Color.GREEN));
    }

    /**
     * @brief Méthode appelée pour connecter les modules Bluetooth
     */
    private void connecterModules() {
        Log.d(TAG, "connecterModules()");
        if(communicationBluetooth == null) {
            communicationBluetooth = CommunicationBluetooth.getInstance(handler);
        }
        if(communicationBluetooth != null) {
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_DETECTION, CommunicationBluetooth.ID_MODULE_DETECTION);
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_SIGNALISATION, CommunicationBluetooth.ID_MODULE_SIGNALISATION);
            communicationBluetooth.seConnecter(CommunicationBluetooth.NOM_MODULE_ECRAN, CommunicationBluetooth.ID_MODULE_ECRAN);
        }
    }

    /**
     * @brief Méthode appelée pour démarrer la partie
     */
    private void demarrerPartie() {
        Log.d(TAG, "demarrerPartie()");
        fabriquerTrameDebutPartie();
    }

    /**
     * @brief Méthode appelée pour arrêter la partie
     */
    private void arreterPartie()
    {
        arreterCompteur();
        reinitialiserCompteur();
        fabriquerTrameArretPartie();

        Log.d(TAG, "arreterPartie()");
        Intent intent = new Intent(PartieSuivi.this, PartieInterrompue.class);
        startActivity(intent);
    }

    /**
     * @brief Méthode appelée pour initialiser la Handler
     */
    @SuppressLint("HandlerLeak")
    private void initialiserHandler() {
        Log.d(TAG, "initialiserHandler()");
        handler = new Handler() {
            public void handleMessage(Message message) {
                super.handleMessage(message);
                //Log.d(TAG, "handleMessage() what = " + message.what);

                switch (message.what)
                {
                    case CommunicationBluetooth.CONNEXION_BLUETOOTH:
                        Log.d(TAG, "handleMessage() CONNEXION_BLUETOOTH " + message.obj.toString());
                        if(message.obj.toString().equals(CommunicationBluetooth.NOM_MODULE_DETECTION))
                        {
                            demarrerPartie();
                        }
                        break;
                    case CommunicationBluetooth.RECEPTION_BLUETOOTH:
                        Log.d(TAG, "handleMessage() RECEPTION_BLUETOOTH " + message.obj.toString());
                        break;
                    case CommunicationBluetooth.DECONNEXION_BLUETOOTH:
                        Log.d(TAG, "handleMessage() DECONNEXION_BLUETOOTH " + message.obj.toString());
                        break;
                    default:
                        Log.e(TAG, "handleMessage() what = " + message.what + " !!!");
                }


            }
        };
        Log.d(TAG, "initialiserHandler() handler = " + handler);
    }

    /**
     * @brief Méthode appelée pour fabriquer la trame de début de partie
     */
    private void fabriquerTrameDebutPartie() {
        communicationBluetooth.envoyer(CommunicationBluetooth.DELIMITEUR_DEBUT_TRAME + CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME +
                        CommunicationBluetooth.Type.START + CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME + "1" +
                        CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME +
                        CommunicationBluetooth.DELIMITEUR_FIN_TRAME,
                        CommunicationBluetooth.ID_MODULE_DETECTION);
    }

    /**
     * @brief Méthode appelée pour fabriquer la trame d'arrêt de partie
     */
    private void fabriquerTrameArretPartie() {
        communicationBluetooth.envoyer(CommunicationBluetooth.DELIMITEUR_DEBUT_TRAME + CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME +
                        CommunicationBluetooth.Type.STOP + CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME + "1" +
                        CommunicationBluetooth.DELIMITEUR_CHAMPS_TRAME +
                        CommunicationBluetooth.DELIMITEUR_FIN_TRAME,
                CommunicationBluetooth.ID_MODULE_DETECTION);
    }
}