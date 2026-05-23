#pragma once

#ifndef A_CMP_H
#define A_CMP_H

#include "../Runner.hpp"
#include "../Helpers.hpp"
#include "../Core.hpp"

#define ASSERT_EQ(a, b) internal::Assert::expectEqual((a), (b), __FILE__, __LINE__)
#define ASSERT_NE(a, b) internal::Assert::expectNotEqual((a), (b), __FILE__, __LINE__)
#define ASSERT_LT(a, b) internal::Assert::expectLessThan((a), (b), __FILE__, __LINE__)
#define ASSERT_LE(a, b) internal::Assert::expectLessThanEqual((a), (b), __FILE__, __LINE__)
#define ASSERT_GT(a, b) internal::Assert::expectGreaterThan((a), (b), __FILE__, __LINE__)
#define ASSERT_GE(a, b) internal::Assert::expectGreaterThanEqual((a), (b), __FILE__, __LINE__)

namespace internal {
    namespace Assert {
        /// @brief Test for expecting a two values to be equal
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectEqual(const T& a, const U& b, const char* file, int line) 
        {
            if (!(a == b)) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a == b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting a two values to be not equal
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectNotEqual(const T& a, const U& b, const char* file, int line) 
        {
            if (a == b) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a != b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting one value to be strictly less than the other
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectLessThan(const T& a, const U&b, const char* file, int line) 
        {
            if (!(a < b)) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a < b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting one value to be less than or equal to the other
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectLessThanEqual(const T& a, const U&b, const char* file, int line) 
        {
            if (!(a <= b)) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a <= b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting one value to be strictly greater than the other
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectGreaterThan(const T& a, const U&b, const char* file, int line) 
        {
            if (!(a > b)) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a > b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }

        /// @brief Test for expecting one value to be greater than or equal to the other
        /// @tparam T A generic type
        /// @tparam U A generic type 
        /// @param a a T
        /// @param b a U
        /// @param file The file the test is in
        /// @param line The line the test is on
        template <typename T, typename U>
        inline void expectGreaterThanEqual(const T& a, const U&b, const char* file, int line) 
        {
            if (!(a >= b)) {
                std::string aStr = Helpers::toString(a);
                std::string bStr = Helpers::toString(b);
                Runner::CURRENT_TEST->failures.push_back({
                    "Expected: a >= b \n      a = " + aStr + "\n      b = " + bStr,
                    file,
                    line
                });

                throw Core::AssertionFailure();
            }
        }
    }
}

#endif