package edu.vtc.fileserver;

import java.util.ArrayList;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.ObjectPrx;
import storage.*;

public class DirectoryWrapper implements Directory {

    private ObjectAdapter objectAdapter;
    private String physicalPath;


    public DirectoryWrapper(ObjectAdapter objectAdapter, String physicalPath)
    {
        this.objectAdapter = objectAdapter;
        this.physicalPath = physicalPath;
    }

    @Override
    public void destroy(Current current)
    {
        objectAdapter.remove(current.id);
    }


    @Override
    public String name(Current current)
    {
        if (physicalPath.equals("ROOT")) {
            return "/";
        }
        else {
            int lastSeparator = physicalPath.lastIndexOf(java.io.File.separatorChar);
            return physicalPath.substring(lastSeparator + 1);
        }
    }


    @Override
    public NodeType getType(Current current)
    {
        return NodeType.DirectoryNode;
    }


    @Override
    public NodePrx[] list(Current current)
    {
        ArrayList<NodePrx> nodes = new ArrayList<>();

        java.io.File directoryFile = new java.io.File(physicalPath);
        // TODO: listFiles() could return null if directoryFile is not a directory or if an I/O error occurs.
        for (java.io.File directoryEntry : directoryFile.listFiles()) {

            // Deal with file nodes.
            if (directoryEntry.isFile()) {
                FileWrapper subFile =
                        new FileWrapper(objectAdapter, directoryEntry.getAbsolutePath());
                ObjectPrx newObject = objectAdapter.addWithUUID(subFile);
                nodes.add(NodePrx.checkedCast(newObject));
            }

            // Deal with directory nodes.
            if (directoryEntry.isDirectory()) {
                DirectoryWrapper subDirectory =
                        new DirectoryWrapper(objectAdapter, directoryEntry.getAbsolutePath());
                ObjectPrx newObject = objectAdapter.addWithUUID(subDirectory);
                nodes.add(NodePrx.checkedCast(newObject));
            }
        }
        return nodes.toArray(new NodePrx[1]);
    }
}
