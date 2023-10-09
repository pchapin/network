package edu.vtc.fileclient;

import com.zeroc.Ice.*;
import com.zeroc.Ice.Exception;
import storage.FileSystemPrx;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import storage.*;

public class Main {
    /**
     * The program entry point.
     *
     * @param args Command line arguments.
     */
    public static void main(String[] args)
        throws java.io.IOException
    {
        int status = 0;

        Communicator ic = null;
        try {
            ic = Util.initialize(args);

            // Get a proxy to the "well known" file system.
            // TODO: The location of the file system should be configurable.
            ObjectPrx rawFileSystem = ic.stringToProxy("PrimaryFileSystem:tcp -h localhost -p 9100");
            FileSystemPrx fs = FileSystemPrx.checkedCast(rawFileSystem);

            // TODO: Use a better exception. This is an abuse of IllegalArgumentException.
            if (fs == null) throw new IllegalArgumentException("Invalid File System");

            boolean done = false;
            BufferedReader consoleInput = new BufferedReader(new InputStreamReader(System.in));
            DirectoryPrx currentDirectory = fs.getRootDirectory();
            while (!done) {
                System.out.print("$ ");
                String line = consoleInput.readLine();
                String[] command = line.split("\\s+");

                switch (command[0]) {
                    case "cat":
                        // TODO: Add some error handling.
                        // TODO: Probably the directory interface should have a method for looking up file names.
                        for (NodePrx node : currentDirectory.list()) {
                            if (command[1].equals(node.name()) && node.getType() == NodeType.FileNode) {
                                FilePrx file = FilePrx.checkedCast(node);
                                if (!file.open()) {
                                    System.out.println("Error opening file " + command[1]);
                                }
                                else {
                                    byte[] block = file.read(512);
                                    while (block.length != 0) {
                                        // TODO: Output the block.
                                        block = file.read(512);
                                    }
                                    file.close();
                                }
                            }
                        }
                        break;

                    case "cd":
                        // TODO: Add some error handling.
                        // TODO: Probably the directory interface should have a method for looking up directory names.
                        for (NodePrx node : currentDirectory.list()) {
                            if (command[1].equals(node.name()) && node.getType() == NodeType.DirectoryNode) {
                                currentDirectory = DirectoryPrx.checkedCast(node);
                            }
                        }
                        break;

                    case "ls":
                        for (NodePrx node : currentDirectory.list()) {
                            System.out.println(node.name());
                        }
                        break;

                    case "quit":
                        done = true;
                        break;

                    default:
                        System.out.println("Unknown command: " + command[0]);
                        break;
                }
            }

            ic.waitForShutdown();
        }
        catch (LocalException e) {
            e.printStackTrace();
            status = 1;
        }
        catch (Exception e) {
            System.err.println(e.getMessage());
            status = 1;
        }
        if (ic != null) {
            // Clean up
            try {
                ic.destroy();
            }
            catch (Exception e) {
                System.err.println(e.getMessage());
                status = 1;
            }
        }
        System.exit(status);
    }
}
