#pragma once

#include <string>
#include <functional>
#include <vector>

namespace internal::Core {
    using namespace std;

    enum class Status {
        Passed,
        Failed
    };

    struct Test {
        string suite_name;
        std::string test_name;

        function<void()> test;
    };

    struct Failure {
        string message;
        string file;
        int line;
    };

    struct Result {
        string suite_name;
        string test_name;

        vector<Failure> failures;

        Status status;
        double durationMs = 0.0;
    };

    struct TestRun {
        vector<Result> results;

        int total = 0;
        int passed = 0;
        int failed = 0;
        int skipped = 0;

        double totalMs = 0.0;
    };

    struct TestHash {
        size_t operator()(const Test& t) {
            size_t h1 = hash<string>()(t.suite_name);
            size_t h2 = hash<string>()(t.test_name);
            return h1 ^ (h2 << 1);
        }
    };

    struct AssertionFailure : public std::exception {
        const char* what() const noexcept override {
            return "Assertion failed";
        }
    };
}