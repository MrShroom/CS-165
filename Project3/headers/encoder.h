
// Based off slide 1 of http://www.ics.uci.edu/~dan/class/165/notes/CS165-LZ.pdf, we'll need two different encoders
// and decoders.
#ifndef ENCODERS_H
#define ENCODERS_H
#include <tuplet.h>
using byte = unsigned char;

class IEncoder {
public:
	~IEncoder() { std::cout << "Deleting iencoder base pointer" << std::endl; };
	byte* encode(tuplet *t) { return t->visit(this); };
	virtual byte* accept(string_reference_tuplet *tuplet) = 0;
	virtual byte* accept(character_tuplet *tuplet) = 0;
};

class character_encoder : public IEncoder {
public:
	character_encoder() {};
	byte* accept(character_tuplet *tuplet);
	byte* accept(string_reference_tuplet *tuplet);
};

class string_reference_encoder : public IEncoder {
public:
	string_reference_encoder() {};
	byte* accept(character_tuplet *tuplet);
	byte* accept(string_reference_tuplet *tuplet);
};
#endif
