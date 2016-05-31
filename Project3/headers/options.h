#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <logger.h>

using namespace std;

#ifndef OPTIONS_H
#define OPTIONS_H
class Options {
public:
	Options() : N(11), L(4), S(3), file("") {}
	int getN() const { return N; }
	int getL() const { return L; }
	int getS() const { return S; }
	int getW() const { return powbTwo(N); } // lazy loading
	int getF() const { return powbTwo(L) - 1; } // lazy loading
	int getT() const { return powbTwo(S) - 1; } // lazy loading
    bool getDebug() const { return D; }
	string getFile() const { return file; }
	static Options GetOptions(int argc, char** argv);
	int N, L, S;
private:
	int powbTwo(int x) const { return pow(2, x); }
    bool D = false;
	const string opts = "DN:S:L:";
	string file;
};

#endif
