#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

enum {
    CHAR_SEEN_DUBS,
    CHAR_SEEN,
    CHAR_UNSEEN,
};

typedef struct character {
    unsigned status;
    unsigned index;
} character_t;

int main( int argc, char *argv[] ){
    character_t chars[256];
    unsigned char buf[2 << 12];
    size_t str_index = 0;
    unsigned i;

    for ( i = 0; i < 256; i++ ){
        chars[i].status = CHAR_UNSEEN;
        chars[i].index  = 0;
    }

    ssize_t readret = 1;
    while ( readret > 0 ){
        readret = read( 0, buf, sizeof( buf ));

        for ( i = 0; i < readret; i++, str_index++ ){
            if ( chars[buf[i]].status ){
                chars[buf[i]].status--;
                chars[buf[i]].index = str_index;
            }
        }
    }

    unsigned c;
    unsigned lowest;
    unsigned ret = '\0';

    for ( lowest = UINT_MAX, c = 0; c < 256; c++ ){
        if ( chars[c].status == CHAR_SEEN && chars[c].index < lowest ){
            ret = c;
            lowest = chars[c].index;
        }
    }

    if ( ret ){
        printf( "%c\n", ret );

    } else {
        puts( "u fugged" );
    }

    return 0;
}
