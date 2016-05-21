#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bitset>
#include <fstream>
#include <string>
#include <cstring> // strcpy
#include <tuplet.h>
#include <options.h>
#include <lempel_ziv.h>
#include <encoder.h>

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

/**
 * get_tuplet_string(character_tuplet)
 *	given a new character template, return the string (essentially, return the character property on the tuplet)
 */
std::string LempelZiv::get_tuplet_string(character_tuplet t) {

}

/**
 * get_tuplet_string(string_reference_tuplet t)
 *	given a string reference template.
 *	Method will look through the bits string and return
 *	the string pattern matching it.
 */
std::string LempelZiv::get_tuplet_string(string_reference_tuplet t) {

}

// @TODO: @MrShroom: This is where compress will happen
vector<tuplet> LempelZiv::compress() {
	// iterate over our bits to try to find a pattern for reduction.
	vector<tuplet> compress;

	// create a window of size W-F
	std::string window(opt.getW() - opt.getF(), ' ');

	// output N, L, S

	// copy the first F bits
	window.insert(0, bits, 0, opt.getF());
	unsigned char *data = new byte[opt.getW() - opt.getF()];
	std::strcpy((char*)data, window.c_str());
	compress.push_back(character_tuplet(1, data, new character_encoder()));

	// Search the window to find the longest match with a prefix of the lookahead buffer.
	return compress;
}

// @TODO: @MrShroom: This is where decompress will happen
vector<tuplet> LempelZiv::decompress() {
	vector<tuplet> v;
	return v;
}
