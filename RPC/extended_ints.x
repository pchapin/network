/*****************************************************************************
FILE   : extended_ints.x
AUTHOR : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This file contains the interface description for an RPC server that does arbitrary integer
arithemtic.
*****************************************************************************/

/* Two types of integers can be used. */
enum int_type { INT = 1, EXTENDED = 2 };

/* Extended precision integers have a variable number of bytes. */
typedef opaque extended_int<>;

/* The procedures work on a "int_value" which can be one of the two different supported integer
   types. Thus the procedures can add just plain ints (for speed) as well as extended ints when
   necessary. They can convert from one to the other as required */

union int_value switch (int_type type_flag) {
  case INT:
    int value_i;
  case EXTENDED:
    extended_int value_e;
};

/* For the procedures requiring two int_value arguments. */
struct two_intvalues {
  int_value arg1;
  int_value arg2;
};

/* Some of the procedurces operate on rational numbers. */
struct rational_value {
  int_value top;
  int_value bottom;
};

/* For the procedures requiring two rational_value arguments. */
struct two_rationalvalues {
  rational_value arg1;
  rational_value arg2;
};

program EXTENDED_INTS {
  version EXTENDED_INTS_VER {
    int_value IADD(two_intvalues) = 1;
    int_value ISUB(two_intvalues) = 2;
    int_value IMUL(two_intvalues) = 3;
    int_value IDIV(two_intvalues) = 4;
    int_value IMOD(two_intvalues) = 5;
    int_value INEG(int_value)     = 6;
    
    rational_value RADD(two_rationalvalues) = 7;
    rational_value RSUB(two_rationalvalues) = 8;
    rational_value RMUL(two_rationalvalues) = 9;
    rational_value RDIV(two_rationalvalues) = 10;
    rational_value RNEG(rational_value)     = 11;
    rational_value REDUCE(rational_value)   = 12;
  } = 1;
} = 0x30009000;
