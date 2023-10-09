package edu.vtc.fileserver;

import com.zeroc.Ice.Current;
import com.zeroc.Ice.ObjectAdapter;
import storage.*;

public class FileWrapper implements File {

    private ObjectAdapter objectAdapter;
    private String physicalPath;


    public FileWrapper(ObjectAdapter objectAdapter, String physicalPath)
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
        int lastSeparator = physicalPath.lastIndexOf(java.io.File.separatorChar);
        return physicalPath.substring(lastSeparator + 1);
    }


    @Override
    public NodeType getType(Current current)
    {
        return NodeType.FileNode;
    }


    @Override
    public boolean open(Current current)
    {
        return false;
    }


    @Override
    public byte[] read(int requestedSize, Current current)
    {
        return null;
    }


    @Override
    public void close(Current current)
    {

    }

}
