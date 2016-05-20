GCC=g++ -Iheaders
FLAGS=-std=c++11
SUPPORT_FILES=./source/logger.cpp ./source/lempel_ziv.cpp ./source/options.cpp
SUPPORT_OBJS=logger.o lempel_ziv.o options.o
SUPPORT_LIB=./lib/supportlib.a
DFLAGS=-ggdb $(FLAGS)
all: lz expand
lz: support
	$(GCC) $(FLAGS) -o LZ $(SUPPORT_LIB) ./source/encoder.cpp ./source/lz.cpp

expand: support
	$(GCC) $(FLAGS) -o EXPAND $(SUPPORT_LIB) ./source/decoder.cpp ./source/expand.cpp

support:
	@$(foreach f,$(SUPPORT_FILES), echo $(f);$(GCC) $(FLAGS) -c $(f);)
	ar rcs $(SUPPORT_LIB) $(SUPPORT_OBJS)
	rm $(SUPPORT_OBJS)

debug: debug-lz debug-expand

debug-lz: support
	$(GCC) $(DFLAGS) -o LZ $(SUPPORT_LIB) ./source/encoder.cpp ./source/lz.cpp

debug-expand: support
	$(GCC) $(DFLAGS) -o EXPAND $(SUPPORT_LIB) ./source/expand.cpp

clean: clean-lz clean-expand clean-support

clean-lz:
	rm LZ
	rm -rf LZ.dSYM 2>/dev/null

clean-expand:
	rm EXPAND
	rm -rf EXPAND.dSYM 2>/dev/null

clean-support:
	rm $(SUPPORT_LIB)
