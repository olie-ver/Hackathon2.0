#include "Core.hpp"
#include "Runner.hpp"

#include <iostream>

namespace internal::Runner {
    extern Core::Result* CURRENT_TEST;

    bool registerTest(const Core::Test &test)
        {
            auto& ALL_TESTS = getAllTests();

            auto [it, inserted] = ALL_TESTS.insert(test);
            if (!inserted) {
                std::cerr << "Test under suite: " << test.suite_name
                        << " has duplicate name: " << test.test_name << std::endl;
                std::abort();
            }

            std::vector<Core::Test>& REGISTRY = getRegistry();

            REGISTRY.push_back(test);
            return true;
        }

        void runAllRegisteredTests(Core::TestRun& run)
        {   
            using clock = std::chrono::steady_clock;
            using ms = std::chrono::milliseconds;

            clock::time_point start_time = clock::now();

            std::vector<Core::Test>& REGISTRY = getRegistry();

            clock::time_point end_time = clock::now();

            run.totalMs =  std::chrono::duration_cast<ms>(end_time - start_time).count();
        }

        Core::Result runTest(Core::Test& test) {
            Core::Result result;
            CURRENT_TEST = &result;
            CURRENT_TEST->suite_name = test.suite_name;
            CURRENT_TEST->test_name = test.test_name;
            CURRENT_TEST->status = Core::Status::Passed;

            using clock = std::chrono::steady_clock;
            using ms = std::chrono::milliseconds;

            clock::time_point start_time = clock::now();
        
            try {
                test.test();
                clock::time_point end_time = clock::now();

                CURRENT_TEST->durationMs =  std::chrono::duration_cast<ms>(end_time - start_time).count();

                if (!CURRENT_TEST->failures.empty()) {
                    CURRENT_TEST->status =  Core::Status::Failed;
                }

                return *CURRENT_TEST;
            } catch (...) { //we don't care what error gets thrown, we end the test
                clock::time_point end_time = clock::now();

                CURRENT_TEST->durationMs =  std::chrono::duration_cast<ms>(end_time - start_time).count();

                CURRENT_TEST->status =  Core::Status::Failed;

                return *CURRENT_TEST;
            }
        }
}