#include <iostream>
#include <vector>
using std::vector;

#ifndef TUPLET_H
#define TUPLET_H
typedef char byte;
class IEncoder;
class IDecoder;
class tuplet {
public:
	tuplet() = delete;
	tuplet(const tuplet &t) : position(t.position), data(t.data), encoded_data(t.encoded_data), encoder(t.encoder), decoder(t.decoder) {};
	tuplet(int position, IEncoder *encoder) : position(position), encoder(encoder), data(), encoded_data() {};
	tuplet(int position, IDecoder *decoder) : position(position), decoder(decoder), data(), encoded_data() {};
	vector<byte> data;
	vector<byte> encoded_data;
	vector<byte> decoded_data; 
	byte *encode();
	byte *decode();
	int getPosition() { return position; }
private: 
	int position;
	IEncoder *encoder;
	IDecoder *decoder;
};

class IEncoder {
public:
	virtual byte* encode(tuplet) = 0;
};

class IDecoder {
public:
	virtual byte* decode(tuplet) = 0;
};
#endif // TUPLET_H
