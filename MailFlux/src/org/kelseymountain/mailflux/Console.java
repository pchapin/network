/*
 * Copyright (C) 2025 Peter Chapin
 *
 * Licensed under the Apache v. 2 License. See the LICENSE file in the project root for details.
 *
 * File:    Console.java
 * Purpose: Class for managing the console.
 * Author:  Peter Chapin <spicacality@kelseymountain.org>
 */
package org.kelseymountain.mailflux;


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
