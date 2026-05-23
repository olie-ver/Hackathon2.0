#pragma once

#ifndef E_META_H
#define E_META_H

#include "../Runner.hpp"
#include "../Core.hpp"
#include <functional>

#define  EXPECT_PASSES(test) internal::Assert::expectPasses([&](){(test);}, #test, __FILE__, __LINE__)
#define  EXPECT_FAILS(test) internal::Assert::expectFails([&](){(test);}, #test, __FILE__, __LINE__)
#define  EXPECT_FAILS_MSG(test, msg) \
    internal::Assert::expectFailsWithMessage([&](){(test);}, #test, (msg), __FILE__, __LINE__)

namespace internal {
    namespace Assert {
        inline void expectPasses(const std::function<void()>& test, const char* testName, 
            const char* file, const int line) 
        {
            size_t oldSize = Runner::CURRENT_TEST->failures.size();

            //needs to take into account for things that throw => the expection tests
            try {
                test();
            } catch(...) {

            }

            //if the number of failures grew
            if (oldSize < Runner::CURRENT_TEST->failures.size()) {
                //Get the last failure
                Core::Failure prev = Runner::CURRENT_TEST->failures.back();
                //Delete it
                Runner::CURRENT_TEST->failures.pop_back();
                //Return a new one
                Runner::CURRENT_TEST->failures.push_back({
                    std::string("Expected test: ") + testName 
                    + " to pass, but it failed with message:\n"
                    + prev.message,
                    file,
                    line
                });
            }
        }

        inline void expectFails(const std::function<void()>& test, const char* testName, 
            const char* file, const int line) 
        {
            size_t oldSize = Runner::CURRENT_TEST->failures.size();

            try {
                test();
            } catch(...) {

            }

            //if the number of failures didn't change
            if (Runner::CURRENT_TEST->failures.size() == oldSize) {
                std::string error = std::string("Expected test: ") + testName + " to fail, but it passed";
                //modify the old failure message to use this function's failure message
                Runner::CURRENT_TEST->failures.push_back({
                    error,
                    file,
                    line
                });
            } else { //the test failed => this one passed => erase its error message
                Runner::CURRENT_TEST->failures.pop_back();
            }
        }

        inline void expectFailsWithMessage(const std::function<void()>& test, const char* testName, 
            const std::string& msg, const char* file, const int line) 
        {
            size_t oldSize = Runner::CURRENT_TEST->failures.size();

            try {
                test();
            } catch(...) {

            }

            //if the number of failures didn't change
            if (Runner::CURRENT_TEST->failures.size() == oldSize) {
                std::string error = std::string("Expected test: ") + testName + " to fail, but it passed";
                //modify the old failure message to use this function's failure message
                Runner::CURRENT_TEST->failures.push_back({
                    error,
                    file,
                    line
                });
            } else { //The test failed
                Core::Failure last = Runner::CURRENT_TEST->failures.back();

                Runner::CURRENT_TEST->failures.pop_back(); //delete the old failure

                if (msg != last.message) { //failed with wrong message
                    Runner::CURRENT_TEST->failures.push_back({
                        "Expected test to fail with message: \"" 
                        + msg
                        + "\"\nbut it failed with message: \""
                        + last.message + "\"",
                        file,
                        line
                    });
                }
            }
        }
    }
}

#endif