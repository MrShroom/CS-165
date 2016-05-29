#include <iostream>
#include <bitset>
#include <vector>
#include <tuplet.h>
#include <options.h>

constexpr int MAX_CHAR_SET=9;
byte *string_reference_tuplet::encode(const Options& opt, std::vector<encoded_tuplet>& res) {
	std::bitset<MAX_BIT_SET> bits;
	int head = opt.getL() + opt.getN();
	// len is encoded as len - 1
	if (len > opt.getF()) {
		std::cerr << len << " is greater than the max " << opt.getF() << std::endl;
	}
	len = len >> 1;
	for (int j=0,i = 1; j < opt.getL(); j++,i = i << 1) {
		bits.set(head - opt.getL() + j, (len & i) >> j);	
	}
	head -= opt.getL();

	for (int j=0, i = 1; j < opt.getN(); j++, i = i << 1) {
		bits.set(j, (offset & i) >> j);
	}
	std::cerr << "str_ref: " << len << " and " << offset << " encoded too " << bits.to_string();
	std::cerr << std::endl;
	res.push_back(encoded_tuplet(bits, opt.getL() + opt.getN()));
	return NULL;
}

byte *character_tuplet::encode(const Options& opt, std::vector<encoded_tuplet> &t) {
	std::bitset<MAX_BIT_SET> bits;
	std::bitset<4> chars((byte)c[0]);
	int head = opt.getL();

	// automatically skip S bits.
	for (int j=0, i = 1; j < head; j++,i = i<<1) {
		bits.set(j + 4, (strlen & i) >> j);
	}

	for (int i = 0; i < 4; i++) {
		bits.set(i, chars[i]);
	}
	std::cerr << "char: " << strlen << " and " << c << " encoded too " << bits.to_string() << std::endl;
	
	t.push_back(encoded_tuplet(bits, head + 4));
}

