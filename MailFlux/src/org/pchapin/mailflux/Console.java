//-----------------------------------------------------------------------
// FILE    : Console.java
// SUBJECT : Class for managing the console.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;


/**
 * Instances of this class manage a terminal-style console onto which messages can be displayed.
 * This class supports access from multiple threads simultaneously.
 */
public class Console implements Logger {

    /**
     * Displays the given message on the console. If multiple threads calls this method
     * simultaneously (on the same object), the messages will be displayed in a non-
     * deterministic order, but they will not corrupt each other.
     *
     * @param message The message to be displayed.
     */
    public synchronized void log(String message)
    {
        // TODO: Log messages in a useful way.
        System.out.println(message);
    }
}
