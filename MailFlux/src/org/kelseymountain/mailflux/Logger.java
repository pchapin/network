/*
 * Copyright (C) 2025 Peter Chapin
 *
 * Licensed under the Apache v. 2 License. See the LICENSE file in the project root for details.
 *
 * File:    Logger.java
 * Purpose: Interface to objects that can log messages.
 * Author:  Peter Chapin <spicacality@kelseymountain.org>
 */
package org.kelseymountain.mailflux;

/**
 * Classes implementing this interface can be used to log messages. Potentially other kinds of
 * information could be logged through this interface in the future. The precise format of
 * logged messages and precisely where the messages are logged is unspecified.
 */
public interface Logger {

    /**
     * Log a message. The message is intended to be a single line of text. The effect of
     * including embedded line breaks in the message is unspecified.
     *
     * @param message The line of text to log. Do not include a trailing '\n'
     */
    public void log(String message);
}
