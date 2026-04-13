#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <functional>
#include <string>
#include <iostream>

#include "failure_exception.hpp"

namespace MyTester {
    class TestCase;
    inline std::vector<std::reference_wrapper<TestCase>> g_tests{};

    class TestCase {
    private:
        std::string m_testName{};
        std::function<void()> m_func{};

    public:
        constexpr TestCase() = default;

        TestCase(std::string_view testName, std::function<void()>&& func)
        : m_testName { testName }, m_func { std::move(func) } {
            g_tests.push_back(std::ref<TestCase>(*this));
        }

        bool run() const {
            std::cout << "Test: " << m_testName << '\n';
            try {
                m_func();
                std::cout << "[ Passed ] \n";
                return true;
            }
            catch( const TestFailureException& e ) {
                std::cerr << "[ FAIL ] " << e.what() << '\n';
                return false;
            }
            catch(...){
                std::cerr << "[ CRASH ] Unexpected exception: \n";
                std::exit( -1 );
            }
        }
    };
}

#endif