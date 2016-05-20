#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bitset>
#include <fstream>
#include "tuplet.h"
#include "options.h"
#include "lempel_ziv.h"

using std::vector;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;
using bits_in_byte = std::bitset<BITS_PER_BYTE>;
LempelZiv::LempelZiv(const Options& opt) : opt(opt), bits{""} {
	read_file_binary();	
	std::cout << "The file in all bytes " << bits << std::endl;
}

// code reference: http://www.cplusplus.com/forum/general/119145/
void LempelZiv::read_file_binary() {
	std::ifstream file(opt.getFile(), std::ios::binary);
	char c;
	while (file.get(c))
		bits += bits_in_byte(byte(c)).to_string(); // cast necessary due to signdness 
}

// @TODO: @MrShroom: This is where compress will happen
vector<tuplet> LempelZiv::compress() {
	vector<tuplet> v;
	return v;
}

// @TODO: @MrShroom: This is where decompress will happen
vector<tuplet> LempelZiv::decompress() {
	vector<tuplet> v;
	return v;
}
