//-----------------------------------------------------------------------
// FILE    : Logger.java
// SUBJECT : Interface to objects that can log messages.
// AUTHOR  : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>
//
//-----------------------------------------------------------------------
package org.pchapin.mailflux;

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
