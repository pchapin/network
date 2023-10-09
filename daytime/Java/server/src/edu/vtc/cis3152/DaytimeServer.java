//**************************************************************************
// FILE   : DayTimeServer.java
// SUBJECT: Simple TCP server program in Java
//
// Please send comments or bug reports to
//
//      Peter C. Chapin
//      Computer Information Systems
//      Vermont Technical College
//      Randolph Center, VT 05061
//      PChapin@vtc.vsc.edu
//***************************************************************************
package edu.vtc.cis3152;

import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Calendar;

/**
 * Main class of a simple daytime server. See RFC 867.
 */
public class DaytimeServer {

    public static void main(String[] args)
    {
        try {
            wrappedMain(args);
        }
        catch (Exception ex) {
            System.out.println("Unhandled exception: " + ex.getMessage());
        }
    }


    public static void wrappedMain(String[] args) throws IOException
    {
        // Get stuff from the command line if it is there.
        int port = 13;
        if (args.length >= 1) {
            port = Integer.parseInt(args[0]);
        }

        ServerSocket listenerSocket = new ServerSocket(port);
        while (true) {
            try {
                Socket connectedSocket = listenerSocket.accept();
                System.out.println(
                        "Accepted connection from: " + connectedSocket.getInetAddress());
                try (OutputStream output = connectedSocket.getOutputStream()) {

                    // Output the daytime.
                    // Format suggested by RFC-867: Tuesday, February 22, 1982 17:37:43-PST
                    Calendar now = Calendar.getInstance();
                    String daytime = String.format("%1$tB %1$te, %1$tY %1$tT\n", now);
                    output.write(daytime.getBytes());
                }
            }
            catch (Exception ex) {
                System.out.println(
                        "Unhandled exception during accept or client processing: " + ex.getMessage());
            }
        }
    }

}
