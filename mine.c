// OC DO NOT STEAL
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

struct character {
  uint_fast8_t seen;
  struct character *earlier;
};

int main(int argc, char *argv[]) {
  struct character characters[256];
  char output[] = {0, 0};
  unsigned char buffer[2 << 12];
  size_t i;
  ssize_t result;
  struct character *latest = NULL;
  struct character *character;

  for(i = 0; i < sizeof(characters) / sizeof(characters[0]); ++i) {
    characters[i].seen = 0;
  }

  for(;;) {

    do {
      result = read(0, buffer, sizeof(buffer));
    } while(result < 0 && errno == EINTR);
    if(result < 0) return -1;

    if(!result) break;

    // Process the buffer.
    for(i = 0; i < (size_t) result; ++i) {
      character = &characters[buffer[i]];

      // If this is the first time we've seen this character, link it 
      // into the order we've first seen them.
      if(!character->seen) {
        character->earlier = latest;
        latest = character;
      }

      // Count the times we've seen this value and truncate at 2 to 
      // prevent wrapping.
      if(character->seen < 2) ++character->seen;
    }
  }

  // Walk the list of characters used and choose the earliest that was 
  // seen only once.
  for(character = latest; character; character = character->earlier) {
    if(character->seen == 1) output[0] = (char) (character - characters);
  }

  (void) puts(output);

  return 0;
}
