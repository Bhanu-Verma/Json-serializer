#ifndef FAILURE_EXCEPTION_H
#define FAILURE_EXCEPTION_H

#include <stdexcept>
#include <sstream>

namespace MyTester {
    class TestFailureException : public std::runtime_error {
    public:
        TestFailureException(const std::string& msg) : std::runtime_error(msg) {}
    };
}

#endif