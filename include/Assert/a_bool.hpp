#pragma once

#ifndef A_BOOL_H
#define A_BOOL_H

#include "../Runner.hpp"
#include "../Core.hpp"

#define ASSERT_TRUE(cond) internal::Assert::expectTrue((cond), #cond, __FILE__, __LINE__)
#define ASSERT_FALSE(cond) internal::Assert::expectFalse((cond), #cond, __FILE__, __LINE__)

namespace internal {
    namespace Assert {
        /// @brief Test for expecting a condition to be true
        /// @param condition a condition
        /// @param expr The expression being evaluated in char* form
        /// @param file The file the condition is in
        /// @param line The line the condition is in
        inline void expectTrue(bool condition, const char* expr, const char* file, int line) 
        {
            if (!condition) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: " + std::string(expr) + " to be true",
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting a condition to be false
        /// @param condition a condition
        /// @param expr The expression being evaluated in char* form
        /// @param file The file the condition is in
        /// @param line The line the condition is in
        inline void expectFalse(bool condition, const char* expr, const char* file, int line) 
        {
            if (condition) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: " + std::string(expr) + " to be false",
                    file,
                    line
                });
                throw Core::AssertionFailure();
            }
        }
    }
}

#endif