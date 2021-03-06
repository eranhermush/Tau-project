#ifndef TAU_PROJECT_PWE_SAMPLE_H
#define TAU_PROJECT_PWE_SAMPLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

/* This is a data structue used to store sample data from the Password-to-Element part of WPA3-SAE protocol.
	
	It stores all data apart from the actual password which was used,
	i.e. the 2 MAC addresses, the counter and the result of the test (where the test itself is unspecified,
	as it can be the Quadratic Residue test in the ECC case, or a simple Comparission in the FFC case).*/

struct pwe_sample{
	unsigned char mac1[6];
	unsigned char mac2[6];
	unsigned char counter;
	char result;

	/* Constructor.
		mac_addr1 and mac_addr2 are converted from hex string represntation of MAC addresses to 6-bytes
		for example ff:00:ff:00:10:00 => [255, 0, 255, 0, 16, 0]*/
	pwe_sample(const std::string& mac_addr1, const std::string& mac_addr2, unsigned char count, char res);

	//implicit copy constructor works as expected

	/* Creates a vector of pwe_sample with the appropriate arguments of size min(spoofed_macs.size()), ... , results.size())*/
	static std::vector<pwe_sample> create_sample_vector(const std::vector<std::string>& macs1, const std::vector<std::string>& mac2,
		const std::vector<unsigned char>& counts, const std::vector<char>& results);

	/* Creates a vector of pwe_samples from file at path
		The file should be in format
		Each line containing a new sample is of the form (with separating spaces)
			MAC_ADDRESS_1 MAC_ADDRESS_2 counter result*/
	static std::vector<pwe_sample> create_sample_vector_from_file(const std::string& path);
};

#endif