using Filesystem;
using System;
using System.Collections;

public class DirectoryI : DirectoryDisp_ {

    public static Ice.ObjectAdapter _adapter;
    private string     _name;
    private DirectoryI _parent;
    private ArrayList  _contents = new ArrayList();

    public DirectoryI(string name, DirectoryI parent)
    {
        _name = name;
        _parent = parent;
        
        // Create an identity. The parent has the fixed identity "RootDir"
        Ice.Identity myID = Ice.Util.stringToIdentity(
            _parent != null ? Ice.Util.generateUUID() : "RootDir");
        
        // Add the identity to the object adapter
        _adapter.add(this, myID);
        
        // Create a proxy for the new node and add it as a child to the parent
        NodePrx thisNode = NodePrxHelper.uncheckedCast(_adapter.createProxy(myID));
        if (_parent != null)
            _parent.addChild(thisNode);
    }
    
    public void addChild(NodePrx child)
    {
        _contents.Add(child);
    }

    public override string name(Ice.Current current)
    {
        return _name;
    }

    public override NodePrx[] list(Ice.Current current)
    {
        return (NodePrx[])_contents.ToArray(typeof(NodePrx));
    }
}

