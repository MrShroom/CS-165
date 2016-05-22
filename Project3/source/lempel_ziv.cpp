#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bitset>
#include <fstream>
#include <string>
#include <set>
#include <cstring> // strcpy
#include <tuplet.h>
#include <options.h>
#include <lempel_ziv.h>
#include <encoder.h>

using std::vector;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;
using bits_in_byte = std::bitset<BITS_PER_BYTE>;
LempelZiv::LempelZiv(const Options& opt) : opt(opt), bits{""}, tuplets() {
	read_file_binary();	
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

std::set<std::string> LempelZiv::get_permutations_of_string(std::string str, int length) { // O(n - length)
	std::set<std::string> permutations;
	for (int i = 0; i < str.length() - length; i++) {
		permutations.insert(str.substr(i, length + 1));	
	}
	return permutations;
}

// @TODO: @MrShroom: This is where compress will happen
vector<tuplet*> LempelZiv::compress() {
	
	// iterate over our bits to try to find a pattern for reduction.
	// create a window of size W-F
	std::string window(std::max((int)bits.length(), opt.getF()), ' ');

	// output N, L, S

	// copy the first F bits
	window.insert(0, bits, 0, opt.getF());
	tuplets.push_back(new character_tuplet(0, std::string{bits[0]}, new character_encoder()));

	// match the common longest substring.
	int start = 1;
	while (start < window.length()) {
		bool match = false;
		int distance = 0, start = 1; // start will act as our window frame
		std::string current{window[start]}, buffer(window.substr(0, start));
		while (distance < start) { // O(N!)
			std::set<std::string> possible_matches = get_permutations_of_string(buffer, distance);
			for (const auto& x : possible_matches) std::cout << x << " ";
			std::cout << std::endl;
			for (int i = 0; i < start; i++) { 
				if (possible_matches.find(current) != possible_matches.end() && current.length() > min_length) {
					// @todo: Make distance the distance from start to the index of possible_matches.find
					tuplets.push_back(new string_reference_tuplet(start, distance, new string_reference_encoder()));	
					match = true;
					break;
				} 
			}
			current = window.substr(start, ++distance);
		}
		if (match) {
			start += distance + 1;
		} else {
			tuplets.push_back(new character_tuplet(1, std::string{window[start]}, new character_encoder()));
			start++;
		}
	}
	
	// Search the window to find the longest match with a prefix of the lookahead buffer.
	return tuplets;
}

// @TODO: @MrShroom: This is where decompress will happen
vector<tuplet> LempelZiv::decompress() {
	vector<tuplet> v;
	return v;
}
