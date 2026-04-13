#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <iostream>

namespace MyTester {
    inline void runAllTests() {
        int totalTests = g_tests.size();
        std::cout << "Total Tests: " << totalTests << '\n';

        int passed = 0;
        for ( int i = 0; i < totalTests; ++i ) {
            std::cout << "\n================= Executing Test: " << i+1 << " / " << totalTests << " =================\n";
            passed += g_tests[i].get().run();
            std::cout << "=============================================================\n";
        }

        std::cout << "Summary: \n";
        std::cout << "Passed: " << passed << " | Failed: " << totalTests - passed << '\n';
    }
}

#endif