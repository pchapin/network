package org.pchapin.mailflux;

import java.io.*;
import java.net.Socket;

public class ClientConnection extends Thread {

    private Logger logger;
    private Message currentMessage;
    private Socket serverSocket;
    private BufferedReader fromServer;
    private OutputStreamWriter toServer;

    /**
     * Constructs a ClientConnection object.
     *
     * @param logger The Logger to use for messages and other informational text.
     * @param currentMessage The message to be sent to the server.
     * @param serverSocket The socket representing the open connection to the server.
     */
    public ClientConnection(Logger logger, Message currentMessage, Socket serverSocket)
        throws IOException
    {
        this.logger         = logger;
        this.currentMessage = currentMessage;
        this.serverSocket   = serverSocket;

        fromServer =
                new BufferedReader(
                        new InputStreamReader(serverSocket.getInputStream(), "US-ASCII"));
        toServer = new OutputStreamWriter(serverSocket.getOutputStream(), "US-ASCII");
        // TODO: Fix the exception safety problem above.
    }

    private boolean doEHLO() throws IOException
    {
        // TODO: Process the EHLO exchange. Return true if successful.
        return false;
    }

    private boolean doMAIL() throws IOException
    {
        // TODO: Process the MAIL FROM exchange. Return true if successful.
        return false;
    }

    private boolean doRCPT() throws IOException
    {
        // TODO: Process the RCPT TO exchange. Return true if successful.
        return false;
    }

    private boolean doMESSAGE() throws IOException
    {
        // TODO: Process the sending message exchange. Return true if successful.
        return false;
    }

    /**
     * The main method of the ClientConnection thread. This method runs the SMTP protocol from
     * the client side, aborting at the first step that indicates failure. When this method
     * returns the connection to the server has been closed.
     */
    public void run()
    {
        try {
            String banner = fromServer.readLine();
            boolean success = doEHLO() && doMAIL() && doRCPT() && doMESSAGE();

            // Terminate the transaction in any case. Ignore the response to the QUIT command.
            toServer.write("QUIT\r\n");
            toServer.flush();
            String response = fromServer.readLine();

            serverSocket.close();
        }
        catch (Exception ex) {
            StackTraceElement[] traceElements = ex.getStackTrace();
            StackTraceElement topElement = traceElements[0];
            logger.log(
                    "Unhandled exception in ClientConnection: " + ex.getClass() +
                            ", message = " + ex.getMessage());
            logger.log(
                    "\tLocation: File = " + topElement.getFileName() +
                            ", Line = " + topElement.getLineNumber());
        }
    }

}
