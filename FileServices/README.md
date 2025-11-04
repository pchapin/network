
FileServices Demonstration
==========================

This folder contains a demonstration file client/server program to illustrate some middleware
technologies. Currently versions using Ice and ONC RPC are provided. The ONC RPC version is in
the RPC folder. Various Ice versions are in folders separated by programming language. Not all
versions are equally mature and some versions may be quite out of date.

On a Ubuntu system, you many need to install the following packages to build and run the ONC
RPC version:

```
sudo apt install rpcsvc-proto libtirpc-dev rpcbind
```

The package `libtirpc-dev` is needed to provide the `tirpc/rpc/rpc.h` header file. It is for a
more modern implementation of ONC RPC than the traditional one that uses "transport independent
RPC" (TIRPC).