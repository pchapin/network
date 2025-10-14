/*
 * Copyright (C) 2025 Peter Chapin
 *
 * Licensed under the Apache v. 2 License. See the LICENSE file in the project root for details.
 *
 * File:    Main.java
 * Purpose: The main class of the MailFlux application.
 * Author:  Peter Chapin <spicacality@kelseymountain.org>
 */
package org.kelseymountain.mailflux;

/**
 * Entry point class for the MailFlux application.
 * Responsible for initializing core components and starting the server listener thread.
 * TODO: Review Javadoc
 */
public class Main {
    /**
     * Application entry point. Initializes the console and spool, starts the server listener,
     * and waits for it to terminate.
     * TODO: Review Javadoc
     *
     * @param args Command-line arguments (currently unused).
     */
    public static void main(String[] args)
    {
        try {
            Console userConsole = new Console();
            Spool mainSpool = new Spool(userConsole);
            userConsole.log("===> MailFlux v0.00 pre-Alpha ready!");

            ServerListener server = new ServerListener(userConsole, mainSpool);
            server.start();
            server.join();
        }
        catch (Exception ex) {
            System.out.println(
                    "Unhandled exception in Main: " + ex.getClass() +
                            ", message = " + ex.getMessage());
        }
    }
}
