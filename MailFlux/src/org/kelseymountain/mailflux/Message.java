/*
 * Copyright (C) 2025 Peter Chapin
 *
 * Licensed under the Apache v. 2 License. See the LICENSE file in the project root for details.
 *
 * File:    Message.java
 * Purpose: Class that represents a single message.
 * Author:  Peter Chapin <spicacality@kelseymountain.org>
 */
package org.kelseymountain.mailflux;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Class that represents a single message. MTAs regard messages as blobs of text but they do
 * have an associated sender and recipient list (that might not be related to the From or To
 * header fields in the message itself). This class allows messages to be treated abstractly.
 */
public class Message {

    private String sender = "";
    private final ArrayList<String> recipients = new ArrayList<String>();
    private final ArrayList<String> text = new ArrayList<String>();

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
        for (String currentRecipient : recipients) {
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

    /**
     * Gets the SMTP sender address associated with this message.
     * TODO: Review Javadoc
     *
     * @return The sender address as a string.
     */
    public String getSender()                { return sender; }

    /**
     * Gets the list of SMTP recipient addresses for this message.
     * TODO: Review Javadoc
     *
     * @return A mutable list of recipient addresses in the order they were added.
     */
    public ArrayList<String> getRecipients() { return recipients; }

    /**
     * Gets the message body as a list of lines.
     * TODO: Review Javadoc
     *
     * @return A mutable list of lines that comprise the message text.
     */
    public ArrayList<String> getText()       { return text; }
}
