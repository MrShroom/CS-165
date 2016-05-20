#include "tuplet.h"

// Based off slide 1 of http://www.ics.uci.edu/~dan/class/165/notes/CS165-LZ.pdf, we'll need two different encoders
// and decoders.
class character_encoder : IEncoder {
public:
	byte *encode(tuplet);
};

class string_reference_encoder : IEncoder {
public:
	byte *encode(tuplet);
};
