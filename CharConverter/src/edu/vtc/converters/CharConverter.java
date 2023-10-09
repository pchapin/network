package edu.vtc.converters;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;

/**
 * The main class of a useful character conversion utility.
 */
public class CharConverter {

    private static final String[] validEncodingNames =
            new String[]{"US-ASCII", "ISO-8859-1", "UTF-8", "UTF-16BE", "UTF-16LE", "UTF-16"};


    /**
     * Checks to be sure the given name is the name of a supported encoding.
     * @param name The name (of an encoding) to check.
     * @return True if the encoding is supported.
     */
    private static boolean validEncoding(String name)
    {
        boolean found = false;
        for (String s : validEncodingNames) {
            if (name.equals(s)) found = true;
        }
        return found;
    }


    public static void main(String[] args) throws IOException
    {
        if (args.length != 2) {
            System.out.println("Usage: CharConverter input.txt:encoding output.txt:encoding");
        }
        else {
            int firstSplitPoint = args[0].indexOf(':');
            int secondSplitPoint = args[1].indexOf(':');

            if (firstSplitPoint == -1 || secondSplitPoint == -1) {
                System.out.println("Invalid format in either " + args[0] + " or " + args[1]);
            }
            else {
                String inputFileName  = args[0].substring(0, firstSplitPoint);
                String inputEncoding  = args[0].substring(firstSplitPoint + 1);
                String outputFileName = args[1].substring(0, secondSplitPoint);
                String outputEncoding = args[1].substring(secondSplitPoint + 1);

                // Make sure the encoding names are valid (in validEncodingNames).
                if (!validEncoding(inputEncoding) || !validEncoding(outputEncoding)) {
                    System.out.println("Invalid encoding in either " + inputEncoding + " or " + outputEncoding);
                }
                else {

                    // Set up input decoding...
                    CharsetDecoder inputDecoder =
                            Charset.forName(inputEncoding).newDecoder();
                    InputStreamReader inputReader =
                            new InputStreamReader(new FileInputStream(inputFileName), inputDecoder);
                    BufferedReader inputBufferedReader =
                            new BufferedReader(inputReader);

                    // Set up output encoding...
                    CharsetEncoder outputEncoder =
                            Charset.forName(outputEncoding).newEncoder();
                    OutputStreamWriter outputWriter =
                            new OutputStreamWriter(new FileOutputStream(outputFileName), outputEncoder);
                    BufferedWriter outputBufferedWriter =
                            new BufferedWriter(outputWriter);

                    // Read input file one character at a time and write to the output file...
                    int ch;
                    while ((ch = inputBufferedReader.read()) != -1) {
                        outputBufferedWriter.write(ch);
                    }

                    // TODO: Make sure these files get closed properly if an exception is thrown.
                    inputBufferedReader.close();
                    outputBufferedWriter.close();
                }
            }
        }
    }
}
