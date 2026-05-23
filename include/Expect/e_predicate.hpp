#pragma once

#ifndef A_PRED_H
#define A_PRED_H

#include "../Runner.hpp"
#include "../Helpers.hpp"
#include "../Core.hpp"

#define EXPECT_ALL(container, condition) \
    internal::Assert::expectAllOf((container), (condition), __FILE__, __LINE__)

#define EXPECT_SOME(container, condition) \
    internal::Assert::expectAnyOf((container), (condition), __FILE__, __LINE__)

#define EXPECT_NONE(container, condition) \
    internal::Assert::expectNoneOf((container), (condition), __FILE__, __LINE__)

namespace internal {
    namespace Assert {
        template <typename T, typename Func>
        inline void expectAllOf(const T& t, const Func& func, const char* file, const int line) {
            if (!std::all_of(std::begin(t), std::end(t), func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });
            }
        }

        template <typename T, size_t N, typename Func>
        inline void expectAllOf(const T(& t)[N], const Func& func, const char* file, const int line) {
            if (!std::all_of(t, t + N, func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });

            }
        }

        template <typename T, typename Func>
        inline void expectAnyOf(const T& t, const Func& func, const char* file, const int line) {
            if (!std::any_of(std::begin(t), std::end(t), func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });

                 ;
            }
        }

        template <typename T, size_t N, typename Func>
        inline void expectAnyOf(const T(& t)[N], const Func& func, const char* file, const int line) {
            if (!std::any_of(t, t + N, func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });

    
            }
        }

        template <typename T, typename Func>
        inline void expectNoneOf(const T& t, const Func& func, const char* file, const int line) {
            if (!std::none_of(std::begin(t), std::end(t), func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });

         
            }
        }

        template <typename T, size_t N, typename Func>
        inline void expectNoneOf(const T(& t)[N], const Func& func, const char* file, const int line) {
            if (!std::none_of(t, t + N, func)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Not every element satisfied the passed in condition",
                    file,
                    line
                });


            }
        }
    }
}

#endif