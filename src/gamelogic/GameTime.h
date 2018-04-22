#pragma once

#include <chrono>

class Time {
public:
	double elapsed(){
		return std::chrono::duration_cast<std::chrono::milliseconds>(t_1 - t_0).count() * 0.001;
	}
	void tick(){ // advance Time one frame
		t_0 = t_1;
		t_1 = std::chrono::high_resolution_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> t_0 = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> t_1 = std::chrono::high_resolution_clock::now();
};