using Filesystem;
using System;

public class FileI : FileDisp_ {
    public static Ice.ObjectAdapter _adapter;
    private string _name;
    private DirectoryI _parent;
    private string[] _lines;
    
    public FileI(string name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;
        // Debug.Assert(_parent != null);
        
        // Create an identity
        Ice.Identity myID = Ice.Util.stringToIdentity(Ice.Util.generateUUID());
        
        // Add the identity to the object adapter
        _adapter.add(this, myID);
        
        // Create a proxy for the new node and add it as a child to the parent
        NodePrx thisNode = NodePrxHelper.uncheckedCast(_adapter.createProxy(myID));
        _parent.addChild(thisNode);
    }
    
    public override string name(Ice.Current current)
    {
        return _name;
    }

    public override string[] read(Ice.Current current)
    {
        return _lines;
    }

    public override void write(string[] text, Ice.Current current)
    {
        _lines = text;
    }
}

