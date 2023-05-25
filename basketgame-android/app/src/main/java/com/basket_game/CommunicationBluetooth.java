/**
 * @file Communication.java
 * @brief Déclaration de la classe CommunicationBluetooth
 * @author Guillaumet Florent
 */

package com.basket_game;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
import java.util.Vector;

/**
 * @class Communication
 * @brief Définit la classe de communication bluetooth
 */
public class CommunicationBluetooth
{
    /**
     * Constantes
     */
    private static final String TAG = "_CommunicationBluetooth"; //!< TAG pour les logs (cf. Logcat)
    private static final UUID   identifiantUUID =
      UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    public final static int CONNEXION_BLUETOOTH     = 0;
    public final static int RECEPTION_BLUETOOTH     = 1;
    public final static int DECONNEXION_BLUETOOTH   = 2;
    public final static String NOM_MODULE_DETECTION     = "basket-detection"; // émission/réception
    public final static String NOM_MODULE_SIGNALISATION = "basket-signalisation"; // émission
    public final static String NOM_MODULE_ECRAN         = "basket-ecran"; // émission
    public final static int ID_MODULE_DETECTION     = 0; // émission/réception
    public final static int ID_MODULE_SIGNALISATION = 1; // émission
    public final static int ID_MODULE_ECRAN         = 2; // émission
    public final static int NB_MODULES              = 3;
    public final static String DELIMITEUR_DEBUT_TRAME         = "$BASKET"; // délimiteur début trame
    public final static String DELIMITEUR_FIN_TRAME         = "\r\n"; // délimiteur fin trame
    public final static String DELIMITEUR_CHAMPS_TRAME         = ";"; // délimiteur champs trame

    /**
     * Attributs
     */
    private static CommunicationBluetooth communication       = null;
    private BluetoothAdapter              adaptateurBluetooth = null;
    private Vector<BluetoothDevice>       peripheriques;
    private Vector<BluetoothSocket>       socketsBluetooth;
    private InputStream                   inputStream         = null; // pour le module de détection
    private Thread                        filExecutionReception;
    private Handler                       handler = null;
    public enum Type {
        SEANCE, START, TIR, STOP, RESET
    };

    /**
     * @fn getInstance
     * @brief Retourne l'instance Communication
     */
    public synchronized static CommunicationBluetooth getInstance(Handler handler)
    {
        if(communication == null)
        {
            communication = new CommunicationBluetooth(handler);
        }
        else
        {
            communication.setHandler(handler);
        }

        return communication;
    }

    /**
     * @brief Constructeur par défaut
     */
    public CommunicationBluetooth()
    {
        Log.d(TAG, "CommunicationBluetooth()");
        this.peripheriques = new Vector<BluetoothDevice>();
        this.socketsBluetooth = new Vector<BluetoothSocket>();
        for(int i = 0; i < NB_MODULES; i++)
        {
            this.peripheriques.add(null);
            this.socketsBluetooth.add(null);
        }
        activer();
    }

    /**
     * @brief Constructeur d'initialisation
     */
    public CommunicationBluetooth(Handler handler)
    {
        Log.d(TAG, "CommunicationBluetooth(handler)");
        this.peripheriques = new Vector<BluetoothDevice>();
        this.socketsBluetooth = new Vector<BluetoothSocket>();
        for(int i = 0; i < NB_MODULES; i++)
        {
            this.peripheriques.add(null);
            this.socketsBluetooth.add(null);
        }
        this.handler = handler;
        activer();
    }

    /**
     * @fn setHandler
     * @brief Fixe le gestionnaire de messages du thread UI
     * @param handler Handler le gestionnaire de messages du thread UI
     */
    public void setHandler(Handler handler)
    {
        this.handler = handler;
    }

    /**
     * @brief Pour activer le bluetooth
     */
    @SuppressLint({ "MissingPermission" })
    public void activer()
    {
        if(this.adaptateurBluetooth == null)
            this.adaptateurBluetooth = BluetoothAdapter.getDefaultAdapter();
        if(this.adaptateurBluetooth == null)
        {
            Log.e(TAG, "Bluetooth non supporté par l'appareil");
        }
        else if(!adaptateurBluetooth.isEnabled())
        {
            Log.d(TAG, "Bluetooth désactivé -> activation");
            adaptateurBluetooth.enable();
        }
        else
        {
            Log.d(TAG, "Bluetooth activé");
        }
    }

    /**
     * @brief Pour se connecter
     */
    @SuppressLint("MissingPermission")
    public boolean seConnecter(String nomPeripherique, int idModule)
    {
        Log.d(TAG, "seConnecter(" + nomPeripherique + ", " + idModule + ")");
        // Activer le Bluetooth
        activer();

        // Rechercher nomPeripherique dans les appareils déjà appairés
        Set<BluetoothDevice> peripheriquesAppaires = adaptateurBluetooth.getBondedDevices();
        if(peripheriquesAppaires.size() > 0)
        {
            Log.d(TAG, "Nb appareils appairés : " + peripheriquesAppaires.size());
            for(BluetoothDevice appareil: peripheriquesAppaires)
            {
                if(appareil.getName().contains(nomPeripherique))
                {
                    if(peripheriques.get(idModule) == null)
                        this.peripheriques.set(idModule, appareil);
                    break;
                }
            }
        }
        if(peripheriques.get(idModule) == null)
        {
            Log.e(TAG, "Appareil Bluetooth " + nomPeripherique + " non trouvé !");
            return false;
        }
        else
        {
            Log.d(TAG,
                  "Appareil Bluetooth " + peripheriques.get(idModule).getName() +
                    " trouvé : " + peripheriques.get(idModule).getAddress());
            return creerSocket(idModule);
        }
    }

    /**
     * @brief Pour créer un socket Bluetooth
     */
    @SuppressLint("MissingPermission")
    public boolean creerSocket(int idModule)
    {
        Log.d(TAG, "creerSocket(" + idModule + ")");
        // Créer le canal Bluetooth
        try
        {
            this.socketsBluetooth.set(
              idModule,
              peripheriques.get(idModule).createRfcommSocketToServiceRecord(identifiantUUID));
        }
        catch(IOException e)
        {
            Log.e(TAG, "Erreur lors de la creation du canal");
            return false;
        }
        // Connecter le canal
        try
        {
            this.socketsBluetooth.get(idModule).connect();
            if(idModule == ID_MODULE_DETECTION)
            {
                inputStream = this.socketsBluetooth.get(idModule).getInputStream();
            }
            if(handler != null)
            {
                Log.d(TAG, "Message handler");
                Message messageHandler = new Message();
                messageHandler.what    = CONNEXION_BLUETOOTH;
                messageHandler.obj     = peripheriques.get(idModule).getName();
                handler.sendMessage(messageHandler);
            }
            if(idModule == ID_MODULE_DETECTION)
            {
                // Démarrer la reception
                recevoir();
            }
            Log.d(TAG, "Canal Bluetooth connecté");
            return true;
        }
        catch(IOException e)
        {
            Log.e(TAG, "Erreur lors de la connexion du canal");
            try
            {
                this.socketsBluetooth.get(idModule).close();
            }
            catch(IOException closeException)
            {
                Log.e(TAG, "Erreur lors de la fermeture du socket");
            }
            return false;
        }
    }

    /**
     * @brief Pour se déconnecter
     */
    @SuppressLint("MissingPermission")
    public void seDeconnecter(int idModule)
    {
        Log.d(TAG, "deconnecter(" + idModule + ")");
        // Fermer les connexions et le socket
        try
        {
            if(idModule == ID_MODULE_DETECTION)
            {
                if(inputStream != null)
                {
                    inputStream.close();
                }
            }
            if(this.socketsBluetooth.get(idModule) != null)
            {
                this.socketsBluetooth.get(idModule).getOutputStream().close();
                this.socketsBluetooth.get(idModule).close();
            }
            if(handler != null)
            {
                Message messageHandler = new Message();
                messageHandler.what    = DECONNEXION_BLUETOOTH;
                messageHandler.obj     = peripheriques.get(idModule).getName();
                handler.sendMessage(messageHandler);
            }
        }
        catch(IOException e)
        {
            Log.e(TAG, "Erreur lors de la fermeture des connexions.");
        }
    }

    /**
     * @brief Pour envoyer un message via le Bluetooth
     */
    public void envoyer(String message, int idModule)
    {
        if(this.socketsBluetooth.get(idModule) != null && this.socketsBluetooth.get(idModule).isConnected())
        {
            new Thread() {
                @Override
                public void run()
                {
                    try
                    {
                        Log.d(TAG, "envoyer() " + message);
                        socketsBluetooth.get(idModule).getOutputStream().write(message.getBytes());
                        socketsBluetooth.get(idModule).getOutputStream().flush();
                    }
                    catch(IOException e)
                    {
                        Log.e(TAG, "Erreur lors de l'envoi de données");
                    }
                }
            }.start();
        }
    }

    /**
     * @brief Pour recevoir des messages via le Bluetooth
     */
    public void recevoir()
    {
        Log.d(TAG, "recevoir()");
        filExecutionReception = new Thread(new Runnable() {
            @Override
            public void run()
            {
                Log.d(TAG, "recevoir() thread démarré");
                while(socketsBluetooth.get(ID_MODULE_DETECTION).isConnected())
                {
                    try
                    {
                        String message = String.valueOf((char)inputStream.read());
                        if(message != null)
                        {
                            if(handler != null)
                            {
                                Message messageHandler = new Message();
                                messageHandler.what    = RECEPTION_BLUETOOTH;
                                messageHandler.obj     = message;
                                handler.sendMessage(messageHandler);
                            }
                        }
                    }
                    catch(IOException e)
                    {
                        Log.e(TAG, "Erreur lors de la réception de données");
                        seDeconnecter(ID_MODULE_DETECTION);
                    }
                }
                Log.d(TAG, "recevoir() thread arrêté");
            }
        });
        filExecutionReception.start();
    }
}
