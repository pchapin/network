

// File.cpp

#include <string>
#include "File.h"

storage::Block FileI::read(int requstedSize, const Ice::Current &)
{
    storage::Block file_contents;  // Just empty for now.
    return file_contents;
}

std::string FileI::name(const Ice::Current &)
{
    return physical_name;
}
