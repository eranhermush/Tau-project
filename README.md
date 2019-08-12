# Tau-project
This is a password enumeration project for a course in Tel Aviv University.

It consists of a server with a text user interface, that manages the enumeration and communicates with the user;
And of workers who preform the enumeration.

The project supports various hash functions, and an enumeration over data collected from attacks of the SAE protocol password-to-element
functions as is presented in https://eprint.iacr.org/2019/383.
The code also includes an implementation of the necessary parts of these functions. These implementations are in the Worker directory, under files with prefixes such as ecc/ffc/sae/kdf . 

---------------------------------------------------------------------------------------------------------

The code uses to external static libraries:
	Crypto++ version 8.20 (Available at https://www.cryptopp.com/downloads.html#CRYPTOPP_820 )
	NTL version 11.3.2 by Victor Shoup (Available at https://www.shoup.net/ntl/download.html )

To install them where you don't have root access:
	For Crypto++: run make install with PREFIX=/path/to/install/in
	For NTL: run configure with REFIX=/path/to/install/in

Compilation flags for the libraries:
	Crypto++:   -DNDEBUG -g3 -O2 -Wall -Wextra -l:/path/to/install/in/lib/libcryptopp.a -I:/path/to/install/in/include
		or if you installed as root
				-DNDEBUG -g3 -O2 -Wall -Wextra -l:libcryptopp.a
	NTL: 	 -g -O2 -std=c++11 -pthread -march=native  -l:/path/to/install/in/lib/libntl.a -lgmp -lm -I:/path/to/install/in/inlcude
		or if you installed as root
			 -g -O2 -std=c++11 -pthread -march=native  -lntl -lgmp -lm


---------------------------------------------------------------------------------------------------------

To compile the server you can run:

	g++ -Wall -Wextra -o SERVER main.cpp user_text_interface.cpp file_object.cpp file_manager.cpp helpful_functions.cpp pattern_utils.cpp aimd_sleep.cpp -std=c++11

And to compile the worker:
	
	g++ -DNDEBUG -g3 -O2 -Wall -Wextra -o WORKER Worker/client_main.cpp file_object.cpp helpful_functions.cpp Worker/char_password_generator.cpp Worker/client.cpp Worker/file_password_generator.cpp Worker/hash_functions.cpp Worker/nested_password_generator.cpp Worker/preimage_seeker.cpp Worker/Id_Hash.cpp Worker/preimage_matcher_by_name.cpp Worker/pwe_sample.cpp Worker/generator_creator.cpp pattern_utils.cpp  -pthread -march=native -std=c++11 -l:/path/to/install/in/lib/libcryptopp.a -l:/path/to/install/in/lib/libntl.a -lgmp -lm -I/path/to/install/in/inlcude

To run multiple workers you can use the python script in ./Worker/create_workers.py with usage instructions in it.
