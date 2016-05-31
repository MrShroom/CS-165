#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <stdio.h>
#include <logger.h>
#include <options.h>
#include <lempel_ziv.h>
#include <tuplet.h>
#include <vector>

int main(int argc, char **argv) {
	Options opt = Options::GetOptions(argc, argv);
	LempelZiv ziv(opt);
	std::vector<unsigned char> v = ziv.decompress();
	fwrite(&v[0], sizeof(unsigned char), v.size(), stdout);	
	return 0;
}
