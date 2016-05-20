#include <iostream>
#include <vector>
#include <algorithm>
#include "tuplet.h"
#include "options.h"

#ifndef LEMPELZIV_H
#define LEMPELZIV_H
using std::vector;
class LempelZiv {
public:
	LempelZiv(const Options& opt) : opt(opt) {};
	vector<tuplet> compress();
	vector<tuplet> decompress();
private:
	const Options& opt;
};
#endif
