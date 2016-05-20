#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include "logger.h"
#include "options.h"

int main(int argc, char** argv) {
	Options opt = Options::GetOptions(argc, argv);
	std::cout << "opt(N, S, L, file): (" << opt.getN() << "," << opt.getS() << "," << opt.getL() << "," << opt.getFile() << ")" << std::endl;
	return 0;
}
