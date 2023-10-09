

// FileSystem.h

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <Ice/Ice.h>
#include "Storage.h"

class FileSystemI : virtual public storage::FileSystem {
public:
    FileSystemI(const Ice::ObjectAdapterPtr &my_adapter,
                const std::string &root_path): adapter(my_adapter), local_root(root_path) { }

    virtual
    storage::DirectoryPrx getRootDirectory(const Ice::Current &);

private:
    Ice::ObjectAdapterPtr adapter;
    std::string           local_root;
};

#endif
