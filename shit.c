// OC DO NOT STEAL
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

struct character {
  uint8_t seen;
  struct character *earlier;
};

int main(int argc, char *argv[]) {
  struct character characters[256];
  char output = 0;
  unsigned char buffer[2 << 12];
  size_t i;
  ssize_t result;
  struct character *latest = NULL;
  struct character *character;

  for(i = 0; i < sizeof(characters) / sizeof(characters[0]); ++i) {
    characters[i].seen = 0xff;
  }

  for(;;) {

    do {
      result = read(0, buffer, sizeof(buffer));
    } while(result < 0 && errno == EINTR);
    if(result < 0) return -1;

    // Fuck checking for newlines like an undergrad and having to deal 
    // with different line endings, we'll watch for EOF and save a few 
    // cycles.
    if(!result) break;

    // Process the buffer.
    for(i = 0; i < (size_t) result; ++i) {
      character = &characters[buffer[i]];

      // If this is the first time we've seen this character, link it 
      // into the order we've first seen them.
      if(character->seen == 0xff) {
        character->earlier = latest;
        latest = character;
      }

      // Count the times we've seen this value as the number of 0 bits 
      // left.  Truncates at 8 but does not wrap like adding would (you 
      // faggots, consider your edge cases) without requiring a 
      // conditional.
      character->seen <<= 1;
    }
  }

  // Walk the list of characters used and choose the earliest that was 
  // seen only once.
  for(character = latest; character; character = character->earlier) {
    if(character->seen == 0xfe) output = (char) (character - characters);
  }

  // Since OP left the 'if no such characters exist' case as UNDEFINED 
  // BEHAVIOR, I choose to write a NULL.  Also, fuck newlines.
  (void) putchar(output);
  (void) putchar('\n');

  return 0;
}
