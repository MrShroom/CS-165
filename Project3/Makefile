GCC=g++ -I headers
FLAGS=-std=c++11
SUPPORT_FILES=./source/logger.cpp ./source/lempel_ziv.cpp ./source/options.cpp
SUPPORT_OBJS=logger.o lempel_ziv.o options.o
SUPPORT_LIB=./lib/supportlib.a
DFLAGS=-ggdb $(FLAGS)
all: lz expand
load-cpp11:
	module load gcc/5.2.0 2>/dev/null || echo ""

lz: load-cpp11 support
	$(GCC) $(FLAGS) -o LZ ./source/encoder.cpp ./source/lz.cpp $(SUPPORT_LIB) 

expand: load-cpp11 support
	$(GCC) $(FLAGS) -o EXPAND ./source/decoder.cpp ./source/expand.cpp $(SUPPORT_LIB) 

support:
	@$(foreach f,$(SUPPORT_FILES), echo $(f);$(GCC) $(DFLAGS) -c $(f);)
	ar rcs $(SUPPORT_LIB) $(SUPPORT_OBJS)
	rm $(SUPPORT_OBJS)

debug: debug-lz debug-expand

debug-lz: support
	$(GCC) $(DFLAGS) -o LZ ./source/encoder.cpp ./source/lz.cpp $(SUPPORT_LIB) 

debug-expand: support
	$(GCC) $(DFLAGS) -o EXPAND ./source/expand.cpp $(SUPPORT_LIB) 

clean: clean-lz clean-expand clean-support

clean-lz:
	rm LZ
	rm -rf LZ.dSYM 2>/dev/null || echo ""

clean-expand:
	rm EXPAND
	rm -rf EXPAND.dSYM 2>/dev/null || echo ""

clean-support:
	rm $(SUPPORT_LIB)
