GCC=g++ -I headers
FLAGS=-std=c++11
SUPPORT_FILES=./source/logger.cpp ./source/lempel_ziv.cpp ./source/options.cpp
SUPPORT_OBJS=obj/logger.o obj/lempel_ziv.o obj/options.o
SUPPORT_LIB=./lib/supportlib.a
DFLAGS=-ggdb $(FLAGS)
all: LZ EXPAND
load-cpp11:
	module load gcc/5.2.0 2>/dev/null || echo ""

LZ: load-cpp11 lib/support.a
	$(GCC) $(FLAGS) -o LZ ./source/encoder.cpp ./source/lz.cpp $(SUPPORT_LIB) 

EXPAND: load-cpp11 lib/support.a
	$(GCC) $(FLAGS) -o EXPAND ./source/decoder.cpp ./source/expand.cpp $(SUPPORT_LIB) 

lib/support.a: $(SUPPORT_OBJS)
	mkdir lib 2>/dev/null || echo "lib directory created"
	ar rcs $(SUPPORT_LIB) $(SUPPORT_OBJS)

obj/logger.o: source/logger.cpp
	$(GCC) $(DFLAGS) -c $^
	mkdir obj 2>/dev/null || echo "obj made"
	mv logger.o obj/

obj/lempel_ziv.o: source/lempel_ziv.cpp
	$(GCC) $(DFLAGS) -c $^
	mkdir obj 2>/dev/null || echo "obj made"
	mv lempel_ziv.o obj/

obj/options.o: source/options.cpp
	$(GCC) $(DFLAGS) -c $^
	mkdir obj 2>/dev/null || echo "obj made"
	mv options.o obj


debug: debug-lz debug-expand

debug-lz: lib/support.a
	$(GCC) $(DFLAGS) -o LZ ./source/encoder.cpp ./source/lz.cpp $(SUPPORT_LIB) 

debug-expand: lib/support.a
	$(GCC) $(DFLAGS) -o EXPAND ./source/encoder.cpp ./source/expand.cpp $(SUPPORT_LIB) 

clean: clean-lz clean-expand clean-support

clean-lz:
	rm LZ
	rm -rf LZ.dSYM 2>/dev/null || echo ""

clean-expand:
	rm EXPAND
	rm -rf EXPAND.dSYM 2>/dev/null || echo ""

clean-support:
	rm $(SUPPORT_LIB)
