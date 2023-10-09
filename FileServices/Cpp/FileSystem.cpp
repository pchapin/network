

// FileSystem.cpp

#include "Directory.h"
#include "FileSystem.h"

storage::DirectoryPrx FileSystemI::getRootDirectory(const Ice::Current &)
{
    storage::DirectoryPtr resultPtr = new DirectoryI(adapter, local_root);
    Ice::ObjectPrx resultPrx = adapter->addWithUUID(resultPtr);
    return storage::DirectoryPrx::checkedCast(resultPrx);
}
