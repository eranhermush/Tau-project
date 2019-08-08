#ifndef TAU_PROJECT_MATHCER_BY_NAME
#define TAU_PROJECT_MATHCER_BY_NAME

#include "hash_matcher.h"
#include "kdf_sae.h"
#include "ffc_pwe_sample_matcher.h"
#include "ecc_pwe_sample_matcher.h"
#include "pwe_sample.h"
#include <NTL/ZZ.h>
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/sha3.h"
#include "cryptopp/whrlpool.h"
#include "cryptopp/keccak.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

namespace Matcher_By_Name{
	/* Creates a Preimage matcher with the given parameters
		If name is "sae ecc" or "sae ffc" then create_sae_sample_matcher and the arguments are expected to match

		
		Else, create_matcher creates a hash matcher
			args should be of lenght at least 1, and args[0] should be the target is a hexstring (with printable characters, e.g. "02fbc5")
			The supported hashes are (case insensitive)
				sha2/sha256/sha256
				sha384/sha-384
				sha512/sha-512
				sha3/sha3-256
				sha3-384
				sha3-512
				sha1
				md5
				whirlpool
				keccak/keccak-256

	*/

	std::unique_ptr<Preimage_Matcher> create_matcher(const std::string& name, const std::vector<std::string>& args);


	/* Creates a sae_sample_matcher with the given parameters
		args should be of size 2
		arg[0] is path to a file containing sample data by the conventions in pwe_sample.h
		args[1] is a the group name
		If name is "sae ecc":
			Group names are "group19"/"group20"/"group21"
		If name is "sae ffc":
			Group names are - "group22"/"group23"/"group24" */
	std::unique_ptr<Preimage_Matcher> create_sae_sample_matcher(const std::string& lc_name, const std::vector<std::string>& args);

	

}

#endif