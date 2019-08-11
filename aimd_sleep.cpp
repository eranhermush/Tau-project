#include "aimd_sleep.h"

AIMD_Sleep::AIMD_Sleep(unsigned int min_sleep, unsigned int max_sleep, unsigned int add_inc,
			unsigned int mul_dec, unsigned int additive_threshold, unsigned int max_sleep_before_stop){
	if(min_sleep > 1){
		min = min_sleep < 999999 ? min_sleep : 999999;
	}
	else{
		min = 1;
	}
	if(max_sleep > min){
		max = max_sleep < 999999 ? max_sleep : 999999;
	}
	else{
		max = min;
	}
	if(min <= additive_threshold && additive_threshold <= max){
		threshold = additive_threshold;
	}
	else{
		threshold = min;
	}
	add = add_inc > 1 ? add_inc : 1;
	mul = mul_dec > 2 ? mul_dec : 2;
	max_long_sleeps = max_sleep_before_stop > 1 ? max_sleep_before_stop : 1; // allow for at lest one sleep
	sleep_time = min;
	count_long_sleeps = 0;
}


void AIMD_Sleep::inc(){
	if(sleep_time >= max){
		return;
	}
	if(sleep_time >= threshold){
		sleep_time = std::min(max, sleep_time + add);
		return;
	}
	if(sleep_time < threshold){
		sleep_time = std::min(max, sleep_time * 2);
		return;
	}

}

void AIMD_Sleep::dec(){
	count_long_sleeps = 0;
	if(sleep_time <= min){
		return;
	}
	sleep_time = std::max(min, sleep_time/mul);
}

void AIMD_Sleep::sleep(){
	usleep(sleep_time);
	if(sleep_time >= max && count_long_sleeps < max_long_sleeps){
		++count_long_sleeps;
	}
}


void AIMD_Sleep::set_sleep_time(unsigned int new_sleep_time){
	if(min <= new_sleep_time && new_sleep_time <= max){
		sleep_time = new_sleep_time;
	}
}


void AIMD_Sleep::set_bounds(unsigned int new_min_sleep, unsigned int new_max_sleep, unsigned int new_threshold){
	// 0 < min <= threshold <= max < 10^6
	if(0 < new_min_sleep && new_min_sleep <= new_threshold && new_threshold <= new_max_sleep && new_max_sleep < 1000000){
		min = new_min_sleep;
		threshold = new_threshold;
		if(new_max_sleep > max){
			count_long_sleeps = 0;
		}
		max = new_max_sleep;
	}
}