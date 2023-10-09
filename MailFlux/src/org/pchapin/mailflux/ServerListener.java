//-----------------------------------------------------------------------
// FILE    : ServerListener.java
// SUBJECT : Class that listens for incoming client connections.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

import java.net.ServerSocket;
import java.net.Socket;

/**
 * Listen for incoming client connections. Instances of this class run in their own threads and
 * spend significant time blocked on accept(). Whenever a new client connects a new instance of
 * the ServerConnection class is created to interact with that client. The ServerListener thread
 * then immediately tries to accept another connection.
 */
public class ServerListener extends Thread {

    private Logger logger;
    private Spool mainSpool;

    /**
     * Constructs a ServerListener.
     *
     * @param logger The object to be used for processing log messages. All ServerConnection
     * objects created by this ServerListener will use the same logger.
     * @param mainSpool This object represents the primary spool area used to store incoming
     * messages.
     */
    public ServerListener(Logger logger, Spool mainSpool)
    {
        this.logger = logger;
        this.mainSpool = mainSpool;
    }

    /**
     * The main method of the ServerListener thread.
     */
    public void run()
    {
        try {
            // TODO: Take port address from a configuration file and/or the command line.
            ServerSocket listeningSocket = new ServerSocket(9100);

            while (true) {
                Socket clientSocket = listeningSocket.accept();
                ServerConnection clientConnection =
                        new ServerConnection(logger, mainSpool, clientSocket);
                clientConnection.start();
            }
        }
        catch (Exception ex) {
            logger.log(
                    "Unhandled exception in ServerListener: " + ex.getClass() +
                            ", message = " + ex.getMessage());
        }
    }
}
