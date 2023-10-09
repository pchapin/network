//**************************************************************************
// FILE   : DayTimeClient.java
// SUBJECT: Simple TCP client program in Java
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

import java.io.InputStream;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Main class of a simple daytime client. See RFC 867.
 */
public class DaytimeClient {

    public static void main(String[] args)
    {
        try {
            wrappedMain(args);
        }
        catch (Exception ex) {
            System.out.println("Unhandled exception: " + ex.getMessage());
        }
    }


    public static void wrappedMain(String[] args) throws java.io.IOException
    {
        // Get stuff from the command line if it is there.
        int port = 13;
        if (args.length >= 2) {
            port = Integer.parseInt(args[1]);
        }
        String host = "localhost";
        if (args.length >= 1) {
            host = args[0];
        }

        // Connect to the server.
        InetAddress serverAddress = InetAddress.getByName(host);
        Socket clientSocket = new Socket(serverAddress, port);
        try (InputStream input = clientSocket.getInputStream()) {
            byte[] buffer = new byte[1024];

            // Get everything and display it.
            int count;
            while ((count =  input.read(buffer, 0, buffer.length)) != -1) {
                System.out.write(buffer, 0, count);
            }
        }
    }

}
