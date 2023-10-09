

// Directory.cpp

#include <string>
#include "Directory.h"

storage::NodeList DirectoryI::list(const Ice::Current &)
{
    storage::NodeList directory_contents;  // Just empty for now.
    return directory_contents;
}

std::string DirectoryI::name(const Ice::Current &)
{
    return physical_directory;
}
