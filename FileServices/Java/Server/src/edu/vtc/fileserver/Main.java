package edu.vtc.fileserver;

import com.zeroc.Ice.*;
import com.zeroc.Ice.Exception;

public class Main {

    /**
     * The main method initializes Ice and creates an instance of an object that does the work of the a file server.
     *
     * @param args Command line arguments.
     */
    public static void main(String[] args)
    {
        int status = 0;
        Communicator ic = null;
        try {
            ic = Util.initialize(args);
            ObjectAdapter adapter = ic.createObjectAdapterWithEndpoints("FileServerAdapter", "default -p 9100");
            FileSystemI object = new FileSystemI(adapter);
            adapter.add(object, Util.stringToIdentity("PrimaryFileSystem"));
            adapter.activate();
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
