#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace cg::utils
{
	class timer {
    public:
        explicit timer(const std::string& message) : event(message)
        {
            start = std::chrono::high_resolution_clock::now();
        }
        ~timer() 
        {
            auto stop = std::chrono::high_resolution_clock::now();
	        std::chrono::duration<float, std::milli> duration = stop - start;
	        std::cout << event << "- took " << duration.count() << "ms\n";
        }
    private:
        std::string event;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    };
}
