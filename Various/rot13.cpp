
#include <cctype>
#include <string>

static char lower_translation[] = "nopqrstuvwxyzabcdefghijklm";
static char upper_translation[] = "NOPQRSTUVWXYZABCDEFGHIJKLM";

// Do a ROT13 encoding of the given string.
//
void rot13( std::string &line )
{
    for( std::string::size_type i = 0; i < line.size(); ++i ) {
        if( std::isalpha( line[i] ) ) {
            if( std::isupper( line[i] ) ) {
                line[i] = upper_translation[line[i] - 'A'];
            }
            else {
                line[i] = lower_translation[line[i] - 'a'];
            }
        }
    }
}
