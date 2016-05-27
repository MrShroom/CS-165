#include <iostream>
#include <vector>
using std::vector;

#ifndef TUPLET_H
#define TUPLET_H
using byte = unsigned char;
class IEncoder;
class IDecoder;
class tuplet {
public:
	tuplet() = delete;
	~tuplet() {}// std::cout << "deleting tuplet base pointer" << std::endl; }
	tuplet(const tuplet &t) : data(t.data), encoded_data(t.encoded_data), encoder(t.encoder), decoder(t.decoder) {};
	tuplet(bool is_character, IEncoder *encoder) : is_character(is_character), encoder(encoder), data(), encoded_data() {};
	tuplet(bool is_character, IDecoder *decoder) : is_character(is_character), decoder(decoder), data(), encoded_data() {};
	vector<byte> data;
	vector<byte> encoded_data;
	vector<byte> decoded_data; 
	byte *encode();
	byte *decode();
	bool is_character_reference() { return is_character; };
private: 
	bool is_character;
	IEncoder *encoder;
	IDecoder *decoder;
};

class character_tuplet : public tuplet {
public:
	character_tuplet() = delete;
	~character_tuplet() { std::cout << "deleting character_tuplet pointer" << std::endl; }
	character_tuplet(int strlen, std::string c, IEncoder *encoder) : tuplet(true, encoder), c(c) {}; // chars will be length strlen
	character_tuplet(int strlen, std::string c, IDecoder *decoder) : tuplet(true, decoder), c(c) {}; // chars will be length strlen
private:
	int code = 0; // Will be L bits
	int strlen; // Will be S bits
	std::string c;
};

class string_reference_tuplet : public tuplet {
public:
	string_reference_tuplet() = delete;
	~string_reference_tuplet() { std::cout << "deleting string_reference_tuplet pointer" << std::endl; }
	string_reference_tuplet(int len, int offset, IEncoder *encoder) : tuplet(false, encoder) {};
	string_reference_tuplet(int len, int offset, IDecoder *decoder) : tuplet(false, decoder) {};
private:
	int len;
	int offset;
};

class IEncoder {
public:
	~IEncoder() { std::cout << "Deleting iencoder base pointer" << std::endl; };
	virtual byte* encode(tuplet) = 0;
};

class IDecoder {
public:
	virtual byte* decode(tuplet) = 0;
};
#endif // TUPLET_H
