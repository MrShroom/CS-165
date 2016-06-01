// @TODO: Seriously, make this multiple files. lol
#include <iostream>
#include <vector>
#include <bitset>
#include <options.h>
using std::vector;

#ifndef TUPLET_H
#define TUPLET_H
using byte = unsigned char;
constexpr int MAX_BIT_SET=265;
// these will be fused into a byte*
class encoded_tuplet {
public:
	encoded_tuplet(std::bitset<MAX_BIT_SET> s, int actual) : set(s), count(actual) {};
	std::bitset<MAX_BIT_SET> set;
	int count;
};

class tuplet {
public:
	tuplet() = delete;
	~tuplet() {}// std::cout << "deleting tuplet base pointer" << std::endl; }
	tuplet(const tuplet &t) {};
	tuplet(bool is_character) : is_character(is_character) {};
	virtual byte *encode(const Options& opt, std::vector<encoded_tuplet>& result) = 0;
	virtual std::string const getC (){ return "";}
    virtual int getLen() {return 0;}
    virtual void setC (std::string newC){};
	virtual int getOffset() { return 0; }; 
    bool is_character_reference() { return is_character; };
private: 
	bool is_character;
};

class character_tuplet : public tuplet {
public:
	character_tuplet() = delete;
	~character_tuplet() { std::cerr << "deleting character_tuplet pointer" << std::endl; }
	character_tuplet(int _strlen, std::string c) : tuplet(true), strlen(_strlen), c(c) { }; // chars will be length strlen
	int getCode() { return code; };
    void setC (std::string newC)
    {
        c = newC;
        strlen = c.length()/8;
    }
    std::string const getC() { return c;}
	int getLen() { 
        return strlen; };
	byte *encode(const Options& opt, std::vector<encoded_tuplet>& result);
private:
	int code = 0; // Will be L bits
	int strlen; // Will be S bits
	std::string c;
};

class string_reference_tuplet : public tuplet {
public:
	string_reference_tuplet() = delete;
	~string_reference_tuplet() { std::cerr << "deleting string_reference_tuplet pointer" << std::endl; }
	string_reference_tuplet(int len, int offset) : tuplet(false), len(len), offset(offset) {};
	int getLen() { return len; };
	int getOffset() { return offset; };
	byte *encode(const Options& opt, std::vector<encoded_tuplet>& result);
private:
	int len;
	int offset;
};

#endif // TUPLET_H
