/*****************************************************************************
FILE   : server.cpp
AUTHOR : (C) Copyright 2012 by Peter C. Chapin <PChapin@vtc.vsc.edu>

This file contains the implementation of the remote procedures.
*****************************************************************************/

#include "extended_ints.h"

#include <sstream>
#include "VeryLong.hpp"

using namespace std;

int_value *iadd_1_svc( two_intvalues *arg, struct svc_req * )
{
    static int_value result;
    
    if( arg->arg1.type_flag == INT && arg->arg2.type_flag == INT ) {
        result.type_flag = INT;
        result.int_value_u.value_i =
            arg->arg1.int_value_u.value_i + arg->arg2.int_value_u.value_i;
            
        return &result;
    }
    
    if( arg->arg1.type_flag == EXTENDED && arg->arg2.type_flag == EXTENDED ) {
        result.type_flag = EXTENDED;
        
        VeryLong num1( arg->arg1.int_value_u.value_e.extended_int_val );
        VeryLong num2( arg->arg2.int_value_u.value_e.extended_int_val );
        VeryLong sum = num1 + num2;
        ostringstream formatter;
        formatter << sum;
        string rep = formatter.str( );
        result.int_value_u.value_e.extended_int_val = new char[rep.size( ) + 1];
        strcpy( result.int_value_u.value_e.extended_int_val, rep.c_str( ) );
        result.int_value_u.value_e.extended_int_len = rep.size( ) + 1;
        
        return &result;
    }
    
    return NULL;
}


int_value *isub_1_svc( two_intvalues *, struct svc_req * )
{
    return NULL;
}


int_value *imul_1_svc( two_intvalues *arg, struct svc_req * )
{
    static int_value result;
    
    if( arg->arg1.type_flag == INT && arg->arg2.type_flag == INT ) {
        result.type_flag = INT;
        result.int_value_u.value_i =
            arg->arg1.int_value_u.value_i * arg->arg2.int_value_u.value_i;
            
        return &result;
    }
    
    if( arg->arg1.type_flag == EXTENDED && arg->arg2.type_flag == EXTENDED ) {
        result.type_flag = EXTENDED;
        
        VeryLong num1( arg->arg1.int_value_u.value_e.extended_int_val );
        VeryLong num2( arg->arg2.int_value_u.value_e.extended_int_val );
        VeryLong sum = num1 * num2;
        ostringstream formatter;
        formatter << sum;
        string rep = formatter.str( );
        result.int_value_u.value_e.extended_int_val = new char[rep.size( ) + 1];
        strcpy( result.int_value_u.value_e.extended_int_val, rep.c_str( ) );
        result.int_value_u.value_e.extended_int_len = rep.size( ) + 1;
        
        return &result;
    }
    
    return NULL;
}


int_value *idiv_1_svc( two_intvalues *, struct svc_req * )
{
    return NULL;
}


int_value *imod_1_svc( two_intvalues *, struct svc_req * )
{
    return NULL;
}


int_value *ineg_1_svc( int_value *, struct svc_req * )
{
    return NULL;
}


rational_value *radd_1_svc( two_rationalvalues *, struct svc_req * )
{
    return NULL;
}


rational_value *rsub_1_svc( two_rationalvalues *, struct svc_req * )
{
    return NULL;
}


rational_value *rmul_1_svc( two_rationalvalues *, struct svc_req * )
{
    return NULL;
}


rational_value *rdiv_1_svc( two_rationalvalues *, struct svc_req * )
{
    return NULL;
}


rational_value *rneg_1_svc( rational_value *, struct svc_req * )
{
    return NULL;
}


rational_value *reduce_1_svc( rational_value *, struct svc_req * )
{
    return NULL;
}

