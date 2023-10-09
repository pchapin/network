package edu.vtc.fileserver;

import com.zeroc.Ice.Current;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.ObjectPrx;
import storage.*;

public class FileSystemI implements FileSystem {

    private ObjectAdapter objectAdapter;

    public FileSystemI(ObjectAdapter objectAdapter)
    {
        this.objectAdapter = objectAdapter;
    }

    @Override
    public DirectoryPrx getRootDirectory(Current current)
    {
        DirectoryWrapper root = new DirectoryWrapper(objectAdapter, "ROOT");
        ObjectPrx newObject = objectAdapter.addWithUUID(root);
        return DirectoryPrx.checkedCast(newObject);
    }

}
