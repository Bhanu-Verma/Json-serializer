#ifndef ASSERT_MACRO_H
#define ASSERT_MACRO_H

#define ASSERT(condition) \
do { \
    if (!(condition)) { \
        std::stringstream ss; \
        ss << "Assertion Failed: (" << #condition << ") at " \
           << __FILE__ << ":" << __LINE__; \
        throw MyTester::TestFailureException(ss.str()); \
    } \
} while (0)

#endif