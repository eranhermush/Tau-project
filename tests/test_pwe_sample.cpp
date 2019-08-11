#include "../Worker/pwe_sample.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

void print_sample(pwe_sample& sample){
	std::cout << std::hex << (int) sample.mac1[0];
	for(int i = 1; i < 6; ++i){
		std::cout << ':' << (int) sample.mac1[i];
	}
	std::cout << ' ' << (int) sample.mac2[0];
	for(int i = 1; i < 6; ++i){
		std::cout << ':' << (int) sample.mac2[i];
	}
	std::cout << ' ' << std::dec << (int) sample.counter;
	std::cout << ' ' << (int) sample.result;
	std::cout << std::endl;
}

int main(int argc, char** argv){
	unsigned char correct[6] = {0x23, 0x45, 0x67, 0x89, 0xab, 0x00};

	std::vector<std::string> MACs1({"23:45:67:89:ab:00", "23:45:67:89:ab:00:cd:ef", "23:45:67:89:Ab:00", "23:45:67:89:AB:00",
		"23456789ab00", "23-45-67-89-ab-00", "2^3&4g5k6l7q8p9\x00?a\xff'b%0!0", "23:45:67:89:ab"});
	std::vector<std::string> MACs2({"223:45:67:89:ab:00", "23:45:67:89:ab:01", "23:45:67:89", "45:67:89:ab:00", "223:45:67:89:ab:00",
		"23:45:67:89:ab:01", "23:45:67:89", "45:67:89:ab:00"});
	std::vector<unsigned char> count(9, 1);
	std::vector<char> results(12, 0);

	std::vector<pwe_sample> samples(pwe_sample::create_sample_vector(MACs1, MACs2, count, results));
	if(samples.size() != 8){
		std::cout << "failed vector creation" << std::endl;
	}
	
	for(int i = 0; i < 8; ++i){
		if(!std::equal(std::begin(correct), std::end(correct), std::begin(samples[i].mac1))){
			std::cout << "falied correct mac input " << i << std::endl;
		}
		if(std::equal(std::begin(correct), std::end(correct), std::begin(samples[i].mac2))){
			std::cout << "falied incorrect mac input " << i << std::endl;
		}
	}

	if(argc >= 2){
		// argument is path
		std::vector<pwe_sample> file_samples(pwe_sample::create_sample_vector_from_file(argv[1]));
		std::cout << "read " << file_samples.size() << " samples" << std::endl;
		for(unsigned int i = 0; i < file_samples.size(); ++i){
			print_sample(file_samples[i]);
		}
	}

	
	std::cout << "finished testing pwe_sample" << std::endl;
	return 0;
}