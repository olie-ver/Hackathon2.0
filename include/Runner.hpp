#pragma once

#include "Core.hpp"
#include <vector>
#include <unordered_set>

#define STR(x) #x

#define UNIQUE_NAME(base, line) base##_##line
#define MAKE_UNIQUE(base, line) UNIQUE_NAME(base, line)

#define TEST_IMPL(suite_name, test_name, line) \
    void suite_name##_##test_name(); \
    static bool MAKE_UNIQUE(register, line) = \
        internal::Runner::registerTest( \
            {STR(suite_name), STR(test_name), suite_name##_##test_name}); \
    void suite_name##_##test_name()

#define TEST(suite_name, test_name) \
    TEST_IMPL(suite_name, test_name, __LINE__)

#define D_TEST(test_name) \
    TEST(Default, test_name)

namespace internal::Runner {
    std::vector<Core::Test>& getRegistry();

    std::unordered_set<Core::Test, Core::TestHash>& getAllTests();

    bool registerTest(const Core::Test& test);

    void runAllRegisteredTests(Core::TestRun& run);

    Core::Result runTest(const Core::Test& test);
}