#include "preimage_seeker.h"

Preimage_Seeker::Preimage_Seeker(const Password_Generator& gen, const Preimage_Matcher& match, bool should_reset)
	:generator(gen.clone()), matcher(match.clone()){
	if(should_reset){
		generator->reset();
	}
}

std::vector<std::string> Preimage_Seeker::seek_all(){
	std::vector<std::string> answer;

	while(!generator->is_over()){
		if(matcher->test(generator->get_password())){
			answer.push_back(generator->get_password());
		}
		generator->advance_password();
	}

	return answer;
}

std::string Preimage_Seeker::seek_next(bool& found){
	found = false;
	std::string answer;

	while(!generator->is_over() && !found){
		if(matcher->test(generator->get_password())){
			found = true;
			answer = generator->get_password();
		}
		generator->advance_password();
	}

	return answer;
}