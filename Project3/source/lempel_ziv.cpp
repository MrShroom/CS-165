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
#include <encoder.h>

using std::vector;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;
using bits_in_byte = std::bitset<BITS_PER_BYTE>;
LempelZiv::LempelZiv(const Options& opt) : opt(opt), m_bits{""}, m_tuplets() {
	read_file_binary();	
}

// code reference: http://www.cplusplus.com/forum/general/119145/
void LempelZiv::read_file_binary() {
	std::ifstream file(opt.getFile(), std::ios::binary);
	char c;
	while (file.get(c))
		m_bits += bits_in_byte(byte(c)).to_string(); // cast necessary due to signdness 
}


void LempelZiv::compress_window(std::string window, tuplet_count_t& data) {

    std::unordered_map< std::string, int> bit_pattern_to_index_map;
    
	// O(wf)
    for(int current_bit_index = 0; current_bit_index < window.size(); current_bit_index++ ) 
    {	
        int look_ahead_amount = min(opt.getF(), (int)window.length() - current_bit_index);
        std::string current_buffer(window.substr(current_bit_index, look_ahead_amount));
		bool found_and_added_tuple = false;

         while(look_ahead_amount --> 0 )
         {
            std::unordered_map< std::string, int>::iterator  biPaToInMa_Itr = bit_pattern_to_index_map.find(current_buffer);
			if (biPaToInMa_Itr == bit_pattern_to_index_map.end())
            {
                bit_pattern_to_index_map.insert(std::make_pair (current_buffer, current_bit_index));

            }else if(!found_and_added_tuple)
            {
                int len = current_buffer.size();
                int offset = current_bit_index - biPaToInMa_Itr -> second; 
				m_tuplets.push_back(new string_reference_tuplet(len, offset, new string_reference_encoder()));	
                found_and_added_tuple = true;
                
                if(opt.getDebug())
                {
                    data.string_ref_counts++;
				    data.distro[len]++;
				}
			} 

			current_buffer = window.substr(current_bit_index, look_ahead_amount);
		}

		if (!found_and_added_tuple) {
			m_tuplets.push_back(new character_tuplet(1, std::string{window[current_bit_index]}, new character_encoder()));
            if(opt.getDebug())
			    data.character_counts++;
		}
	}
}

// This is where compress will happen
vector<char> LempelZiv::compress() {
	
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
    for(int start = 1; start < m_bits.length(); start += window.length())
    {
        if(opt.getDebug())
        {
            std::cerr << "\rcompressing: |";
            int i;
            for( i = 0; i < (int)(100 * (double)start /(double) m_bits.length()); i++)
                std::cerr << "#";
            for(;i< 100;i++)
                std::cerr << " ";
            std::cerr << "|"; 

        }
		window = m_bits.substr(start - 1, std::min((int)m_bits.length() - start, opt.getW()));
		compress_window(window, analyze);
	}
	
	if(opt.getDebug())
    {
        std::cerr << "\n" << analyze.character_counts << " new characters\t" << analyze.string_ref_counts << " repeats\n\t";
        std::cerr << "Reduced roughly " << ((double)analyze.string_ref_counts / (double)analyze.character_counts) << " repetitions\n\t";
        std::cerr << std::flush;
        for (int i = 0; i < 90; i++) {
            std::cerr << "_";
        }
        std::cerr << std::endl << std::flush;
        for (int i = 0; i < opt.getF(); i++) {
            std::cerr << i+1 << " " << analyze.distro[i] << "\t|";
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
	return encode();
}

vector<char> LempelZiv::encode()
{
    vector<char> output;
    string output_bit_string = "";

    //@TODO @IanSchweer The plan here is to loop through all tuplets to create bit string
    for(int i = 0; i < m_tuplets.size(); i++)
    {
        //Please change
       // m_tuplets[i]->encode(output_bit_string);
    }

    //@TODO @IanSchweer Then convert bit to vector of char with your voodo
    return output;

}

// @TODO: @MrShroom: This is where decompress will happen
vector<char> LempelZiv::decompress() {

    decode();

	vector<char> v;
	return v;
}


void LempelZiv::decode()
{
    //m_bit -> m_tuplets
}
