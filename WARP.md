# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

This repository contains various network programming experiments and demonstrations implementing different middleware technologies and network communication patterns. It serves as a testbed for educational purposes exploring network programming in multiple languages (C, C++, Java, Python).

## Key Components

### RPC (Remote Procedure Call) Systems
- **ONC RPC Examples**: Located in `RPC/` and `FileServices/RPC/`
- **Extended Integer Arithmetic Service**: Demonstrates RPC with complex data types including unions and custom structures
- **File Services**: Simple file server/client using RPC for basic file operations

### CORBA Implementation
- **FileServices CORBA**: Object-oriented distributed computing example
- **IDL Definitions**: Interface Definition Language files for cross-platform interoperability

### Various Network Programming Examples
- **Echo Servers**: Multiple implementations (threaded, concurrent, client variations)
- **Protocol Implementations**: VTP ("VTC Transport Protocol", a custom, simplified transport protocol), IPX networking
- **Utility Programs**: Character conversion, base64 encoding, DNS lookups

### Middleware Technologies Demonstrated
- **Ice Framework**: Various language bindings (C++, Java) for FileServices
- **Web Services**: Java-based SOAP examples
- **Socket Programming**: Raw socket examples with select() multiplexing

## Common Development Commands

### Building RPC Examples
```bash
# Extended Integers RPC service
cd RPC
make clean
make all
# Generates RPC stubs from extended_ints.x using rpcgen

# File Services RPC
cd FileServices/RPC  
make clean
make all
```

### Building CORBA Examples
```bash
cd CORBA
# IDL compilation required (specific commands depend on CORBA implementation)
# TAO or omniORB typically used
```

### Building Individual Components
```bash
# Most subdirectories with Makefiles can be built with:
make clean && make all

# For single-file C programs:
gcc -Wall -o program_name source_file.c

# For C++ programs:
g++ -Wall -o program_name source_file.cpp
```

### Running RPC Services
```bash
# Start RPC server (in one terminal)
cd RPC
./server

# Run client (in another terminal)
./client <hostname> <number1> <number2>
```

## Architecture Notes

### RPC Interface Design Patterns
- **Union Types**: Used for polymorphic data (int vs extended_int in `extended_ints.x`)
- **Opaque Data**: Variable-length byte arrays for arbitrary precision integers
- **Structured Arguments**: Complex parameter passing with nested types
- **Discriminated Unions**: Type-safe variant types in interface definitions

### Code Generation Workflow
1. Define interface in `.x` files (RPC) or `.idl` files (CORBA)
2. Use `rpcgen` or IDL compiler to generate client/server stubs
3. Implement business logic in server procedures
4. Build client applications using generated headers

### Memory Management Considerations
- RPC-generated code requires careful cleanup of dynamically allocated structures
- Extended precision arithmetic (VeryLong class) uses STL vectors for automatic memory management
- Current implementations have known memory leaks that should be addressed in production use

### Multi-language Integration
- **C/C++ Integration**: RPC examples show mixing C-generated stubs with C++ business logic
- **Platform Portability**: Code tested on Linux systems, may require adjustments for other platforms
- **Compiler Requirements**: Uses traditional gcc/g++ toolchain

## Development Environment Setup

### Prerequisites
- GCC/G++ compiler toolchain
- RPC development libraries (typically part of glibc-dev)
- CORBA implementation (TAO, omniORB, or similar) for CORBA examples
- Java SDK for Web Services examples

### Platform Notes
- Originally developed and tested on SUSE Linux 10.1 with GCC 4.1.0
- Code structure assumes Unix-like environment
- Some examples may require specific network configurations or services

## Interface Definition Language (IDL) Files

### Key IDL Patterns
- **Module Namespacing**: CORBA interfaces organized in modules (FileServices)
- **Sequence Types**: Variable-length arrays with optional size limits
- **Enumeration Types**: Type-safe constants (OpenMode: READ, WRITE)
- **Method Signatures**: Input/output parameter specifications with CORBA types

The codebase demonstrates fundamental distributed systems concepts including marshaling, remote object references, and cross-platform data representation.
