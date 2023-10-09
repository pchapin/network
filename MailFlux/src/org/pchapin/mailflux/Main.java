//-----------------------------------------------------------------------
// FILE    : Main.java
// SUBJECT : The main class of the MailFlux application.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

public class Main {
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
