#include "../include/Renderer.hpp"
#include <iostream>

namespace internal {
    namespace Renderer {
        void Renderer::render(Core::TestRun& testRun) {
            std::cout << "Running " << testRun.total << " tests...\n" << std::endl;

            auto& tests = testRun.results;

            size_t size = tests.size();
            // std::cout << "There are " << size << "tests " << std::endl;
            for (size_t i = 0; i < size; i++) {
                const Core::Result& result = tests[i];

                switch (result.status) {
                    case Core::Status::Passed:
                        std::cout << "[PASS] " << result.suite_name << " -> " << result.test_name;
                        std::cout << " (" << result.durationMs << " ms" << ')' << std::endl;
                        testRun.passed++;
                        break;
                
                    case Core::Status::Failed:
                        std::cout << "[FAIL] " << result.suite_name << " -> " << result.test_name;
                        std::cout << " (" << result.durationMs << " ms" << ')' << std::endl;

                        for (size_t j = 0; j < result.failures.size(); j++) {
                            const Core::Failure& fail = result.failures[j];
                            std::cout << "\tat: " << fail.file << ":" << fail.line << std::endl;
                            std::cout << '\t' << fail.message << std::endl;
                            std::cout << std::endl;
                        }
                        testRun.failed++;
                        break;
                }
            }

            std::cout << std::string(50, '-') << std::endl;
            std::cout << "Total: " << testRun.total;
            std::cout << " | Passed: " << testRun.passed;
            std::cout << " | Failed: " << testRun.failed;
            std::cout << "\nTime: " << testRun.totalMs << " ms" << std::endl;
        }
    }
}