#include "pwe_sample.h"

/* Converts a MAC in string form to 6-bytes (for example "00:01:02:03:0A:0B" --> [0,1,2,3,10,11])
	stores the result in dest_mac must be of size >= 6

	if string is does not contain a valid mac address dest_mac still might be changed */
void string_to_mac(const std::string& mac_str, unsigned char* dest_mac);


void string_to_mac(const std::string& mac_str, unsigned char* dest_mac){
	unsigned int i = 0, j = 0, n = mac_str.length();
	unsigned char temp_val = 0, temp_chr = 0;
	while(j < n && i < 6){
		temp_val = 0;
		temp_chr = (unsigned char) mac_str[j];
		++j;
		if('0' <= temp_chr && temp_chr <= '9'){
			temp_val += temp_chr - '0';
		}
		else if('a' <= temp_chr && temp_chr <= 'f'){
			temp_val += temp_chr - 'a' + 10;
		}
		else if('A' <= temp_chr && temp_chr <= 'F'){
			temp_val += temp_chr - 'A' + 10;
		}
		else{
			continue;
		}
		// find next 4-bits of the byte
		while(j < n){
			temp_chr = (unsigned char) mac_str[j];
			++j;
			if('0' <= temp_chr && temp_chr <= '9'){
				temp_val <<= 4;
				temp_val += temp_chr - '0';
			}
			else if('a' <= temp_chr && temp_chr <= 'f'){
				temp_val <<= 4;
				temp_val += temp_chr - 'a' + 10;
			}
			else if('A' <= temp_chr && temp_chr <= 'F'){
				temp_val <<= 4;
				temp_val += temp_chr - 'A' + 10;
			}
			else{
				// did not find
				continue;
			}
			// found
			break;
		}
		dest_mac[i] = temp_val;
		++i;
	}
}

pwe_sample::pwe_sample(const std::string& mac_addr1, const std::string& mac_addr2, unsigned char count, char res)
	: counter(count), result(res){
		string_to_mac(mac_addr1, mac1);
		string_to_mac(mac_addr2, mac2);
}

std::vector<pwe_sample> pwe_sample::create_sample_vector(const std::vector<std::string>& macs1, const std::vector<std::string>& macs2,
	const std::vector<unsigned char>& counts, const std::vector<char>& results){
	
	unsigned int size = std::min(std::min(macs1.size(), macs2.size()), std::min(counts.size(), results.size()));
	std::vector<pwe_sample> samples;
	samples.reserve(size);
	for(unsigned int i = 0; i < size; ++i){
		samples.push_back(pwe_sample(macs1[i], macs2[i], counts[i], results[i]));
	}
	return samples;
}