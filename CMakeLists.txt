cmake_minimum_required(VERSION 2.8)
project(Tau-project)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)


add_executable(EranTester tester_for_fileObject.cpp file_object.h file_object.cpp file_manager.h file_manager.cpp parser_main.h parser_main.cpp worker_manager_worker_object.h worker_manager_worker_object.cpp worker_manager.h worker_manager.cpp another_functions.h another_functions.cpp helpful_functions.h helpful_functions.cpp index_vector_convertor.h index_vector_convertor.cpp)

add_executable(Worker_new_run Worker-Section-new/client_main.cpp file_object.h file_object.cpp parser_main.h parser_main.cpp another_functions.h another_functions.cpp helpful_functions.h helpful_functions.cpp index_vector_convertor.h index_vector_convertor.cpp Worker-Section-new/char_password_generator.h Worker-Section-new/char_password_generator.cpp Worker-Section-new/client.h Worker-Section-new/client.cpp Worker-Section-new/file_password_generator.h Worker-Section-new/file_password_generator.cpp Worker-Section-new/hash_functions.h Worker-Section-new/hash_functions.cpp Worker-Section-new/hash_matcher.h Worker-Section-new/password_generator.h Worker-Section-new/nested_password_generator.h Worker-Section-new/nested_password_generator.cpp Worker-Section-new/preimage_matcher.h Worker-Section-new/preimage_seeker.h Worker-Section-new/preimage_seeker.cpp Worker-Section-new/Id_Hash.h Worker-Section-new/Id_Hash.cpp Worker-Section-new/preimage_matcher_by_name.h Worker-Section-new/preimage_matcher_by_name.cpp  Worker-Section-new/kdf_sae.h Worker-Section-new/ffc_pwe_sample_matcher.h  Worker-Section-new/ecc_pwe_sample_matcher.h Worker-Section-new/pwe_sample.h Worker-Section-new/pwe_sample.cpp)


-DNDEBUG -g3 -O2 -Wall -Wextra -pthread -march=native -std=c++11  -l:libcryptopp.a   -lntl -lgmp -lm



g++ -o ServerFile tester_for_fileObject.cpp user_text_interface.h user_text_interface.cpp file_object.h file_object.cpp file_manager.h file_manager.cpp parser_main.h parser_main.cpp worker_manager_worker_object.h worker_manager_worker_object.cpp worker_manager.h worker_manager.cpp another_functions.h another_functions.cpp helpful_functions.h helpful_functions.cpp index_vector_convertor.h index_vector_convertor.cpp pattern_utils.h pattern_utils.cpp

g++ -o ServerFile_new main.cpp user_text_interface.h user_text_interface.cpp file_object.h file_object.cpp file_manager.h file_manager.cpp helpful_functions.h helpful_functions.cpp pattern_utils.h pattern_utils.cpp


g++ -o Worker_new_run Worker-Section-new/client_main.cpp file_object.h file_object.cpp helpful_functions.h helpful_functions.cpp Worker-Section-new/char_password_generator.h Worker-Section-new/char_password_generator.cpp Worker-Section-new/client.h Worker-Section-new/client.cpp Worker-Section-new/file_password_generator.h Worker-Section-new/file_password_generator.cpp Worker-Section-new/hash_functions.h Worker-Section-new/hash_functions.cpp Worker-Section-new/hash_matcher.h Worker-Section-new/password_generator.h Worker-Section-new/nested_password_generator.h Worker-Section-new/nested_password_generator.cpp Worker-Section-new/preimage_matcher.h Worker-Section-new/preimage_seeker.h Worker-Section-new/preimage_seeker.cpp Worker-Section-new/Id_Hash.h Worker-Section-new/Id_Hash.cpp Worker-Section-new/preimage_matcher_by_name.h Worker-Section-new/preimage_matcher_by_name.cpp  Worker-Section-new/kdf_sae.h Worker-Section-new/ffc_pwe_sample_matcher.h  Worker-Section-new/ecc_pwe_sample_matcher.h Worker-Section-new/pwe_sample.h Worker-Section-new/pwe_sample.cpp Worker-Section-new/generator_creator.h Worker-Section-new/generator_creator.cpp pattern_utils.h pattern_utils.cpp -DNDEBUG -g3 -O2 -Wall -Wextra -pthread -march=native -std=c++11 -l:libcryptopp.a -lntl -lgmp -lm

g++ -o Ecc speedtest_ecc_seeker.cpp preimage_seeker.cpp file_password_generator.cpp ecc_pwe_sample_matcher.h kdf_sae.h sae_groups.h pwe_sample.cpp password_generator.h -DNDEBUG -g3 -O2 -Wall -Wextra -pthread -march=native -std=c++11 -l:libcryptopp.a -lntl -lgmp -lm

g++ -o FFC speedtest_ffc_seeker.cpp preimage_seeker.cpp file_password_generator.cpp ecc_pwe_sample_matcher.h kdf_sae.h sae_groups.h pwe_sample.cpp password_generator.h char_password_generator.cpp ffc_pwe_sample_matcher.h  -DNDEBUG -g3 -O2 -Wall -Wextra -pthread -march=native -std=c++11 -l:libcryptopp.a -lntl -lgmp -lm
