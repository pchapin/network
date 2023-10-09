
// Storage.ice

module storage {

    enum NodeType { FileNode, DirectoryNode };

    // Nodes can be either files or directories. All nodes have a name.
    interface Node {
        idempotent string name();
        idempotent NodeType getType();
        void destroy();
    };

    sequence<byte> BlockType;

    // Files can be read sequentially. Only a single reader is allowed.
    interface File extends Node {
        bool open();
        BlockType read(int requestedSize);
        void close();
    };

    sequence<Node*> NodeList;

    // A directory provides access to the nodes it contains.
    interface Directory extends Node {
        idempotent NodeList list();
    };

    interface FileSystem {
        idempotent Directory *getRootDirectory();
    };
};
