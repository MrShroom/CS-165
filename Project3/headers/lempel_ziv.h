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
	const int min_length = 1;
	
    typedef struct tuplet_counts_s {
		int character_counts;
		int string_ref_counts;
		int *distro;
	} tuplet_count_t;
	
    //member functions
	std::string get_tuplet_string(character_tuplet t);
	std::string get_tuplet_string(string_reference_tuplet t);
	void compress_window(std::string window, tuplet_count_t& data);
	void read_file_binary();
    
    //member vars.
	vector<tuplet*> m_tuplets;
	std::string m_bits;
};
#endif
