GCC=g++
FLAGS=-std=c++11
FILES=logger.cpp lempel_ziv.cpp options.cpp
DFLAGS=-ggdb $(FLAGS)
all: lz expand
lz:
	$(GCC) $(FLAGS) -o LZ $(FILES) encoder.cpp lz.cpp
expand:
	$(GCC) $(FLAGS) -o EXPAND $(FILES) expand.cpp

debug: debug-lz debug-expand

debug-lz:
	$(GCC) $(DFLAGS) -o LZ $(FILES) lz.cpp

debug-expand:
	$(GCC) $(DFLAGS) -o EXPAND $(FILES) expand.cpp

clean: clean-lz clean-expand

clean-lz:
	rm LZ
	rm -rf LZ.dSYM 2>/dev/null

clean-expand:
	rm EXPAND
	rm -rf EXPAND.dSYM 2>/dev/null

