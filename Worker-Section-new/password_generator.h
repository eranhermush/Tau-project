#ifndef TAU_PROJECT_PASSWORD_GENERATOR_H
#define TAU_PROJECT_PASSWORD_GENERATOR_H

#include <string>
#include <memory>
#include <cstdint>

/* Password_Generator is a basic interface for a password generator object */
class Password_Generator{
	public:

		virtual ~Password_Generator() =default;

		/* Returns a unique pointer to a new clone of the generator */
		virtual std::unique_ptr<Password_Generator> clone() const =0;

		/* Returns a reference to the curent password */
		virtual const std::string& get_password(){
			return curr_password;
		}

		/* Advances the current password, if possible */
		virtual void advance_password() =0;

		/* Sets the current password to be the one at position (position should be in the range)*/
		virtual void set_password(uint64_t position) =0;

		/* Returns whether the generator has another password to produce */
		virtual bool has_next() const =0;

		/* Returns whether the generator has finished will all passwords (== advanced from the last password) */
		virtual bool is_over() const{
			return is_over_flag;
		}

		/* Reverts the generator to its original state right after its construction */
		virtual void reset() =0;

		/* Expands the password range to the widest possible of the generator
			Does not change the current password */
		virtual void expand_to_max_range() =0;

		/* Returns whether the generator produces passwords of fixed length */
		virtual bool has_fixed_length() const =0;

		/* Returns the size of the range the generator works with */
		virtual uint64_t get_range_length() const =0;

		/* Returns the current position in the range (from 0 *not* from reset)*/
		virtual uint64_t get_curr_position() const =0;

	protected:
		std::string curr_password;
		bool is_over_flag = false;
};


#endif