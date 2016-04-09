// OC DO NOT STEAL
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <memory.h>

struct character {
  struct character *earlier;
};

int main(int argc, char *argv[]) {
  uint8_t seen[256];
  struct character characters[256];
  char output[] = {0, 0};
  unsigned char buffer[2 << 12];
  unsigned char c;
  size_t i;
  ssize_t result;
  struct character *latest = NULL;
  struct character *character;

  (void) memset(seen, 0, sizeof(seen));

  for(;;) {

    do {
      result = read(0, buffer, sizeof(buffer));
    } while(result < 0 && errno == EINTR);
    if(result < 0) return -1;

    if(!result) break;

    // Process the buffer.
    for(i = 0; i < (size_t) result; ++i) {
      c = buffer[i];
      character = &characters[c];

      // If this is the first time we've seen this character, link it 
      // into the order we've first seen them.
      if(!seen[c]) {
        character->earlier = latest;
        latest = character;
      }

      // Count the times we've seen this value and truncate at 2 to 
      // prevent wrapping.
      if(seen[c] < 2) ++seen[c];
    }
  }

  // Walk the list of characters used and choose the earliest that was 
  // seen only once.
  for(character = latest; character; character = character->earlier) {
    c = (char) (character - characters);
    if(seen[c] == 1) output[0] = c;
  }

  (void) puts(output);

  return 0;
}
