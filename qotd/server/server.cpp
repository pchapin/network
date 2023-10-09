/*****************************************************************************
FILE          : qotdd.cpp
PROGRAMMER    : (C) Copyright 2013 by Peter C. Chapin
LAST REVISION : 2013-02-02
SUBJECT       : A concurrent quote of the day server.


This file contains a concurrent "quote of the day" server. This version reads a file of cute
quotes.

The format of the quotes file is very simple. It is a plain text file with each quote separated
by one or more blank lines. Note that this version of the program reads the entire quotes file
into memory. While this is fine for a relatively small quotes file, it isn't a great idea when
the quotes file is large.

A future version of this program should probably process SIGHUP by rereading the quotes file
each time a SIGHUP is received. A future version should also probably take the name of the
quotes file from the command line, daemonize itself properly, and generally allow for more
configuration options.

Please send comments or bug reports to

     Peter C. Chapin
     Computer Information Systems Department
     Vermont Technical College
     Williston, VT 05495
     PChapin@vtc.vsc.edu
*****************************************************************************/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128
#define PID_PATH    "../../qotdd.pid"
#define QUOTES_PATH "../../qotdd.txt"

std::vector<std::string> quotes;

//
// This function handles the SIGCHLD signal by cleaning up (and throwing away) the exit status
// of any currently dead children.
//
void SIGCHLD_handler(int)
{
  int status;

  while (waitpid(-1, &status, WNOHANG) > 0) ;
  return;
}

//
// This function creates a pid file for this process.
//
void create_pidfile()
{
  std::ofstream outfile(PID_PATH);

  if (!outfile) return;
  outfile << getpid() << "\n";
}


//
// This function erases the pid file. It should be called when this program terminates, but
// currently this program never terminates (cleanly).
//
void remove_pidfile()
{
  remove(PID_PATH);
}


//
// This function reads the quotes from disk into the vector 'quotes'. Storing all the quotes in
// memory is probably not great, but it will be fine until the quote database becomes large.
//
void read_quotes()
{
  std::ifstream infile(QUOTES_PATH);
  std::string   line_buffer;
  std::string   current_quote;
  bool          in_quote = false;

  // If the quotes file failed to open install an error message as a quote.
  if (!infile) {
    quotes.push_back("Unable to open quotes file!\r\n");
    return;
  }

  while (getline(infile, line_buffer)) {

    // Is this a blank line?
    bool is_blank = true;
    for (std::string::size_type i = 0; i < line_buffer.size(); ++i) {
      if (line_buffer[i] != ' ' && line_buffer[i] != '\t') {
        is_blank = false;
        break;
      }
    }

    if (is_blank && in_quote) {

      // Close off current_quote.
      quotes.push_back(current_quote);
      in_quote = false;
    }
    else if (!is_blank && in_quote) {

      // Append this line to the current quote.
      current_quote.append(line_buffer);
      current_quote.append("\r\n");
    }
    else if (is_blank && !in_quote) {

      // Ignore it.
    }
    else if (!is_blank && !in_quote) {

      // This is the start of a quote.
      current_quote = line_buffer;
      current_quote.append("\r\n");
      in_quote = true;
    }

  }

  // Handle the last quote special... there might not be any blanks after it.
  if (in_quote) quotes.push_back(current_quote);

  #ifdef DEBUG
  printf("Read %lu quotes.\n\n", quotes.size());
  fflush(stdout);
  #endif
}


//
// Main Program
//
int main(int argc, char **argv)
{
  int                 listen_handle;     // Socket handle to listen on.
  int                 connection_handle; // Socket handle for each connection.
  struct sockaddr_in  server_address;    // Our address.
  struct sockaddr_in  client_address;    // Remote address.
  socklen_t           client_length;     // Size of remote address.
  unsigned short      port = 17;         // Port number to listen on.
  #ifdef DEBUG
  char                buffer[BUFFER_SIZE];  // Buffer for results.
  #endif
  struct sigaction    action;            // Disposition of SIGCHLD.
  struct sigaction    old_action;        // Old disposition of SIGCHLD.
  pid_t               child_ID;          // Process ID of child.

  // Do we have an explicit port address? If so, use it.
  if (argc == 2) {
    port = atoi(argv[1]);
  }

  // Get the witty tag lines and put them all in memory.
  read_quotes();

  // Prepare the random number generator.
  srand(time(NULL));

  // Set up the signal handling.
  action.sa_handler = SIGCHLD_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGCHLD, &action, &old_action);


  // Create the server socket.
  if ((listen_handle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Problem creating socket");
    return 1;
  }

  // Prepare the server socket address structure.
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family      = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port        = htons(port);

  // Bind the server socket.
  if (bind(listen_handle,
        (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    perror("Problem binding");
    return 1;
  }

  // Allow incoming connections.
  if (listen(listen_handle, 32) < 0) {
    perror("Problem listening");
    return 1;
  }

  // It looks like I'm going to run okay. Create pid file.
  create_pidfile();

  // Accept connections forever; create a child process to handle each.
  while (1) {
    memset(&client_address, 0, sizeof(client_address));
    client_length = sizeof(client_address);

    // Block until a client comes along. If accept() returns due to a signal just call it again.
    // The only signal we should be getting is SIGCHLD.
    //
    while ((connection_handle = accept(listen_handle,
              (struct sockaddr *) &client_address, &client_length)) < 0) {

      // If this is an interruption due to a signal, just loop back.
      if (errno != EINTR) {
        perror("Problem accepting a connection");
        return 1;
      }
    }

    // Display informational message.
    #ifdef DEBUG
    inet_ntop(AF_INET, &client_address.sin_addr, buffer, BUFFER_SIZE);
    printf("Accepted client connection from: %s\n", buffer);
    fflush(stdout);
    #endif

    // Come up with a new random number here, in the parent, so that the seed inside rand() can
    // evolve for each connection. If you do this in the child, every child computes the same
    // result for rand() because the seed never changes in the parent!
    //
    int random = rand() % quotes.size();

    // Create the child process to handle this connection.
    if ((child_ID = fork()) == -1) {
      perror("Problem creating child server");
      return 1;
    }

    // This is the child code.
    else if (child_ID == 0) {

      // Send the quote of the day.
      close(listen_handle);
      #ifdef DEBUG
      printf("\tServicing client with process: %d\n", getpid());
      sleep(30);
      snprintf(buffer, BUFFER_SIZE, "From process %d: ", getpid());
      write(connection_handle, buffer, strlen(buffer));
      #endif
      write(connection_handle, quotes[random].c_str(), quotes[random].size());
      close(connection_handle);
      exit(0);

    }

    // This is the parent code.
    close(connection_handle);
  }
}
