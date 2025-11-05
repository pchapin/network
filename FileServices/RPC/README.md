
README
======

On a Ubuntu system, you many need to install the following packages to build and run the ONC
RPC version:

```
sudo apt install rpcsvc-proto libtirpc-dev rpcbind
```

The package `libtirpc-dev` is needed to provide the `tirpc/rpc/rpc.h` header file. It is for a
more modern implementation of ONC RPC than the traditional one that uses "transport independent
RPC" (TIRPC).
