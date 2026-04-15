#ifndef ASSERT_THROWS_H
#define ASSERT_THROWS_H

#define ASSERT_THROWS(func, type) \
do{ \
    try{ \
        func; \
        std::stringstream ss; \
        ss << "(" << #func << " didn't throw an exception of type: " << #type << ") at " \
           << __FILE__ << ":" << __LINE__; \
        throw MyTester::TestFailureException(ss.str()); \
    } \
    catch(type){} \
} while (0);

#endif