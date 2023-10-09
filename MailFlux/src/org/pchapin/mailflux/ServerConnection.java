//-----------------------------------------------------------------------
// FILE    : ServerConnection.java
// SUBJECT : Class for doing server side SMTP.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

import java.io.*;
import java.net.Socket;

/**
 * Class for doing server side SMTP. This class interacts with a connected client and performs
 * the SMTP protocol with that client. Instances of this class run in their own thread so that
 * multiple ServerConnection objects can be active at the same time. Errors and other
 * information about the transaction are logged. Any acceptable mail received is spooled.
 *
 * The design of this class should be improved. It should use a table with rows for each state
 * and columns for each possible received command. The table cells would contain an action to
 * perform when a particular command is received while in a particular state. This would make it
 * easier to handle "Bad sequence of commands" error messages since some "bad" commands are
 * perfectly legitimate if received while in an appropriate state.
 *
 * This design could be imposed on the structure of this class by putting a suitable string-
 * switch statement in each of the doXXXX methods. Since the number of SMTP commands is fairly
 * small and not likely to change quickly such an approach would probably be adequate.
 *
 * TODO: Implement the design ideas described above.
 */
public class ServerConnection extends Thread {
    
    private Logger  logger;
    private Message currentMessage;
    private Spool   mainSpool;
    private Socket  clientSocket;

    private enum State {
        WEHLO, WMAIL, WRCPT1, WRCPT2, GETMESSAGE, WQUIT, DONE
    }
    private State currentState;


    /**
     * Obtain a line of non-trivial text from the client. This method responds to "trivial" SMTP
     * commands such as NOOP, HELP, VRFY (unimplemented) and EXPN (unimplemented). It returns
     * the first line received from the client that is not one of those commands.
     *
     * @param fromClient A reader from which client text can be obtained.
     * @param toClient   A writer allowing text to be sent to the client.
     * @return The first non-trivial line of input from the client. Trivial commands are not
     * returned. The returned line is at least four characters long unless the current state is
     * GETMESSAGE (in which case the length is not defined). If the client closes the connection
     * the null reference is returned.
     * @throws IOException if there is a problem communicating with the client.
     */
    private String getNontrivialSMTP(BufferedReader fromClient, OutputStreamWriter toClient)
            throws IOException
    {
        String  line = "";
        boolean gotSomething;

        // When in the GET_MESSAGE state we return anything we see.
        if (currentState == State.GETMESSAGE) {
            line = fromClient.readLine();
        }
        else {
            gotSomething = false;
            while (!gotSomething) {
                line = fromClient.readLine();
                if (line == null) break;

                // Lines less than four characters long must be garbage. Treat them as errors.
                if (line.length() < 4) {
                    toClient.write("500 Unrecognized command\r\n");
                    toClient.flush();
                }
                else {
                    // TODO: We should verify that the fifth character is a white space or an EOL.
                    String command = line.substring(0, 4).toUpperCase();
                    switch (command) {
                        case "NOOP":
                            toClient.write("250 OK\r\n");
                            toClient.flush();
                            break;

                        case "HELP":
                            toClient.write("250 OK\r\n");
                            toClient.flush();
                            break;

                        case "VRFY":
                            toClient.write("502 Not Implemented\r\n");
                            toClient.flush();
                            break;

                        case "EXPN":
                            toClient.write("502 Not Implemented\r\n");
                            toClient.flush();
                            break;

                        default:
                            gotSomething = true;
                            break;
                    }
                }
            }
        }
        return line;
    }
    

    private void doWEHLO(String incomingLine, OutputStreamWriter toClient) throws IOException
    {
        String command = incomingLine.substring(0, 4).toUpperCase();
        switch (command) {
            case "HELO":
            case "EHLO":
                logger.log(incomingLine);
                toClient.write("250 Ok\r\n");
                toClient.flush();
                currentState = State.WMAIL;
                break;

            default:
                logger.log("UNRECOGNIZED (in WEHLO): " + incomingLine);
                toClient.write("500 Unrecognized command\r\n");
                toClient.flush();
        }
    }


    private void doWMAIL(String incomingLine, OutputStreamWriter toClient) throws IOException
    {
        String command = incomingLine.substring(0, 4).toUpperCase();
        switch (command) {
            case "MAIL":
                currentMessage = new Message();
                // TODO: Extract the sender address in a more robust way.
                String senderAddress = incomingLine.substring(10);
                currentMessage.setSender(senderAddress);
                logger.log(incomingLine);
                toClient.write("250 Ok\r\n");
                toClient.flush();
                currentState = State.WRCPT1;
                break;

            case "RSET":
                logger.log(incomingLine);
                toClient.write("250 OK\r\n");
                toClient.flush();
                break;

            default:
                logger.log("UNRECOGNIZED (in WMAIL): " + incomingLine);
                toClient.write("500 Unrecognized command\r\n");
                toClient.flush();
                break;
        }
    }


    private void doWRCPT1(String incomingLine, OutputStreamWriter toClient) throws IOException
    {
        String command = incomingLine.substring(0, 4).toUpperCase();
        switch (command) {
            case "RCPT":
                // TODO: Extract the recipient address in a more robust way.
                String recipientAddress = incomingLine.substring(8);
                currentMessage.addRecipient(recipientAddress);
                logger.log(incomingLine);
                toClient.write("250 Ok\r\n");
                toClient.flush();
                currentState = State.WRCPT2;
                break;

            default:
                logger.log("UNRECOGNIZED (in WRCPT1): " + incomingLine);
                toClient.write("500 Unrecognized command\r\n");
                toClient.flush();
                break;
        }
    }


    private void doWRCPT2(String incomingLine, OutputStreamWriter toClient) throws IOException
    {
        String command = incomingLine.substring(0, 4).toUpperCase();
        switch (command) {
            case "RCPT":
                // TODO: Extract the recipient address in a more robust way.
                String recipientAddress = incomingLine.substring(8);
                currentMessage.addRecipient(recipientAddress);
                logger.log(incomingLine);
                toClient.write("250 Ok\r\n");
                toClient.flush();
                break;

            case "DATA":
                logger.log(incomingLine);
                toClient.write("354 Blah, blah, blah\r\n");
                toClient.flush();
                currentState = State.GETMESSAGE;
                break;

            default:
                logger.log("UNRECOGNIZED (in WRCPT2): " + incomingLine);
                toClient.write("500 Unrecognized command\r\n");
                toClient.flush();
                break;
        }
    }


    private void doGETMESSAGE(String incomingLine, OutputStreamWriter toClient)
            throws IOException
    {
        // TODO: Handle the transparency condition.
        if (incomingLine.equals(".")) {
            logger.log("END OF DATA: " + incomingLine);
            mainSpool.addMessage(currentMessage);
            toClient.write("250 Ok\r\n");  // We say "OK" even if the message wasn't written.
            toClient.flush();
            currentState = State.WQUIT;
        }
        else {
            logger.log(incomingLine);
            currentMessage.appendText(incomingLine);
        }
    }


    private void doWQUIT(String incomingLine, OutputStreamWriter toClient) throws IOException
    {
        // TODO: The RSET and QUIT commands should be accepted at any time.
        String command = incomingLine.substring(0, 4).toUpperCase();
        switch (command) {
            case "RSET":
                logger.log(incomingLine);
                toClient.write("250 Ok\r\n");
                toClient.flush();
                currentState = State.WMAIL;
                break;

            case "QUIT":
                logger.log(incomingLine);
                toClient.write("221 Ok\r\n");
                toClient.flush();
                currentState = State.DONE;
                break;

            default:
                logger.log("UNRECOGNIZED (in WQUIT): " + incomingLine);
                toClient.write("500 Unrecognized command\r\n");
                toClient.flush();
                break;
        }

    }


    /**
     * Constructs a ServerConnection object.
     *
     * @param logger The Logger to use for messages and other informational text.
     * @param mainSpool The main spool area for messages to be saved.
     * @param clientSocket The socket representing the open connection to the client.
     */
    public ServerConnection(Logger logger, Spool mainSpool, Socket clientSocket)
    {
        this.logger       = logger;
        this.mainSpool    = mainSpool;
        this.clientSocket = clientSocket;
    }


    /**
     * The main method of the ServerConnection thread. This method executes a finite state
     * machine representing the server side of the SMTP protocol. See the state diagram in the
     * MailFlux documentation for more information. When this method returns the connection to
     * the client has been closed.
     */
    public void run()
    {
        try {
            logger.log("Accepted client connection from: " + clientSocket.getInetAddress());
            
            BufferedReader fromClient =
                    new BufferedReader(
                            new InputStreamReader(clientSocket.getInputStream(), "US-ASCII"));

            OutputStreamWriter toClient =
                    new OutputStreamWriter(clientSocket.getOutputStream(), "US-ASCII");

            toClient.write("220 localhost MailFlux v0.00\r\n");
            toClient.flush();

            currentState = State.WEHLO;
            while (currentState != State.DONE) {
                String incomingLine = getNontrivialSMTP(fromClient, toClient);
                if (incomingLine == null) break;
                switch (currentState) {
                    case WEHLO     : doWEHLO (incomingLine, toClient);     break;
                    case WMAIL     : doWMAIL (incomingLine, toClient);     break;
                    case WRCPT1    : doWRCPT1(incomingLine, toClient);     break;
                    case WRCPT2    : doWRCPT2(incomingLine, toClient);     break;
                    case GETMESSAGE: doGETMESSAGE(incomingLine, toClient); break;
                    case WQUIT     : doWQUIT (incomingLine, toClient);     break;
                    case DONE      : break;
                }
            }
            clientSocket.close();
        }
        catch (Exception ex) {
            StackTraceElement[] traceElements = ex.getStackTrace();
            StackTraceElement topElement = traceElements[0];
            logger.log(
                    "Unhandled exception in ServerConnection: " + ex.getClass() +
                            ", message = " + ex.getMessage());
            logger.log(
                    "\tLocation: File = " + topElement.getFileName() +
                            ", Line = " + topElement.getLineNumber());
        }
    }
}
