//-----------------------------------------------------------------------
// FILE    : Spool.java
// SUBJECT : Class that represents a message spool.
// AUTHOR  : (C) Copyright 2013 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

/**
 * Class that represents a non-volatile storage area where messages can be stored.
 */
public class Spool {

    private Logger logger;

    /**
     * Initializes a spool area.
     *
     * @param logger The logger object used by this spool to report error messages.
     */
    public Spool(Logger logger)
    {
        this.logger = logger;
    }


    /**
     * Adds a new message to the spool area.
     *
     * @param newMessage The message to add.
     */
    public void addMessage(Message newMessage)
    {
        // Compute a unique name for the spool file.
        Date now = new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMdd'T'HHmmss");
        String filename = "spool" + File.separator + formatter.format(now) + ".msg";

        // Open the file and write to it.
        try {
            BufferedWriter output =
                    new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filename)));
            String sender = newMessage.getSender();
            ArrayList<String> recipients = newMessage.getRecipients();
            ArrayList<String> message = newMessage.getText();
            try {
                output.write(sender + "\n");
                output.write("=====\n");
                for (String recipient : recipients) {
                    output.write(recipient + "\n");
                }
                output.write("=====\n");
                for (String line : message) {
                    output.write(line + "\n");
                }
            }
            // Deal with errors during writing.
            catch (IOException ioException) {
                logger.log("Error writing spool file: " + filename + ". Message not spooled!");
            }
            finally {
                output.flush();  // In theory close() does this. In practice it seems like maybe it doesn't.
                output.close();
            }
        }
        // Deal with errors during file opening and closing.
        catch (FileNotFoundException fileNotFound) {
            logger.log("Unable to open spool file: " + filename + ". Message not spooled!");
        }
        catch (IOException ioException) {
            logger.log("Unable to close spool file: " + filename + ". Message may not be spooled correctly");
        }
    }
}
