#include <options.h>


Options Options::GetOptions(int argc, char** argv) {
	char c;
	Options options;
	while ((c = getopt(argc, argv, options.opts.c_str())) != -1) {
		
        int arg;
        if(c != 'D')
        {
            string optargStr = optarg;
            if (optarg[0] == '=') {
                arg = std::stoi(optargStr.substr(1));
            } else {
                arg = std::stoi(optargStr);
            }
        }
		switch (c) {
			case 'N':
				if (arg < 2 || 14 < arg) {
					std::cerr << "N argument is out of range. Should be between 9 and 14" << std::endl;
					exit(-1);
				} else {
					options.N = arg;
				}
				break;
			case 'S':
				if (arg < 1 || 5 < arg) {
					std::cerr << "S argument is out of range. Should be between 1 and 5" << std::endl;
					exit(-2);
				} else {
					options.S = arg;
				}
				break;
			case 'L':
				if (arg < 3 || 4 < arg) {
					std::cerr << "L argument is out of range. Should be either 3 or 4" << std::endl;
					exit(-3);
				} else {
					options.L = arg;
				}
				break;
           case 'D':
                options.D = true;
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

