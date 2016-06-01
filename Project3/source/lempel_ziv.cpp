#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bitset>
#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <cstring> // strcpy
#include <tuplet.h>
#include <options.h>
#include <lempel_ziv.h>

using std::vector;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;
using bits_in_byte = std::bitset<BITS_PER_BYTE>;

LempelZiv::LempelZiv(Options& opt) : opt(opt), m_bits{""}, m_tuplets() {
	read_file_binary();	
}

// code reference: http://www.cplusplus.com/forum/general/119145/
void LempelZiv::read_file_binary() {
	std::ifstream file;
	if (opt.getFile() != "") {
		file.open(opt.getFile(), std::ios::binary);
	} else {
		file.open("/dev/stdin", std::ios::binary);	
	}
	char c;
	while (file.get(c))
		m_bits += bits_in_byte(byte(c)).to_string(); // cast necessary due to signdness 
}


void LempelZiv::compress_window(std::string window, tuplet_count_t& data) {

    //bit pattern to index of where it starts
    std::unordered_map< std::string, int> bit_pattern_to_index_map;
	// O(wf)
    bool last_add_was_char_tuplet = false;
    for(int current_bit_index = 0; current_bit_index < window.length() ; ) 
    {
        int look_ahead_amount = min((int)(opt.getF()*BITS_PER_BYTE),(int)(window.length() - current_bit_index));
        std::string current_buffer(window.substr(current_bit_index, look_ahead_amount));
		bool found_and_added_tuple = false;
        while(look_ahead_amount > 2*BITS_PER_BYTE )
        {
            std::unordered_map< std::string, int>::iterator  biPaToInMa_Itr = bit_pattern_to_index_map.find(current_buffer);
			if (biPaToInMa_Itr == bit_pattern_to_index_map.end())
            {
                bit_pattern_to_index_map.insert(std::make_pair(current_buffer, current_bit_index));

            }else if(!found_and_added_tuple)
            {
                int len = current_buffer.size()/BITS_PER_BYTE;
                int offset = (current_bit_index - biPaToInMa_Itr->second)/BITS_PER_BYTE; 
                m_tuplets.push_back(new string_reference_tuplet(len, offset));	

                found_and_added_tuple = true;
                last_add_was_char_tuplet = false;
                
                if(opt.getDebug())
                {
                    data.string_ref_counts++;
				    data.distro[len]++;
				}
			} 
            look_ahead_amount -= BITS_PER_BYTE ;
			current_buffer = window.substr(current_bit_index, look_ahead_amount);
		}

		if (!found_and_added_tuple) {
            
           tuplet* last_tuplet;

            if(last_add_was_char_tuplet )
            {
               last_tuplet = m_tuplets[m_tuplets.size()-1];
               std::string new_bit_string = last_tuplet->getC();
               new_bit_string += window.substr(current_bit_index,BITS_PER_BYTE);
               last_tuplet->setC(new_bit_string); 
            }
            else
            {
			    m_tuplets.push_back(new character_tuplet(1,window.substr(current_bit_index,BITS_PER_BYTE)));
                last_tuplet =  m_tuplets[m_tuplets.size()-1];
            }


            last_add_was_char_tuplet = last_tuplet->getLen() >= opt.getT() ? false : true;
            current_bit_index += BITS_PER_BYTE;
            if(opt.getDebug())
			    data.character_counts++;
		}
        else
        {
            current_bit_index += (m_tuplets[m_tuplets.size()-1]->getLen())*BITS_PER_BYTE;
        }
	}
}

// This is where compress will happen
vector<byte> LempelZiv::compress() {
	
	// iterate over our bits to try to find a pattern for reduction.
	// create a window of size W-F
	std::string window = "" ;
	// output N, L, S

    
	// These only used if in debug mode
    // for measurment
	tuplet_count_t analyze;
	if(opt.getDebug())
    {
        analyze.character_counts = 0;
        analyze.string_ref_counts = 0;
        analyze.distro = new int[opt.getF()];
        for (int i = 0; i < opt.getF(); i++) {
            analyze.distro[i] = 0;
        }
    }

	// Loop through and compress all windows into vector of tuples.
	// @TODO this can be parallalized  
	std::cerr << "\n";
    for(int start = 0; start < m_bits.length(); start += window.length())
    {
        if(opt.getDebug())
        {
            std::cerr << "\rcompressing: |";
            int i;
            for( i = 0; i <= (int)(100 * (double)start /(double) m_bits.length()); i++)
                std::cerr << "#";
            for(;i< 100;i++)
                std::cerr << " ";
            std::cerr << "|"; 

        }
		window = m_bits.substr(start, opt.getW()*BITS_PER_BYTE);
		compress_window(window, analyze);
	}
	
	if(opt.getDebug())
     {
        std::cerr << "\n" << analyze.character_counts << " new characters\t" << analyze.string_ref_counts << " repeats\n\t";
        std::cerr << "Reduced roughly " << ((double)analyze.string_ref_counts / (double)analyze.character_counts) << " repetitions\n";
        std::cerr << std::flush;
        for (int i = 0; i < 90; i++) {
            std::cerr << "_";
        }
        std::cerr << std::endl << std::flush;
        for (int i = 0; i < opt.getF(); i++) {
				
            std::cerr << i+1 << "\t" << analyze.distro[i] << "\t|";
            if(analyze.distro[i] < 100)
            {
                for (int j = 0; j < analyze.distro[i]; j++) {
                    std::cerr << "=";
                }
            }
            else if(analyze.distro[i] < 1000)
            {
                 for (int j = 0; j < analyze.distro[i]; j+=100 ) {
                    std::cerr << "*";
                }

            }
            else if(analyze.distro[i] < 10000)
            {
                 for (int j = 0; j < analyze.distro[i]; j+=1000) {
                    std::cerr << "#";
                }

            }
            else
            {
                 for (int j = 0; j < analyze.distro[i]; j+=10000) {
                    std::cerr << "@";
                }

            }
            std::cerr << std::endl;
        }
    }
    // add final tuplet.
    m_tuplets.push_back(new character_tuplet(0,""));
	return encode();
}

vector<byte> LempelZiv::encode()
{
    vector<byte> output;
	vector<encoded_tuplet> result;
    string output_bit_string = "";

    for(int i = 0; i < m_tuplets.size(); i++)
    {
        m_tuplets[i]->encode(opt, result);
    }

	int index = 7, encryptedcount = 0;
	std::bitset<8> buffer;
	buffer.set();
    // print out N, L, S
    for (int i = 0,j=1; i < 8; i++,j=j<<1) {
        buffer.set(i, ((opt.getN() & j) >> i));
    }
    output.push_back(static_cast<byte>(buffer.to_ulong()));
    buffer.set();

    for (int i = 0,j=1; i < 8; i++,j=j<<1) {
        buffer.set(i, ((opt.getL() & j) >> i));
    }
    output.push_back(static_cast<byte>(buffer.to_ulong()));
    buffer.set();

    for (int i = 0,j=1; i < 8; i++,j=j<<1) {
        buffer.set(i, ((opt.getS() & j) >> i));
    }
    output.push_back(static_cast<byte>(buffer.to_ulong()));
    buffer.set();

	for (auto &i : result) {
		encryptedcount += i.count;
		for (int j = 0; j < i.count; j++,index--) {
			buffer.set(index, i.set[i.count - j - 1]);
			if (index == 0) {
				output.push_back(static_cast<byte>(buffer.to_ulong()));
				buffer.set(); // reset to zero;
				index = 8;
			}
		}
	}

    if (index < 7) {
        // fill the remaining bits.
        for (int i = index; i < 8; i++)
            buffer.set(i, 0);
        output.push_back(static_cast<byte>(buffer.to_ulong()));
    }
    std::cerr << "Input length " << m_bits.size() << " bits" << std::endl;
    std::cerr << "Output length " << encryptedcount << " bits" << std::endl;
	std::cerr << "Compression Savings " << std::setprecision(4) << ((1-(encryptedcount/(m_bits.size()*1.0)))*100) << "%"<< std::endl;
    return output;
}

// This is where decompress will happen
vector<byte> LempelZiv::decompress() {

    decode();
    int original_size = m_bits.size();
    m_bits = "";
    for(auto &current_tuplet : m_tuplets)
    {
        if(current_tuplet->is_character_reference())
        {
            m_bits += current_tuplet->getC();
        }
        else
        {
            int current_size = m_bits.size();
            for(int i = 0 ; i < (current_tuplet->getLen()*BITS_PER_BYTE); i++)
            {
                //                bits                   this is in bytes             to bits
                m_bits += m_bits[current_size + i - (current_tuplet->getOffset() * BITS_PER_BYTE)];
            }
        }
    }
    vector<byte> v;

    std::bitset<8> buffer;
    int index = 7;
    buffer.set();
    for (int i = 0; i < m_bits.size(); i++,index--) {
        buffer.set(index, m_bits[i] == '1');
        if (index == 0) {
	        v.push_back(static_cast<byte>(buffer.to_ulong()));
            index = 8;
        }
        std::cerr << std::flush;
    }
    std::cerr << "Input length " << original_size << " bits" << std::endl; 
    std::cerr << "Output length " << m_bits.size() << " bits" << std::endl;
    std::cerr << "Compression Savings " << std::setprecision(4) << ((1.0-(original_size/(m_bits.size()*1.0)))*100) << "%"<< std::endl;
    return v;
}

unsigned int LempelZiv::getIntFromString(std::string str, int total_bits) {
    unsigned int intfromstring = (unsigned int)(str[0] == '1');
    for (int i = 1; i < total_bits; i++) {
        intfromstring = (intfromstring << 1) + ((unsigned int)(str[i]) == '1');
    }
    return intfromstring;
}
void LempelZiv::decode()
{
    std::bitset<8> buffer;
    std::string N = m_bits.substr(0, 8);
    for (int i = 0; i < 8; i++) {
        buffer.set(i, N[7-i] == '1');
    }
    opt.N = static_cast<unsigned int>(buffer.to_ulong());
    buffer.set();

    std::string L = m_bits.substr(8, 8);
    for (int i = 0; i < 8; i++) {
        buffer.set(i, L[7-i] == '1');
    }
    opt.L = static_cast<unsigned int>(buffer.to_ulong());
    buffer.set();

    std::string S = m_bits.substr(16, 8);
    for (int i = 0; i < 8; i++) {
        buffer.set(i, S[7-i] == '1');
    }
    opt.S = static_cast<unsigned int>(buffer.to_ulong());

    std::cerr << "opt(N, L, S) = " << opt.N << "," << opt.L << "," << opt.S << std::endl;
    // next, read L bits. If 0, then read S and strlen.
    // else, read N bits.
    std::string remainder = m_bits.substr(24);
    int i = 0;
    while (true) {
        std::string LBits = remainder.substr(i, opt.L);
        i += opt.L;
        unsigned int L = getIntFromString(LBits, opt.L);
        if (!L) {
            // read S bits, make that strlen
            std::string SBits = remainder.substr(i, opt.S);
            unsigned int strlen = getIntFromString(SBits, opt.S);
            if (!strlen) break;
            i += opt.S;
            std::string characters = remainder.substr(i, strlen*BITS_PER_BYTE);
            i += strlen*BITS_PER_BYTE;
            m_tuplets.push_back(new character_tuplet(strlen, characters));
        } else {
            std::string NBits = remainder.substr(i, opt.N);
            unsigned int offset = getIntFromString(NBits, opt.N);
            m_tuplets.push_back(new string_reference_tuplet(L + 1, offset));
            i += opt.N;
        }
    }
}
