
project(Tau-project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tau-project main.cpp parser_main.h parser_main.cpp parser_token.h parser_token.cpp scheme.h scheme.cpp worker.h worker.cpp worker_manager.h worker_manager.h worker_manager_worker_object.h worker_manager_worker_object.cpp)
