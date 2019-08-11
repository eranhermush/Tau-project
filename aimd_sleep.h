#ifndef TAU_PROJECT_AIMD_SLEEP
#define TAU_PROJECT_AIMD_SLEEP

#include <unistd.h>
#include <algorithm>

/* This Class implements Additive-Increase/Multiplicative-Decrease sleep management.
	
	The time units are --microseconds-- and should be in the no greater than 999999 = 10^6-1.

	The algorithm uses 3 time constants 0 < min_t <= threshold <= max_t < 10^6, and 2 arithmatic constants
	M>1 and A>0. Suppose the current sleeping time is t:

	Decrease(t) = maximun(t/M, min_t) 										(this is a multiplicative decrease)

	if min_t <= t < threshold: Increase(t) = minimum(2t, max_t) 			(Before the threshold, increare multiplicativly)
	if threshold < t <= max_t: Increase(t) = minimum(t+A, max_t)			(After the threshold, increace additivly)

	Use the sleep() function to sleep t microseconds.
	Remember to call inc() after a wasted wakeup, and dec() after a useful wakeup.

*/



class AIMD_Sleep{
	public:
		/* Constructor - constructs a sleep manager with the parameters as explained above.
			Time units are in microseconds, and between 0 and 10^6 (excluding).

			Use max_sleep_before_stop to specify when the should_stop() function return "true"
			After max_sleep_before_stop consecutive sleeps of max_sleep microseconds, the functions returns true.*/
		AIMD_Sleep(unsigned int min_sleep, unsigned int max_sleep, unsigned int add_inc,
			unsigned int mul_dec, unsigned int additive_thershold, unsigned int max_sleep_before_stop);

		~AIMD_Sleep() =default;

		void inc();

		void dec();

		/* Sleeps sleep_time microseconds, using usleep */
		void sleep();

		/* Sets the sleep time if it is in the bounds */
		void set_sleep_time(unsigned int new_sleep_time);

		void set_bounds(unsigned int new_min_sleep, unsigned int new_max_sleep, unsigned int new_threshold);

		unsigned int get_sleep_time() const{
			return sleep_time;
		}

		bool should_stop() const{
			return max_long_sleeps <= count_long_sleeps;
		}



	private:
		unsigned int sleep_time;
		unsigned int min;
		unsigned int max;
		unsigned int add;
		unsigned int mul;
		unsigned int threshold;
		unsigned int count_long_sleeps;
		unsigned int max_long_sleeps;
};


#endif