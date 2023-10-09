//-----------------------------------------------------------------------
// FILE    : Message.java
// SUBJECT : Class that represents a single message.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Class that represents a single message. MTAs regard messages as blobs of text but they do
 * have an associated sender and recipient list (that might not be related to the From or To
 * header fields in the message itself). This class allows messages to be treated abstractly.
 */
public class Message {
    
    private String sender = "";
    private ArrayList<String> recipients = new ArrayList<String>();
    private ArrayList<String> text = new ArrayList<String>();

    /**
     * Sets the SMTP sender of the message. Each message can have at most one sender. If
     * setSender is called when a sender has already been set, the address passed to the new
     * invocation will overwrite the previous address.
     *
     * @param senderAddress The email address of the sender.
     */
    public void setSender(String senderAddress)
    {
      sender = senderAddress;
    }

    /**
     * Adds a new recipient to the message's recipient list. Each message can have multiple
     * recipients. This class does not provide a way to remove recipients. If the new recipient
     * is already on the message's recipient list, there is no effect. In particular recipients
     * are not added multiple times.
     *
     * @param recipientAddress The email address of a new recipient.
     */
    public void addRecipient(String recipientAddress)
    {
        // Is this recipient already on the list?
        Iterator<String> it = recipients.iterator();
        while (it.hasNext()) {
            String currentRecipient = it.next();
            if (currentRecipient.equalsIgnoreCase(recipientAddress)) return;
        }
        
        // Apparently not. In that case add this new one.
        recipients.add(recipientAddress);
    }

    /**
     * Adds a line of message text to the end of this message's body. The line should not
     * contain any line termination characters (CR/LF, etc) and should be composed of only
     * US-ASCII text. The structure of the message is not checked.
     *
     * @param line The line to add at the end of the message body.
     */
    public void appendText(String line)
    {
        text.add(line);
    }
    
    // ----------------
    // Accessor methods
    // ----------------
    
    public String getSender()                { return sender; }
    public ArrayList<String> getRecipients() { return recipients; }
    public ArrayList<String> getText()       { return text; }
}
