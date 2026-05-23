#include "Renderer.hpp"
#include <iostream>

namespace internal::Renderer {
    void Renderer::render(Core::TestRun& testRun) {
        auto& tests = testRun.results;

        size_t size = tests.size();
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
    }
}