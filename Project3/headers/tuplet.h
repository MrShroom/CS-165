// @TODO: Seriously, make this multiple files. lol
#include <iostream>
#include <vector>
#include <options.h>
using std::vector;

#ifndef TUPLET_H
#define TUPLET_H
using byte = unsigned char;
class tuplet {
public:
	tuplet() = delete;
	~tuplet() {}// std::cout << "deleting tuplet base pointer" << std::endl; }
	tuplet(const tuplet &t) : data(t.data), encoded_data(t.encoded_data) {};
	tuplet(bool is_character) : is_character(is_character), data(), encoded_data() {};
	virtual byte *encode(const Options& opt) = 0;
	bool is_character_reference() { return is_character; };
	vector<byte> data;
	vector<byte> encoded_data;
	vector<byte> decoded_data; 
private: 
	bool is_character;
};

class character_tuplet : public tuplet {
public:
	character_tuplet() = delete;
	~character_tuplet() { std::cout << "deleting character_tuplet pointer" << std::endl; }
	character_tuplet(int strlen, std::string c) : tuplet(true), c(c) { }; // chars will be length strlen
	int getCode() { return code; };
	int getLen() { return strlen; };
	byte *encode(const Options& opt);
private:
	int code = 0; // Will be L bits
	int strlen; // Will be S bits
	std::string c;
};

class string_reference_tuplet : public tuplet {
public:
	string_reference_tuplet() = delete;
	~string_reference_tuplet() { std::cout << "deleting string_reference_tuplet pointer" << std::endl; }
	string_reference_tuplet(int len, int offset) : tuplet(false), len(len), offset(offset) {};
	int getLen() { return len; };
	int getOffset() { return offset; };
	byte *encode(const Options& opt);
private:
	int len;
	int offset;
};

#endif // TUPLET_H
