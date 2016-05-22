#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <tuplet.h>
#include <options.h>

#ifndef LEMPELZIV_H
#define LEMPELZIV_H
using std::vector;
class LempelZiv {
public:
	LempelZiv() = delete;
	~LempelZiv() { };
	LempelZiv(const Options& opt);
	vector<tuplet*> compress();
	vector<tuplet> decompress();
private:
	const Options& opt;
	const int min_length = 2;
	std::string bits;
	vector<tuplet*> tuplets;
	std::set<std::string> get_permutations_of_string(std::string str, int length);
	std::string get_tuplet_string(character_tuplet t);
	std::string get_tuplet_string(string_reference_tuplet t);
	void read_file_binary();
};
#endif
