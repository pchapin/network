/*****************************************************************************
FILE          : client.cpp
PROGRAMMER    : Peter Chapin
LAST REVISION : 2003-02-01

(C) Copyright 2003 by Peter Chapin

This file contains a simple demonstration client for the FileServices
module. It exercies the methods of FileServices::FileServer and of
FileServices::File.

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
*****************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "file_services.h"

//
// The following function reads the named file from the given file server.
//
void read_file(const std::string &name, FileServices::FileServer_var &fs)
{
  // Open the file.
  FileServices::File_var the_file =
    fs->open(name.c_str(), FileServices::READ);

  // Keep reading the file until there is nothing left. Print what I get.
  while (1) {
    FileServices::File::DataBlockType_var data = the_file->read(1024);
    if (data->length() == 0) break;
    for (CORBA::ULong i = 0; i < data->length(); i++) {
      std::cout << data[i];
    }
  }

  // Clean up.
  std::cout << "\n\ndone!\n";
  the_file->close();    
}


//
// The following function writes to the named file on the given file server.
//
void write_file(const std::string &name, FileServices::FileServer_var &fs)
{
  // Open the file.
  FileServices::File_var the_file = 
    fs->open(name.c_str(), FileServices::WRITE);

  // Create an object to hold the data I'm going to send to the file.
  FileServices::File::DataBlockType data;

  // This is lame, but it will do for the moment.
  data.length(5);
  data[0] = 'H';
  data[1] = 'e';
  data[2] = 'l';
  data[3] = 'l';
  data[4] = 'o';
  the_file->write(data);

  // Clean up.
  std::cout << "\n\ndone!\n";
  the_file->close();
}


//
// Main Program
//
// The ORB is initialized and a reference to a suitable file server is
// obtained. The user is then prompted for the desired action. Probably
// it would be better to take the user's commands from the command line.
//
int main(int argc, char **argv)
{
  try {
    // Check arguments.
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " IOR-file-name" << std::endl;
      return 1;
    }

    // Initialize ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Destringify IOR. Should get this from the naming service.
    std::ifstream IOR_file(argv[1]);
    if (!IOR_file) {
      std::cerr << "Can't open IOR file!" << std::endl;
      return 1;
    }
    std::string IOR;
    IOR_file >> IOR;
    IOR_file.close();
    CORBA::Object_var obj = orb->string_to_object(IOR.c_str());
    if (CORBA::is_nil(obj)) {
      std::cerr << "NIL FileServer reference" << std::endl;
      return 1;
    }

    // Narrow.
    FileServices::FileServer_var fs = FileServices::FileServer::_narrow(obj);
    if (CORBA::is_nil(obj)) {
      std::cerr << "Argument is not a FileServer reference" << std::endl;
      return 1;
    }

    // Play with fs.
    int choice;
    std::string file_name;
    std::cout << "\n0) Exit\n1) Read\n2) Write\n\nChoice? ";
    std::cin  >> choice;
    std::cout << "File name? ";
    std::cin  >> file_name;

    // The switch is overkill, but in the future there may be other cases.
    switch (choice) {
      case 0:
        break;

      case 1:
        read_file(file_name, fs);
        break;

      case 2:
        write_file(file_name, fs);
        break;
    }

    // Clean up the ORB.
    orb->destroy();
  }
  catch (const CORBA::Exception &) {
    std::cerr << "Uncaught CORBA exception" << std::endl;
    return 1;
  }
  catch (...) {
    std::cerr << "Uncaught exception of unknonwn type" << std::endl;
    return 1;
  }

  return 0;
}
