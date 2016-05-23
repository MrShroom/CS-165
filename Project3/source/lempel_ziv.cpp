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
	if (length >= str.length()) {
		permutations.insert(str);
		return permutations;
	}
	for (int i = 0; i < str.length() - length + 1; i++) {
		permutations.insert(str.substr(i, length));	
	}
	return permutations;
}

void LempelZiv::compress_window(std::string window, int start, tuplet_count_t& data) {
	while (start < window.length()) {
		bool match = false;
		int distance = std::min(start, (int)window.length() - start); // start will act as our window frame
		std::string current(window.substr(start, distance)), buffer(window.substr(0, start));
		while (distance > 0) { // O(N!)
			std::set<std::string> possible_matches = get_permutations_of_string(buffer, distance);
			if (possible_matches.find(current) != possible_matches.end() && current.length() > min_length) {
				// @todo: Make distance the distance from start to the index of possible_matches.find
				tuplets.push_back(new string_reference_tuplet(start, distance, new string_reference_encoder()));	
				data.string_ref_counts++;
				data.distro[distance]++;
				match = true;
				break;
			} 
			current = window.substr(start, --distance);
		}
		if (match) {
			start += distance + 1;
		} else {
			tuplets.push_back(new character_tuplet(1, std::string{window[start]}, new character_encoder()));
			data.character_counts++;
			start++;
		}
	}
}
// @TODO: @MrShroom: This is where compress will happen
vector<tuplet*> LempelZiv::compress() {
	
	// iterate over our bits to try to find a pattern for reduction.
	// create a window of size W-F
	std::string window{bits.substr(0, std::min((int)bits.length(), opt.getF()))};

	// output N, L, S

	// copy the first F bits
	tuplets.push_back(new character_tuplet(0, std::string{bits[0]}, new character_encoder()));

	// for measurment
	tuplet_count_t analyze;
	analyze.character_counts = 0;
	analyze.string_ref_counts = 0;
	analyze.distro = new int[opt.getF()];
	for (int i = 0; i < opt.getF(); i++) {
		analyze.distro[i] = 0;
	}

	// match the common longest substring.
	int start = 1;
	while (start < bits.length()) {
		compress_window(window, 1, analyze);
		start += window.length();
		window = bits.substr(start - 1, std::min((int)bits.length() - start, opt.getF()));
	}
	
	// Search the window to find the longest match with a prefix of the lookahead buffer.
	std::cout << analyze.character_counts << " new characters\t" << analyze.string_ref_counts << " repeats\n\t";
	std::cout << "Reduced roughly " << ((double)analyze.string_ref_counts / (double)analyze.character_counts) << " repetitions\n\t";
	std::cout << std::flush;
	for (int i = 0; i < analyze.string_ref_counts; i++) {
		std::cout << "_";
	}
	std::cout << std::endl << std::flush;
	for (int i = 0; i < opt.getF(); i++) {
		std::cout << i+1 << " " << analyze.distro[i] << "\t|";
		for (int j = 0; j < analyze.distro[i]; j++) {
			std::cout << "=";
		}
		std::cout << std::endl;
	}
	return tuplets;
}

// @TODO: @MrShroom: This is where decompress will happen
vector<tuplet> LempelZiv::decompress() {
	vector<tuplet> v;
	return v;
}
