
README.txt
==========

This is an example client and server that communicate using Sun's ONC RPC. The server provides
procedures for doing arithmetic operations on extended precision integers. The client simply
calls the server to add and multiply two given numbers. This example illustrates several
things.

1. The way to create RPC clients and servers.

2. Various features of the interface definition language. The interface uses an enumeration
type, an unbounded sequence, and various structure types, including a discriminated type. Some
of these features are a bit unnecessary for a program like this but they do illustrate a range
of possibilities at least.

3. How to combine C and C++ in an RPC program.

The files VeryLong.cpp and VeryLong.hpp are provided to do the actual grunt work of the
computations. They are not part of the example, but they are necessary to get real results.

The example is incomplete. Finishing it would be nice but would probably only be of limited
educational value (perhaps it could be done as a lab experience). Note, however, that the
example currently does not do memory management very well (it leaks memory). This should be
fixed.

The example has been tested on SUSE Linux 10.1 using gcc v4.1.0.
