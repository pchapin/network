using System;
using Filesystem;

public class Client {
    
    // Recursively print the contents of directory "dir" in tree fashion.
    // For files, show the contents of each file. The "depth" parameter is
    // the current nesting level (for indentation).
    //
    static void listRecursive(DirectoryPrx dir, int depth)
    {
        string indent = new string('\t', ++depth);
        NodePrx[] contents = dir.list();
        foreach (NodePrx node in contents) {
            DirectoryPrx subdir = DirectoryPrxHelper.checkedCast(node);
            FilePrx file = FilePrxHelper.uncheckedCast(node);
            Console.WriteLine(indent + node.name() +
                              (subdir != null ? " (directory):" : " (file):"));
            if (subdir != null) {
                listRecursive(subdir, depth);
            }
            else {
                string[] text = file.read();
                for (int j = 0; j < text.Length; ++j)
                    Console.WriteLine(indent + "\t" + text[j]);
            }
        }
    }

    public static void Main(string[] args)
    {
        int status = 0;
        Ice.Communicator ic = null;
        try {
            // Create a communicator
            ic = Ice.Util.initialize(ref args);

            // Create a proxy for the root directory
            Ice.ObjectPrx obj = ic.stringToProxy("RootDir:default -p 10000");

            // Down-cast the proxy to a Directory proxy
            DirectoryPrx rootDir = DirectoryPrxHelper.checkedCast(obj);

            // Recursively list the contents of the root directory
            Console.WriteLine("Contents of root directory:");
            listRecursive(rootDir, 0);
        }
        catch (Exception e) {
            Console.Error.WriteLine(e);
            status = 1;
        }

        if (ic != null) {
            // Clean up
            try {
                ic.destroy();
            }
            catch (Exception e) {
                Console.Error.WriteLine(e);
                status = 1;
            }
        }
        Environment.Exit(status);
    }
}
