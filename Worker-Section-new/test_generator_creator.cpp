#include "generator_creator.h"
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

int main(){
	// these files should be in the dir
	// ./digits.txt should contain all decimal digits (in order) with \n after each but the last (20-1 =19B total)
	std::string path1("./digits.txt");
	// ./digits.txt should contain all 2-digits decimal numbers (in order) with \n after each (300-1 =299B total)
	std::string path2("./doubledigits.txt");
	std::vector<std::string> paths;
	std::unique_ptr<Password_Generator> pg_ptr;
	File_Password_Generator* fpg_ptr;

	//testing simple char patterns
	//test 1
	pg_ptr = Generator_By_Pattern::create_generator("CCc", paths, 704, 780);
	if(!pg_ptr){
		std::cout << "CCc pattern failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"BBc") != 0){
			std::cout << "CCc pattern failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"BEa") != 0){
			std::cout << "CCc pattern failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(730);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,3,"BCc") != 0){
			std::cout << "CCc pattern failed middle point" << std::endl;
		}
	}
	

	//test 2
	pg_ptr = Generator_By_Pattern::create_generator("{}C45Coc0", paths, 704, 780);
	if(!pg_ptr){
		std::cout << "CCc pattern with noise failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"BBc") != 0){
			std::cout << "CCc pattern with noise failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"BEa") != 0){
			std::cout << "CCc pattern with noise failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(730);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,3,"BCc") != 0){
			std::cout << "CCc pattern with noise failed middle point" << std::endl;
		}		
	}

	//test 3
	pg_ptr = Generator_By_Pattern::create_generator("{01}{23}{45}", paths, 1, 5);
	if(!pg_ptr){
		std::cout << "{01}{23}{45} pattern failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"025") != 0){
			std::cout << "{01}{23}{45} pattern failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"125") != 0){
			std::cout << "{01}{23}{45} pattern failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(2);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,3,"034") != 0){
			std::cout << "{01}{23}{45} pattern failed middle point" << std::endl;
		}
	}



	//test 4
	pg_ptr = Generator_By_Pattern::create_generator("{010}{}{323}tl{45}q", paths, 1, 5);
	if(!pg_ptr){
		std::cout << "{01}{23}{45} pattern with noise failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"025") != 0){
			std::cout << "{01}{23}{45} pattern with noise failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"125") != 0){
			std::cout << "{01}{23}{45} pattern with noise failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(2);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,3,"034") != 0){
			std::cout << "{01}{23}{45} pattern with noise failed middle point" << std::endl;
		}
	}


	//test 5
	pg_ptr = Generator_By_Pattern::create_generator("{fxcpC}{ffp}}}}}}x", paths, 2, 182);
	if(!pg_ptr){
		std::cout << "{fxcpC}{fp}} pattern with noise failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,2,"C}") != 0){
			std::cout << "{fxcpC}{fp}} pattern with noise failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,2,"x}") != 0){
			std::cout << "{fxcpC}{fp}} pattern with noise failed ending point when given end was wrong" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(3);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,2,"cf") != 0){
			std::cout << "{fxcpC}{fp}} pattern with noise failed middle point" << std::endl;
		}
	}


	//test 6
	pg_ptr = Generator_By_Pattern::create_generator("{0}{01}{01}{10}{10}", paths, 0, 100000);
	if(!pg_ptr){
		std::cout << "{0}{01}{01}{10}{10} pattern failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,5,"00000") != 0){
			std::cout << "{0}{01}{01}{10}{10} pattern failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,5,"01111") != 0){
			std::cout << "{0}{01}{01}{10}{10} patter failed ending point when given end was wrong" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(3);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,5,"00011") != 0){
			std::cout << "{0}{01}{01}{10}{10} pattern failed middle point" << std::endl;
		}
		pg_ptr->set_password(20);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,5,"00011") != 0){
			std::cout << "{0}{01}{01}{10}{10} pattern failed to ignore out of bounds index" << std::endl;
		}
	}


	std::cout << "finished testing char pattern generators" << std::endl;




	//testing file patterns
	//test 1
	pg_ptr = Generator_By_Pattern::create_generator("f", paths, 0, 1);
	if(pg_ptr){
		std::cout << "f pattern failed to return nullptr on missing path" << std::endl;
	}

	//test 2
	paths.push_back("./this/is/not/a/real/path");
	pg_ptr = Generator_By_Pattern::create_generator("f", paths, 0, 1);
	if(pg_ptr){
		std::cout << "f pattern failed to return nullptr on erronous path" << std::endl;
	}

	//test 3
	paths[0] = path1;
	pg_ptr = Generator_By_Pattern::create_generator("f", paths, 0, 18);
	if(!pg_ptr){
		std::cout << "f pattern " << paths[0] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,1,"0") != 0){
			std::cout << "f pattern " << paths[0] << " failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,1,"9") != 0){
			std::cout << "f pattern " << paths[0] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(4);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,1,"2") != 0){
			std::cout << "f pattern " << paths[0] << " failed middle point" << std::endl;
		}
		fpg_ptr = static_cast<File_Password_Generator*>(pg_ptr.get());
		if(fpg_ptr->file_error()){
			std::cout << "f pattern " << paths[0] << " had file error" << std::endl;
		}
	}

	//test 4
	paths[0] = path2;
	pg_ptr = Generator_By_Pattern::create_generator("f", paths, 0, 5);
	if(!pg_ptr){
		std::cout << "f pattern " << paths[0] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,2,"00") != 0){
			std::cout << "f pattern " << paths[0] << " failed starting point" << std::endl;
		}
		pg_ptr->expand_to_max_range();
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,2,"99") != 0){
			std::cout << "f pattern " << paths[0] << " failed ending point after expansion" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(37);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,1,"2") != 0){
			std::cout << "f pattern " << paths[0] << " failed middle point in middle of word" << std::endl;
		}
		fpg_ptr = static_cast<File_Password_Generator*>(pg_ptr.get());
		if(fpg_ptr->file_error()){
			std::cout << "f pattern " << paths[0] << " had file error" << std::endl;
		}
	}


	paths.clear();

	std::cout << "finished testing file generators" << std::endl;



	//testing complex patterns
	//test 1
	pg_ptr = Generator_By_Pattern::create_generator("ddf", paths, 0, 1);
	if(pg_ptr){
		std::cout << "ddf pattern failed to return nullptr on missing path" << std::endl;
	}

	//test 2
	paths.push_back("./this/is/not/a/real/path");
	pg_ptr = Generator_By_Pattern::create_generator("fdd", paths, 0, 1);
	if(pg_ptr){
		std::cout << "fdd pattern failed to return nullptr on erronous path" << std::endl;
	}
	paths.clear();

	//test 3
	paths.push_back(path1);
	pg_ptr = Generator_By_Pattern::create_generator("df{fd}", paths, 0, 100);
	if(!pg_ptr){
		std::cout << "df{fd} pattern " << paths[0] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"00d") != 0){
			std::cout << "df{fd} pattern " << paths[0] << " failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"26d") != 0){
			std::cout << "df{fd} pattern " << paths[0] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(15);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,2,"0f") != 0){
			std::cout << "df{fd} pattern " << paths[0] << " failed middle point" << std::endl;
		}
	}

	//test 4
	pg_ptr = Generator_By_Pattern::create_generator("}}}}xgdf{fd}{{{{{", paths, 0, 100);
	if(!pg_ptr){
		std::cout << "df{fd} pattern with noise " << paths[0] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"00d") != 0){
			std::cout << "df{fd} pattern with noise " << paths[0] << " failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"26d") != 0){
			std::cout << "df{fd} pattern with noise " << paths[0] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(15);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,2,"0f") != 0){
			std::cout << "df{fd} pattern with noise " << paths[0] << " failed middle point" << std::endl;
		}
	}

	//test 5
	paths.push_back(path2);
	pg_ptr = Generator_By_Pattern::create_generator("}}}}xgdf{fd}{{{{{", paths, 0, 100);
	if(!pg_ptr){
		std::cout << "df{fd} pattern with noise and extra paths " << paths[0] << " " << paths[1] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,3,"00d") != 0){
			std::cout << "df{fd} pattern with noise and extra paths " << paths[0] << " " << paths[1] << " failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,3,"26d") != 0){
			std::cout << "df{fd} pattern with noise and extra paths " << paths[0] << " " << paths[1] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(15);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,2,"0f") != 0){
			std::cout << "df{fd} pattern with noise and extra paths " << paths[0] << " " << paths[1] << " failed middle point" << std::endl;
		}
	}

	//test 6
	pg_ptr = Generator_By_Pattern::create_generator("{fppf}pffp", paths, 0, 995039);
	if(!pg_ptr){
		std::cout << "{fp}pffp pattern " << paths[0] << " " << paths[1] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,6,"f!000!") != 0){
			std::cout << "{fp}pffp pattern " << paths[0] << " " << paths[1] << "failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,6,"f&499~") != 0){
			std::cout << "{fp}pffp pattern " << paths[0]  << " " << paths[1] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(1230);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,6,"f!0/") != 0){
			std::cout << "{fp}pffp pattern " << paths[0]  << " " << paths[1] << " failed middle point" << std::endl;
		}
		pg_ptr->set_password(185);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,4,"f!0^") != 0){
			std::cout << "{fp}pffp pattern " << paths[0]  << " " << paths[1] << " failed middle of word" << std::endl;
		}
	}
	paths.clear();

	//test 7
	paths.push_back(path1);
	paths.push_back(path1);
	paths.push_back(path1);
	pg_ptr = Generator_By_Pattern::create_generator("qqq{01}fff{10}{opop{{{{fnguihfi", paths, 0, 2000);
	if(!pg_ptr){
		std::cout << "{01}fff{10} with noise " << paths[0] << " " << paths[1] << " " << paths[2] << " failed creation" << std::endl;
	}
	else{
		if(pg_ptr->get_password().compare(0,5,"00000") != 0){
			std::cout << "{01}fff{10} with noise " << paths[0] << " " << paths[1] << " " << paths[2] <<  "failed starting point" << std::endl;
		}
		while(pg_ptr->has_next()){
			pg_ptr->advance_password();
		}
		if(pg_ptr->get_password().compare(0,5,"01760") != 0){
			std::cout << "{01}fff{10} with noise " << paths[0]  << " " << paths[1] << " " << paths[2] << " failed ending point" << std::endl;
		}
		pg_ptr->advance_password();
		pg_ptr->set_password(485);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,5,"00671") != 0){
			std::cout << "{01}fff{10} with noise " << paths[0]  << " " << paths[1] << " " << paths[2] << " failed middle point" << std::endl;
		}
		pg_ptr->expand_to_max_range();
		pg_ptr->set_password(27300);
		if(pg_ptr->is_over() || pg_ptr->get_password().compare(0,4,"1940") != 0){
			std::cout << "{01}fff{10} with noise " << paths[0]  << " " << paths[1] << " " << paths[2] << " failed middle of word" << std::endl;
		}
	}

	std::cout << "finished testing complex generators" << std::endl;

	return 0;
}