#include <iostream>
#include <tuplet.h>
#include <encoder.h>

byte *character_encoder::accept(character_tuplet *t) {
	std::cout << "Character encode called" << std::endl;
	return NULL;
}

byte *character_encoder::accept(string_reference_tuplet *t) {
	std::cerr << "Character encoder called with string refference" << std::endl;
	exit(-1);
}

byte *string_reference_encoder::accept(character_tuplet *t) {
	std::cerr << "Character encoder called with string refference" << std::endl;
	exit(-1);
}

byte *string_reference_encoder::accept(string_reference_tuplet *t) {
	std::cout << "String reference encode called" << std::endl;
	return NULL;
}
