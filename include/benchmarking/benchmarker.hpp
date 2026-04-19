#ifndef BENCHMARKER_H
#define BENCHMARKER_H

#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

class Benchmarker {
public:
    template<typename Func, typename... Args>
    static void run(std::string_view label, Func&& func, size_t iterations, Args&&... args ) {
        std::vector<double> timings;
        timings.reserve(iterations);

        // 1. Warm-up (get CPU cache ready and memory pages mapped)
        func(args...); 

        for (size_t i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            
            [[maybe_unused]] auto result = func(args...); // Execute the parser
            
            auto end = std::chrono::high_resolution_clock::now();
 
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            timings.push_back(static_cast<double>(dur.count()) / 1000.0); // Store as ms
        }

        // 2. Statistical Cleanup
        std::sort(timings.begin(), timings.end());
        double avg = std::accumulate(timings.begin(), timings.end(), 0.0) / iterations;
        
        std::cout << "--- " << label << " ---\n"
                  << "Min: " << timings.front() << "ms\n"
                  << "Max: " << timings.back() << "ms\n"
                  << "Avg: " << avg << "ms\n"
                  << "Median: " << timings[iterations / 2] << "ms\n" << std::endl;
    }
};

#endif