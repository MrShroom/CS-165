#include <limits>
#include <iostream>
#include <bitset>
#include <vector>
#include <tuplet.h>
#include <options.h>
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;
byte *string_reference_tuplet::encode(const Options& opt, std::vector<encoded_tuplet>& res) {
	std::bitset<MAX_BIT_SET> bits;
	// len is encoded as len - 1
	if (len > opt.getF()*8) {
		std::cerr << len << " is greater than the max " << opt.getF()*8 << std::endl;
	}
	len -= 1;
	for (int j=0,i = 1; j < opt.getL(); j++,i = i << 1) {
		bits.set( opt.getN() + j, (len & i) >> j);	
	}
	

	for (int j=0, i = 1; j < opt.getN(); j++, i = i << 1) {
		bits.set(j, (offset & i) >> j);
	}
	res.push_back(encoded_tuplet(bits, opt.getL() + opt.getN()));
	return NULL;
}

byte *character_tuplet::encode(const Options& opt, std::vector<encoded_tuplet> &t) {
	std::bitset<MAX_BIT_SET> bits;

	// automatically skip L bits since we never set the leading L bits
	for (int j=0, i = 1; j < opt.getS(); j++,i = i<<1) {
		bits.set(j + strlen*BITS_PER_BYTE, (strlen & i) >> j);
	}

	for (int i = 0; i < strlen*BITS_PER_BYTE; i++) {
		bits.set(i, c[strlen*BITS_PER_BYTE - i - 1] == '1');
	}
	t.push_back(encoded_tuplet(bits, opt.getS() + opt.getL() + strlen*BITS_PER_BYTE));
}

