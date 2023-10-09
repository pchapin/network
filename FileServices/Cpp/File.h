

// File.h

#ifndef FILE_H
#define FILE_H

#include <string>
#include <Ice/Ice.h>
#include "Storage.h"

class FileI : virtual public storage::File {
public:
    FileI(const std::string &path): physical_name(path) { }

    virtual
    storage::Block read(int requestedSize, const Ice::Current &);

    virtual
    std::string name(const Ice::Current &);

private:
    std::string physical_name;
};

#endif
