/*****************************************************************************
FILE          : server.cpp
PROGRAMMER    : Peter Chapin
LAST REVISION : 2003-02-01

(C) Copyright 2003 by Peter Chapin

This file contains the implementation of the FileServices module (in
particular, it implements FileServices::FileServer and associated
items. For todo items see the comments below.

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
*****************************************************************************/

#include <iostream>
#include <fstream>
#include "file_services.h"

#pragma warning(disable:4290)

namespace FileServices {

  // +++++++++++++++++++++++++
  // FileServer Implementation
  // +++++++++++++++++++++++++
  //
  // Servant class for the file server.
  //
  class FileServer_impl : public virtual POA_FileServices::FileServer {
    public:

      virtual File_ptr open(const char *name, OpenMode mode)
        throw (CORBA::SystemException);
  };


  // +++++++++++++++++++
  // File Implementation
  // +++++++++++++++++++
  //
  // Servant class for the open files. It might make more sense to have
  // two separate implementation classes: one for input files and one
  // for output files. Using two different classes to support a single
  // CORBA interface is not a problem. In any event, I should probably
  // throw reasonable exeptions if the client attempts to read from an
  // output file or write to an input file. This version just attempts
  // to use an unopened file object!
  //
  // Notice also that I close the files at destruction time.
  //
  class File_impl : public virtual POA_FileServices::File,
                    public virtual PortableServer::RefCountServantBase {
    public:

     File_impl(const char *name, OpenMode mode);

      virtual FileServices::File::DataBlockType *read(CORBA::Short size)
        throw(CORBA::SystemException);

      virtual CORBA::Short write(const FileServices::File::DataBlockType &data)
        throw (CORBA::SystemException);

      virtual void close()
        throw (CORBA::SystemException);

    private:
      std::ifstream infile;
      std::ofstream outfile;
  };


  // ++++++++++++++++++++++++++
  // Methods of FileServer_impl
  // ++++++++++++++++++++++++++

  // Opening files. I believe I should call _remove_ref() here so that
  // the only reference to the File_impl object is held by the POA. In
  // any case I pass off the grunt work of opening the file to the
  // File_impl object.
  // 
  File_ptr FileServer_impl::open(const char *name, OpenMode mode)
    throw (CORBA::SystemException)
  {
    // I should sanitize the name for security purposes. Maybe later.
    File_impl *my_file = new File_impl(name, mode);
    File_ptr return_value = my_file->_this();
    my_file->_remove_ref();
    return return_value;
  }


  // ++++++++++++++++++++
  // Methods of File_impl
  // ++++++++++++++++++++

  // Open the file at construction time.
  // 
  File_impl::File_impl(const char *name, OpenMode mode)
  {
    switch (mode) {
      case READ:
        infile.open(name, std::ios::in | std::ios::binary);
        break;

      case WRITE:
        outfile.open(name, std::ios::out | std::ios::binary);
        break;
    }
  }


  // Deactivate the object when it is closed. The POA will delete it.
  // 
  void File_impl::close()
    throw (CORBA::SystemException)
  {
    // Now deactivate myself.
    PortableServer::POA_var my_poa = _default_POA();
    PortableServer::ObjectId_var id = my_poa->servant_to_id(this);
    my_poa->deactivate_object(id);
  }


  // Reading files. This version doesn't clean up properly if an
  // exception is thrown.
  //
  File::DataBlockType *File_impl::read(CORBA::Short size)
    throw(CORBA::SystemException)
  {
    if (size > 1024) size = 1024;

    // Allocate a buffer to hold the file's data.
    char *workspace = new char[size];

    // Read the file and remember how much was actually read.
    infile.read(workspace, size);
    int count = infile.gcount();

    // Create the outgoing object and copy the data into it.
    FileServices::File::DataBlockType *result =
      new FileServices::File::DataBlockType;
    result->length(count);
    for (int i = 0; i < count; i++) {
      (*result)[i] = workspace[i];
    }
    
    // Clean up. This will leak memory if the above throws.
    delete [] workspace;

    return result;
  }


  // Writing files. I should definitely check for errors here and return
  // an appropriate value if I encounter one.
  //
  CORBA::Short File_impl::write(const FileServices::File::DataBlockType &data)
    throw (CORBA::SystemException)
  {
    for (CORBA::ULong i = 0; i < data.length(); ++i) {
      outfile.put(data[i]);
    }
    return static_cast<CORBA::Short>(data.length());
  }

}

//
// Main Program
//
//   1. Initialize an ORB.
//   2. Create a FileServer servant.
//   3. Publish the servant's object reference.
//
int main(int argc, char **argv)
{
  try {
    // Initialize ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get reference to root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    
    // Activate POA manager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();
    
    // Create a servant object.
    FileServices::FileServer_impl my_server;
    
    // Write its stringified reference to a file.
    FileServices::FileServer_var fs = my_server._this();
    CORBA::String_var str = orb->object_to_string(fs);
    std::ofstream IOR_file("IOR.txt");
    if (IOR_file) IOR_file << str << std::endl;
    IOR_file.close();
    
    // Accept requests.
    orb->run();

    // Clean up afterwards.
    orb->destroy();
  }
  catch (const CORBA::Exception &) {
    std::cerr << "Uncaught CORBA exception" << std::endl;
    return 1;
  }
  
  return 0;
}
