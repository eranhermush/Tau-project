#include <iostream>
#include "char_password_generator.h"
#include "file_password_generator.h"
 
 int main(){
 	Char_Pattern_Password_Generator cpg("xdxx",{0,1,0},{{'#'}, {'0','2','4','6','8'}}, 0, 49);
 	std::cout << "created pattern generator" << std::endl;
 	Char_Pattern_Password_Generator cpg2(cpg);
 	std::cout << "copied pattern generator" << std::endl;
 	while(!cpg.is_over() && !cpg2.is_over()){
 		std::cout << "original: " << cpg.get_password() << std::endl;
 		std::cout << "copy: " << cpg2.get_password() << std::endl;
 		cpg.advance_password();
 		cpg2.advance_password(); 		
 	}

 	File_Password_Generator fpg("./examplefile.txt", 0, 5000);
 	std::cout << "created file generator" << std::endl;
 	File_Password_Generator fpg2(fpg);
 	std::cout << "copied file generator" << std::endl;
 	while(!fpg.is_over() && !fpg2.is_over()){
 		std::cout << "original: " << fpg.get_password() << std::endl;
 		std::cout << "copy: " << fpg2.get_password() << std::endl;
 		fpg.advance_password();
 		fpg2.advance_password(); 		
 	}
 	return 0;
 }