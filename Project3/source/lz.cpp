#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <logger.h>
#include <options.h>
#include <lempel_ziv.h>
#include <tuplet.h>
#include <encoder.h>
#include <vector>

int main(int argc, char** argv) {
	Options opt = Options::GetOptions(argc, argv);
	std::cout << "opt(N, S, L, file): (" << opt.getN() << "," << opt.getS() << "," << opt.getL() << "," << opt.getFile() << ")" << std::endl;
	LempelZiv ziv(opt);
	std::vector<char> v = ziv.compress();
	return 0;
}
