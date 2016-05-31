#include <iostream>
#include <bitset>
#include <vector>
#include <tuplet.h>
#include <options.h>

byte *string_reference_tuplet::encode(const Options& opt, std::vector<encoded_tuplet>& res) {
	std::bitset<MAX_BIT_SET> bits;
	int head = opt.getL() + opt.getN();
	// len is encoded as len - 1
	if (len > opt.getF()) {
		std::cerr << len << " is greater than the max " << opt.getF() << std::endl;
	}
	len -= 1;
	for (int j=0,i = 1; j < opt.getL(); j++,i = i << 1) {
		bits.set(head - opt.getL() + j, (len & i) >> j);	
	}
	head -= opt.getL();

	for (int j=0, i = 1; j < opt.getN(); j++, i = i << 1) {
		bits.set(j, (offset & i) >> j);
	}
	res.push_back(encoded_tuplet(bits, opt.getL() + opt.getN()));
	if (opt.getDebug()) {
		std::cerr << "Adding string_reference_tuplet <" << (len + 1) << ", " << offset << ">" << std::endl;
	}
	return NULL;
}

byte *character_tuplet::encode(const Options& opt, std::vector<encoded_tuplet> &t) {
	std::bitset<MAX_BIT_SET> bits;

	// automatically skip L bits since we never set the leading L bits
	for (int j=0, i = 1; j < opt.getS(); j++,i = i<<1) {
		bits.set(j + strlen, (strlen & i) >> j);
	}

	for (int i = 0; i < strlen; i++) {
		bits.set(i, c[strlen-i-1] == '1');
	}
	t.push_back(encoded_tuplet(bits, opt.getS() + opt.getL() + strlen));
	if (opt.getDebug()) {
		std::cerr << "Adding character_tuplet <0, " << (strlen) << ", " << c << ">" << std::endl;
	}
}

