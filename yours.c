#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

enum {
    CHAR_SEEN_DUBS,
    CHAR_SEEN,
    CHAR_UNSEEN,
};

static unsigned char buf[0x1a000000];

static unsigned char get_first_char( const char *str ){
    uint8_t chars[256];
    unsigned i;
    memset( chars, CHAR_UNSEEN, 256 );

    for ( i = 0; str[i]; i++ ){
        if ( chars[ str[i] ] ){
            chars[ str[i] ]--;
        }
    }

    for ( i = 0; str[i]; i++ ){
        if ( chars[str[i]] ){
            return str[i];
            break;
        }
    }

    return '\0';
}

int main( int argc, char *argv[] ){
    fgets( buf, sizeof( buf ), stdin );

    unsigned char ret = get_first_char( buf );

    if ( ret ){
        printf( "First non-repeating character is %c\n", ret );

    } else {
        puts( "u fugged" );
    }

    return 0;
}
