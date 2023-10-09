#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#define IS_CONTINUATION(ch) (((ch) & 0xC0) == 0x80)

int decode_UTF8( int ch1, FILE *infile )
{
    int ch2, ch3, ch4;
    int code_point;

    if( (ch1 & 0x80) == 0x00 ) {
        // 1 byte encoding.
        code_point = ch1;
    }
    else if( (ch1 & 0xE0) == 0xC0 ) {
        // 2 byte encoding.
        ch2 = getc( infile );
        if( ch2 == EOF || !IS_CONTINUATION(ch2) ) {
            printf( "Bad UTF-8 encoding!\n" );
            exit( EXIT_FAILURE );
        }
        ch1 &= 0x1F;
        ch2 &= 0x3F;
        ch2 |= (ch1 & 0x03) << 6;
        ch1 >>= 2;
        code_point = (ch1 << 8) | ch2;
    }
    else if( (ch1 & 0xF0) == 0xE0 ) {
        // 3 byte encoding.
        ch2 = getc( infile );
        ch3 = getc( infile );
        if( ch2 == EOF || !IS_CONTINUATION(ch2) ||
            ch3 == EOF || !IS_CONTINUATION(ch3) ) {
            printf( "Bad UTF-8 encoding!\n" );
            exit( EXIT_FAILURE );
        }
        ch1 &= 0x0F;
        ch2 &= 0x3F;
        ch3 &= 0x3F;
        ch3 |= (ch2 & 0x03) << 6;
        ch2 = (ch2 >> 2) | (ch1 << 4);
        code_point = (ch2 << 8) | ch3;
    }
    else if( (ch1 & 0xF8) == 0xF0 ) {
        // 4 byte encoding.
        ch2 = getc( infile );
        ch3 = getc( infile );
        ch4 = getc( infile );
        if( ch2 == EOF || !IS_CONTINUATION(ch2) ||
            ch3 == EOF || !IS_CONTINUATION(ch3) ||
            ch4 == EOF || !IS_CONTINUATION(ch4) ) {
            printf( "Bad UTF-8 encoding!\n" );
            exit( EXIT_FAILURE );
        }
        ch1 &= 0x07;
        ch2 &= 0x3F;
        ch3 &= 0x3F;
        ch4 &= 0x3F;
        ch4 |= (ch3 & 0x03) << 6;
        ch3 = (ch3 >> 2) | ((ch2 & 0x0F) << 4);
        ch2 = (ch2 >> 6) | (ch1 << 2);
        code_point = (ch2 << 16) | (ch3 << 8) | ch4;
    }
    else {
        printf( "Bad UTF-8 encoding!\n" );
        exit( EXIT_FAILURE );
    }
    return code_point;
}


void encode_UTF16( int code_point, FILE *outfile )
{
    // For now I'll just assume BMP characters.
    putc( (code_point & 0x0000FF00) >> 8, outfile );
    putc( (code_point & 0x000000FF)     , outfile );
}


int main( int argc, char *argv[] )
{
    FILE *infile;    // Input file.
    FILE *outfile;   // Output file.
    int ch;          // A character from the input file.
    int code_point;  // Unicode code point of "current" character.

    if( argc != 3 ) {
        fprintf( stderr, "Usage: %s input-file output-file\n", argv[0] );
        return EXIT_FAILURE;
    }

    if( (infile = fopen( argv[1], "rb" )) == NULL ) {
        fprintf( stderr, "Unable to open %s for reading\n", argv[1] );
        return EXIT_FAILURE;
    }

    if( (outfile = fopen( argv[2], "wb" )) == NULL ) {
        fprintf( stderr, "Unable to open %s for writing\n", argv[2] );
        fclose( infile );
        return EXIT_FAILURE;
    }

    // Write the BOM into the output file to start. I'll use big endian order.
    putc( 0xFE, outfile );
    putc( 0xFF, outfile );

    while( (ch = getc( infile )) != EOF ) {
        code_point = decode_UTF8( ch, infile );
        printf( "U+%04X\n", code_point );
        encode_UTF16( code_point, outfile );
    }

    fclose( infile );
    fclose( outfile );
    return EXIT_SUCCESS;
}
