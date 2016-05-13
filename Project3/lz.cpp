#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include "logger.h"

using namespace std;

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

Options Options::GetOptions(int argc, char** argv) {
	char c;
	Options options;
	while ((c = getopt(argc, argv, options.opts.c_str())) != -1) {
		string optargStr = optarg;
		int arg;
		if (optarg[0] == '=') {
			arg = std::stoi(optargStr.substr(1));
		} else {
			arg = std::stoi(optargStr);
		}
		switch (c) {
			case 'N':
				if (arg < 9 || 14 < arg) {
					std::cerr << "N argument is out of range. Should be between 9 and 14" << std::endl;
					exit(-1);
				} else {
					options.N = arg;
				}
				break;
			case 'S':
				if (arg < 3 || 4 < arg) {
					std::cerr << "S argument is out of range. Should be either 3 or 4" << std::endl;
					exit(-2);
				} else {
					options.S = arg;
				}
				break;
			case 'L':
				if (arg < 1 || 5 < arg) {
					std::cerr << "L argument is out of range. Should be between 1 and 5" << std::endl;
					exit(-3);
				} else {
					options.L = arg;
				}
				break;
		}
	}

	// get the filename
	for (int i = optind; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (options.file.empty()) {
				options.file = argv[i];
			} else {
				std::cerr << "Additional file name provided" << std::endl;
			}
		} 
	}
	if (options.file.empty()) {
		std::cerr << "No file name!" << std::endl;
		exit(-4);
	}
	return options;
}

int main(int argc, char** argv) {
	Options opt = Options::GetOptions(argc, argv);
	std::cout << "opt(N, S, L, file): (" << opt.getN() << "," << opt.getS() << "," << opt.getL() << "," << opt.getFile() << ")" << std::endl;
	return 0;
}
