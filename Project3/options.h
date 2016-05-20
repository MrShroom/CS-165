#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include "logger.h"

using namespace std;

#ifndef OPTIONS_H
#define OPTIONS_H
class Options {
public:
	Options() : N(11), L(4), S(3), file("") {}
	int getN() { return N; }
	int getL() { return L; }
	int getS() { return S; }
	int getW() { return powbTwo(N); } // lazy loading
	int getF() { return powbTwo(L) - 1; } // lazy loading
	int getT() { return powbTwo(S) - 1; } // lazy loading
	string getFile() { return file; }
	static Options GetOptions(int argc, char** argv);
private:
	int powbTwo(int x) { return pow(4, x); }
	int N, L, S;
	const string opts = "N:S:L:";
	string file;
};

#endif
