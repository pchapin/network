

// server.cpp

#include <Ice/Ice.h>
#include "Storage.h"
#include "FileSystem.h"

using namespace std;
// using namespace storage;

int main(int argc, char* argv[])
{
  int status = 0;
  Ice::CommunicatorPtr ic;
  try {
    ic = Ice::initialize(argc, argv);
    Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints(
      "ChatterAdapter", "tcp -p 9020");
    Ice::ObjectPtr object = new FileSystemI(adapter, ".");
    adapter->add(object, ic->stringToIdentity("Main File Server"));
    adapter->activate();
    ic->waitForShutdown();
  }
  catch (const Ice::Exception &e) {
    cerr << e << endl;
    status = 1;
  }
  catch (const char *msg) {
    cerr << msg << endl;
    status = 1;
  }

  // We must call ic->destroy() even if an exception is throw above.
  if (ic) {
    try {
      ic->destroy();
    }
    catch (const Ice::Exception &e) {
      cerr << e << endl;
      status = 1;
    }
  }
  return status;
}
