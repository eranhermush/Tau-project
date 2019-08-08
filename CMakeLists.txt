cmake_minimum_required(VERSION 2.8)
project(Tau-project)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)


add_executable(EranTester tester_for_fileObject.cpp file_object.h file_object.cpp file_manager.h file_manager.cpp parser_main.h parser_main.cpp worker_manager_worker_object.h worker_manager_worker_object.cpp worker_manager.h worker_manager.cpp another_functions.h another_functions.cpp helpful_functions.h helpful_functions.cpp index_vector_convertor.h index_vector_convertor.cpp)

add_executable(Worker_new_run Worker-Section-new/client_main.cpp file_object.h file_object.cpp parser_main.h parser_main.cpp another_functions.h another_functions.cpp helpful_functions.h helpful_functions.cpp index_vector_convertor.h index_vector_convertor.cpp Worker-Section-new/char_password_generator.h Worker-Section-new/char_password_generator.cpp Worker-Section-new/client.h Worker-Section-new/client.cpp Worker-Section-new/file_password_generator.h Worker-Section-new/file_password_generator.cpp Worker-Section-new/hash_functions.h Worker-Section-new/hash_functions.cpp Worker-Section-new/hash_matcher.h Worker-Section-new/password_generator.h Worker-Section-new/nested_password_generator.h Worker-Section-new/nested_password_generator.cpp Worker-Section-new/preimage_matcher.h Worker-Section-new/preimage_seeker.h Worker-Section-new/preimage_seeker.cpp Worker-Section-new/Id_Hash.h Worker-Section-new/Id_Hash.cpp)
