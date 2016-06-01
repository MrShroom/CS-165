#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <stdio.h>
#include <logger.h>
#include <options.h>
#include <lempel_ziv.h>
#include <tuplet.h>
#include <chrono>
#include <vector>

int main(int argc, char** argv) {
	using namespace std::chrono;

	Options opt = Options::GetOptions(argc, argv);
	std::cerr << "opt(N, L, S, file): (" << opt.getN() << "," << opt.getL() << "," << opt.getS() << "," << opt.getFile() << ")" << std::endl;
	LempelZiv ziv(opt);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	std::vector<unsigned char> v = ziv.compress();

	high_resolution_clock::time_point t2 = high_resolution_clock::now();	
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cerr << "Compression took " << time_span.count() << " seconds" << std::endl;
	fwrite(&v[0], sizeof(unsigned char), v.size(), stdout);	
	return 0;
}
