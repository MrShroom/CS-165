#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <logger.h>
#include <options.h>
#include <lempel_ziv.h>

int main(int argc, char** argv) {
	Options opt = Options::GetOptions(argc, argv);
	std::cout << "opt(N, S, L, file): (" << opt.getN() << "," << opt.getS() << "," << opt.getL() << "," << opt.getFile() << ")" << std::endl;
	LempelZiv ziv(opt);
	ziv.compress();
	return 0;
}
