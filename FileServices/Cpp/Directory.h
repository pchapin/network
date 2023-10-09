

// Directory.h

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <Ice/Ice.h>
#include "Storage.h"

class DirectoryI : virtual public storage::Directory {
public:
    DirectoryI(const Ice::ObjectAdapterPtr &my_adapter,
               const std::string &path): adapter(my_adapter), physical_directory(path) { }

    virtual
    storage::NodeList list(const Ice::Current &);

    virtual
    std::string name(const Ice::Current &);

private:
    Ice::ObjectAdapterPtr adapter;
    std::string           physical_directory;
};

#endif
