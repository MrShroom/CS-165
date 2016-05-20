#include <iostream>
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
	LempelZiv(const Options& opt);
	vector<tuplet> compress();
	vector<tuplet> decompress();
private:
	const Options& opt;
	void read_file_binary();
	std::string bits;
};
#endif
