#ifndef TAU_PROJECT_USER_TEXT_INTERFACE_H
#define TAU_PROJECT_USER_TEXT_INTERFACE_H

#include "pattern_utils.h"
#include "helpful_functions.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <utility>

/*
	Various functions to communicate with the user and get arguments
*/

namespace User_Text_Interface{

	const std::unordered_set<std::string> FUNCTION_NAMES({"sae ecc","sae ffc","sha1","sha2","sha256","sha2-256","sha384","sha2-384","sha512","sha2-512","sha3",
															"sha3-256","sha3-384","sha3-512","md5","whirlpool","keccak","keccak-256"});

	const std::unordered_map<std::string, unsigned int> DIGEST_SIZES({{"md5",128}, {"sha1",160}, {"sha2",256}, {"sha256",256}, {"sha2-256",256}, {"keccak",256},{"keccak-256",256}, {"sha3",256}, {"sha3-256",256}, {"sha2-384",384}, {"sha384",384}, {"sha3-384",384}, {"sha2-512",512}, {"sha512",512}, {"sha3-512",512}, {"whirlpool",512}});		



	/*--------------------------------------------------------*/

	/* Gets all the enumeration parameters from the user 
		Returns true is user chose "exit" and false otherwise */

	bool get_enumeration_parameters(std::string& func_name, std::string& func_target, std::string& func_args_rep_string,
		std::string& password_pattern, std::vector<std::string>& paths, uint64_t& start, uint64_t& end, unsigned int& job_size);


	/*----------------Uses these sub-fucntions----------------*/


	void begining_prompt();

	void please_wait_prompt();

	std::string get_function_name(bool& should_exit);

	/* Fills tagret and args with paramaters for func_name */
	void get_function_paramaters(const std::string& func_name, bool& should_exit, std::string& target, std::vector<std::string>& args);

	void get_hash_parametes(const std::string& func_name, bool& should_exit, std::string& target, std::vector<std::string>& args);

	void get_sampling_parameters(const std::string& protocol, bool& should_exit, std::string& path, std::vector<std::string>& args);

	/* Returns a (filtered) password pattern */
	std::string get_password_pattern(bool& should_exit);

	/* Returns a path given by the user
		if file_number is non-negative, it will be included on the prompt
		If test_size is true, then the size argumet is updated, and the function ensures the file was non empty and readble
		If absolute_path is true, the the return value is the absolute path*/
	std::string get_file_path(int file_number, bool& should_exit, uint64_t& size, bool test_size=true, bool absolute_path=true);

	/* Fills the start and end parameters, they are now larger then total upper bound */
	void get_enumeration_bounds(bool& should_exit, uint64_t& start, uint64_t& end, uint64_t total_upper_bound=UINT64_MAX);

	bool check_for_help_or_exit(const std::string& reply, const std::string& help_reply, bool& should_exit);



	/*---------------Replies and prompts-------------------*/


	const std::string BEGIN_PROMPT("Passwords Enumeration");
	const std::string PROMPT_REPLY(">>> ");
	const std::string EXIT_REQUEST("exit");
	const std::string HELP_REQUEST("--help");
	const std::string PATTERN_HELP("The password pattern is a non-empty string describing how to build the passwords.\nThere are a few special token representing sets:\n\t'f' - a word from a file.\n\t'c' - a lowercase english letter.\n\t'C' - an uppercase English letter.\n\t'D' - a decimal digit.\n\t'p' - a punctuation mark.\nIf you want to specify your own set of characters, place it in between brackets without any separators: { }.\nNote that if you want to include the } character, you should place it in the end of the set (e.g. {0}} ).\n\nFor example, the string \"09!\" matches the patterns \"{01}dp\" and \"dd{!?}\" but it does not match \"pdC\" or \"ddpp\".");
	const std::string FUNCTION_HELP("The function you want to inverse.\nThe supported hash functions are:\n\tKeccak-256\n\tMD5\n\tSHA1\n\tSHA2-256\n\tSHA2-384\n\tSHA2-512\n\tSHA3-256\n\tSHA3-384\n\tSHA3-512\n\tWhirlpool\nTo enumerate with samples taken from Hash-to-Element algorithm in the SAE protocol choose:\n\tSAE ECC\n\tSAE FFC");
	const std::string BOUNDS_HELP("Set the bounds of the enumeration using lexicorgaphic ordering of the pattern (where files are indexed by bytes).\nWrite the lower bound and then the upperbound, or simply write \"all\" to enumerate all passwords.");
	const std::string HASH_HELP("A hash value should be given as hexadecimal digits.");
	const std::string ECC_HELP("The possible ECC groups are group19, group20 and group21");
	const std::string FFC_HELP("The possible FFC groups are group22, group23 and group24");
	const std::string NO_HELP("--help is not supported here.");
	const std::string WAIT_MSG("Please wait. This might take a while...");
}


#endif