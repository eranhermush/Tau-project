#ifndef TAU_PROJECT_PREIMAGE_SEEKER_H
#define TAU_PROJECT_PREIMAGE_SEEKER_H

#include "preimage_matcher.h"
#include "password_generator.h"
#include <string>
#include <vector>
#include <memory>

class Preimage_Seeker{
	public:
		/* Constructor
			The preimage seeker will match strings from gen using match */
		Preimage_Seeker(const Password_Generator& gen, const Preimage_Matcher& match, bool should_reset=true);

		/* Returns all the generated strings that match */
		std::vector<std::string> seek_all();

		/* Returns fisrt generated string that match */
		std::string seek_next(bool& found);

		/* Returns whether the generator has generator all string */
		bool is_over(){
			return generator->is_over();
		}

		/* Resets the generator */
		void reset(){
			generator->reset();
		}

	private:
		std::unique_ptr<Password_Generator> generator;
		std::unique_ptr<Preimage_Matcher> matcher;
};


#endif