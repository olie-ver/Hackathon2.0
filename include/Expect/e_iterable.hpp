#pragma once

#ifndef E_ITER_H
#define E_ITER_H

#include "../Runner.hpp"
#include "../Helpers.hpp"
#include "../Concepts.hpp"
#include <sstream>
#include <type_traits>
#include <iterator>

#define EXPECT_EQ_ORDERED(a, b) internal::Expect::expectOrderedRangeEq((a), (b), __FILE__, __LINE__)

namespace internal {
    namespace Expect {

        template <typename T, size_t N>
        inline void expectOrderedRangeEq(T(& a)[N], T(& b)[N], const char* file, int line) {
            // static_assert(Concepts::IterableComparable<A, B>::value, 
            //     "Both containers must be iterable and their content must be comparable");

            std::stringstream stream;
            bool mismatch = false;
            for (size_t i = 0; i < N; i++) {
                if (!(a[i] == b[i])) {
                    stream << "     Mismatched element at index " << i;
                    stream << "\n     a[" << i << "]: " << Helpers::toString(a[i]);
                    stream << "\n     b[" << i << "]: " << Helpers::toString(b[i]);
                    mismatch = true;
                }
            }

            if (mismatch) {
                Runner::CURRENT_TEST->failures.push_back({
                    stream.str(),
                    file,
                    line
                });
            }
        }

        template <typename A, typename B>
        inline void expectOrderedRangeEq(const A& a, const B& b, const char* file, int line)
        {
            // static_assert(Concepts::IterableComparable<A, B>::value, 
            //     "Both containers must be iterable and their content must be comparable");

            auto a_itr = std::begin(a);
            auto b_itr = std::begin(b);

            auto a_end = std::end(a);
            auto b_end = std::end(b);
            if (std::distance(a_itr, a_end) != std::distance(b_itr, b_end)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Size of collections are not the same",
                    file,
                    line
                });
                return;
            }
            size_t index = 0;
            for (; a_itr != a_end; ++a_itr, ++b_itr, ++index)
            {
                auto&& a_val = *a_itr;
                auto&& b_val = *b_itr;

                if (!(a_val == b_val)) {
                    std::string idx = Helpers::toString(index);
                    Runner::CURRENT_TEST->failures.push_back({
                        std::string("Mismatch at index = ") + idx
                        + "\n    a[" + idx + "]: " + Helpers::toString(a_val)
                        + "\n    b[" + idx + "]: " + Helpers::toString(b_val),
                        file,
                        line
                    });
                }
            }
        }

        template <typename A, typename B>
        inline void expectUnorderedRangeEq(const A& a, const B& b, const char* file, int line) {
            // static_assert(Concepts::IterableComparable<A, B>::value, 
            //     "Both containers must be iterable and their content must be comparable");

            if (std::size(a) != std::size(b)) {
                Runner::CURRENT_TEST->failures.push_back({
                    "Collection sizes aren't equivalent",
                    file,
                    line
                });
            }

            //since this function is being called, a and b have the same size
            std::vector<bool> used(std::size(a));

            auto a_itr = std::begin(a);

            auto a_end = std::end(a);
            auto b_end = std::end(b);

            for (; a_itr != a_end; ++a_itr) {
                size_t idx = 0;
                auto b_itr = std::begin(b);
                bool found = false;
                for (; b_itr != b_end; ++b_itr, ++idx) {
                    if (*b_itr == *a_itr) {
                        if (!used[idx]) {
                            used[idx] = true;
                            found = true;
                            break;
                        }
                    } 
                }

                if (!found) {
                    Runner::CURRENT_TEST->failures.push_back({
                        std::string("Missing element: ") + Helpers::toString(*a_itr),
                        file,
                        line
                    });
                }
            }

            for (size_t i = 0; i < used.size(); i++) {
                if (!used[i]) {
                    Runner::CURRENT_TEST->failures.push_back({
                        "Collections not equivalent",
                        file,
                        line
                    });
                }
            }
        }
    }
}

#endif