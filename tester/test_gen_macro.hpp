#ifndef TEST_GEN_MACRO_H
#define TEST_GEN_MACRO_H

#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define TEST(testName) \
    void testName(); /* 1. Forward declaration */ \
    /* 2. Create a global object. Its constructor registers 'testName' */ \
    static MyTester::TestCase CONCAT(test_obj_, __LINE__)(#testName, testName); \
    void testName() /* 3. Begin the function body */

#endif