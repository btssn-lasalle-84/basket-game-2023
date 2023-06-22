/**
 * @file CommunicationBluetooth.java
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
import java.util.Arrays;
import java.util.Set;
import java.util.UUID;
import java.util.Vector;

/**
 * @class CommunicationBluetooth
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
    public final static String NOM_MODULE_DETECTION     = "Basket-detection"; // émission/réception
    public final static String NOM_MODULE_SIGNALISATION = "basket-signalisation"; // émission
    public final static String NOM_MODULE_ECRAN         = "basket-ecran"; // émission/réception
    public final static int ID_MODULE_DETECTION     = 0;
    public final static int ID_MODULE_SIGNALISATION = 1;
    public final static int ID_MODULE_ECRAN         = 2;
    public final static int NB_MODULES              = 3;

    /**
     * Attributs
     */
    private static CommunicationBluetooth communication       = null;
    private BluetoothAdapter              adaptateurBluetooth = null;
    private Vector<BluetoothDevice>       peripheriques;
    private Vector<BluetoothSocket>       socketsBluetooth;
    private Vector<InputStream> inputStream;
    private Vector<Thread> filsExecutionReception;
    private Handler                       handler = null;


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
        this.inputStream = new Vector<InputStream>();
        this.filsExecutionReception = new Vector<Thread>();
        for(int i = 0; i < NB_MODULES; i++)
        {
            this.peripheriques.add(null);
            this.socketsBluetooth.add(null);
            this.inputStream.add(null);
            this.filsExecutionReception.add(null);
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
        this.inputStream = new Vector<InputStream>();
        this.filsExecutionReception = new Vector<Thread>();
        for(int i = 0; i < NB_MODULES; i++)
        {
            this.peripheriques.add(null);
            this.socketsBluetooth.add(null);
            this.inputStream.add(null);
            this.filsExecutionReception.add(null);
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
     * @brief Pour vérifier la connexion
     */
    public boolean estConnecte(int idModule) {
        if (socketsBluetooth.get(idModule) != null && socketsBluetooth.get(idModule).isConnected()) {
            return true;
        }
        return false;
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
            inputStream.set(idModule, this.socketsBluetooth.get(idModule).getInputStream());

            if(handler != null)
            {
                Log.d(TAG, "handler.sendMessage() CONNEXION_BLUETOOTH");
                Message messageHandler = new Message();
                messageHandler.what    = CONNEXION_BLUETOOTH;
                messageHandler.obj     = peripheriques.get(idModule).getName();
                handler.sendMessage(messageHandler);
            }

            // Démarrer la reception
            recevoir(idModule);

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
        Log.d(TAG, "seDeconnecter(" + idModule + ")");
        if(peripheriques.get(idModule) == null)
            return;
        // Fermer les connexions et le socket
        try
        {
            if(idModule == ID_MODULE_DETECTION)
            {
                if(inputStream != null)
                {
                    inputStream.get(idModule).close();
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
                        Log.d(TAG, "envoyer() idModule = " + idModule + " - Message = " + message);
                        socketsBluetooth.get(idModule).getOutputStream().write(message.getBytes());
                        socketsBluetooth.get(idModule).getOutputStream().flush();
                        sleep(250);
                    }
                    catch(IOException e)
                    {
                        Log.e(TAG, "Erreur lors de l'envoi de données");
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    }
                }
            }.start();
        }
    }

    /**
     * @brief Pour recevoir des messages via le Bluetooth
     */
    public void recevoir(int idModule)
    {
        Log.d(TAG, "recevoir() idModule = " + idModule);
        filsExecutionReception.set(idModule, new Thread(new Runnable() {
            @Override
            public void run()
            {
                Log.d(TAG, "recevoir() thread démarré pour idModule " + idModule);
                while(socketsBluetooth.get(idModule).isConnected())
                {
                    try
                    {
                        byte[] donnees = new byte[32];
                        int  n = inputStream.get(idModule).read(donnees);
                        if(n > 0)
                        {
                            if(handler != null)
                            {
                                Message messageHandler = new Message();
                                messageHandler.what    = RECEPTION_BLUETOOTH;
                                // @todo Indiquer la provanance de récaption par l'idModule
                                messageHandler.obj     = new String(Arrays.copyOfRange(donnees,0,n));
                                handler.sendMessage(messageHandler);
                            }
                        }
                    }
                    catch(IOException e)
                    {
                        Log.e(TAG, "Erreur lors de la réception de données");
                        seDeconnecter(idModule);
                    }
                }
                Log.d(TAG, "recevoir() thread arrêté");
            }
        }));
        filsExecutionReception.get(idModule).start();
    }
}