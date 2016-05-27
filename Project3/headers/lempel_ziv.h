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
	~LempelZiv() {
        for(int i =0; i < m_tuplets.size(); i++)
            delete m_tuplets[i];
    };
	LempelZiv(const Options& opt);
	vector<char> compress();
	vector<char> decompress();
private:
	const Options& opt;
    
    //This is a debug structure	
    typedef struct tuplet_counts_s {
		int character_counts;
		int string_ref_counts;
		int *distro;
	} tuplet_count_t;
	
    //member functions
	void read_file_binary();
	void compress_window(std::string window, tuplet_count_t& data);
    vector<char> encode();
    void decode();

    //member vars.
	vector<tuplet*> m_tuplets;
	std::string m_bits;
};
#endif
