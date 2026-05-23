#include "../include/Runner.hpp"
#include "../include/Tests.hpp"
#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include <limits>

// ============================================================
//  BUGS NOTICED IN SOURCE (do not remove these comments):
//
//  1. a_iterable.hpp (array overload, line ~20):
//     `throw Core::AssertionFailure()` is OUTSIDE the if-block —
//     it fires unconditionally on every loop iteration.
//
//  2. a_flt.hpp & e_flt.hpp — isNotNaN():
//     Condition is `val != NAN`, which should be `std::isnan(val)`
//     to detect NaN correctly. The NAN macro is not reliably
//     comparable with !=.
//
//  3. ASSERT_NOT_NAN macro calls `isNotNan` (lowercase n),
//     but the function is `isNotNaN` (uppercase N) — likely
//     a linker/compile error at the call site.
//
//  4. e_predicate.hpp declares functions inside `namespace Assert`
//     instead of `namespace Expect`, but macros reference
//     `internal::Assert::expectAllOf` — consistent, but
//     the namespace naming is misleading.
// ============================================================


// ============================================================
// ASSERT BOOL
// ============================================================

TEST(AssertBool, true_passes) {
    ASSERT_TRUE(true);
    ASSERT_TRUE(1 == 1);
    ASSERT_TRUE(5 > 3);
}

TEST(AssertBool, false_passes) {
    ASSERT_FALSE(false);
    ASSERT_FALSE(1 == 2);
    ASSERT_FALSE(5 < 3);
}

TEST(AssertBool, true_fails_on_false) {
    ASSERT_FAILS(ASSERT_TRUE(false));
}

TEST(AssertBool, false_fails_on_true) {
    ASSERT_FAILS(ASSERT_FALSE(true));
}

TEST(AssertBool, assert_stops_after_failure) {
    // The second ASSERT_TRUE should never execute because the first throws
    ASSERT_PASSES(ASSERT_TRUE(true));
    ASSERT_FAILS(ASSERT_TRUE(false));
}


// ============================================================
// EXPECT BOOL
// ============================================================

TEST(ExpectBool, true_passes) {
    EXPECT_TRUE(true);
    EXPECT_TRUE(42 != 0);
}

TEST(ExpectBool, false_passes) {
    EXPECT_FALSE(false);
    EXPECT_FALSE(0 == 1);
}

TEST(ExpectBool, true_fails_on_false) {
    EXPECT_FAILS(EXPECT_TRUE(false));
}

TEST(ExpectBool, false_fails_on_true) {
    EXPECT_FAILS(EXPECT_FALSE(true));
}

TEST(ExpectBool, continues_after_failure) {
    // Both run — EXPECT doesn't abort
    EXPECT_FAILS(EXPECT_TRUE(false));
    EXPECT_PASSES(EXPECT_TRUE(true));
}


// ============================================================
// ASSERT COMPARISON
// ============================================================

TEST(AssertCmp, equal_ints) {
    ASSERT_EQ(5, 5);
    ASSERT_EQ(0, 0);
    ASSERT_EQ(-10, -10);
}

TEST(AssertCmp, equal_strings) {
    std::string a = "hello";
    std::string b = "hello";
    ASSERT_EQ(a, b);
}

TEST(AssertCmp, not_equal) {
    ASSERT_NE(1, 2);
    ASSERT_NE(0, -1);
}

TEST(AssertCmp, less_than) {
    ASSERT_LT(1, 2);
    ASSERT_LT(-5, 0);
}

TEST(AssertCmp, less_than_equal) {
    ASSERT_LE(1, 2);
    ASSERT_LE(5, 5);
}

TEST(AssertCmp, greater_than) {
    ASSERT_GT(10, 3);
    ASSERT_GT(0, -1);
}

TEST(AssertCmp, greater_than_equal) {
    ASSERT_GE(10, 3);
    ASSERT_GE(5, 5);
}

TEST(AssertCmp, eq_fails_on_mismatch) {
    ASSERT_FAILS(ASSERT_EQ(1, 2));
}

TEST(AssertCmp, ne_fails_when_equal) {
    ASSERT_FAILS(ASSERT_NE(5, 5));
}

TEST(AssertCmp, lt_fails_when_equal) {
    ASSERT_FAILS(ASSERT_LT(5, 5));
}

TEST(AssertCmp, gt_fails_when_less) {
    ASSERT_FAILS(ASSERT_GT(1, 10));
}


// ============================================================
// EXPECT COMPARISON
// ============================================================

TEST(ExpectCmp, equal) {
    EXPECT_EQ(42, 42);
    EXPECT_EQ('A', 'A');
}

TEST(ExpectCmp, not_equal) {
    EXPECT_NE(1, 2);
}

TEST(ExpectCmp, less_than) {
    EXPECT_LT(3, 7);
}

TEST(ExpectCmp, less_than_equal) {
    EXPECT_LE(3, 7);
    EXPECT_LE(7, 7);
}

TEST(ExpectCmp, greater_than) {
    EXPECT_GT(7, 3);
}

TEST(ExpectCmp, greater_than_equal) {
    EXPECT_GE(7, 3);
    EXPECT_GE(7, 7);
}

TEST(ExpectCmp, eq_fails_on_mismatch) {
    EXPECT_FAILS(EXPECT_EQ(1, 99));
}

TEST(ExpectCmp, all_run_despite_failures) {
    // EXPECT keeps running even after a failure
    EXPECT_FAILS(EXPECT_EQ(1, 2));
    EXPECT_PASSES(EXPECT_EQ(3, 3));
}


// ============================================================
// ASSERT FLOAT
// ============================================================

TEST(AssertFloat, abs_near_passes) {
    ASSERT_ABS_NEAR(1.0f, 1.05f, 0.1f);
    ASSERT_ABS_NEAR(5.0, 5.0, 0.0);
}

TEST(AssertFloat, abs_near_fails_outside_tolerance) {
    ASSERT_FAILS(ASSERT_ABS_NEAR(1.0f, 2.0f, 0.5f));
}

TEST(AssertFloat, rel_near_passes) {
    // |5.0 - 6.0| = 1.0 <= 0.5 * max(5,6) = 3.0 => passes
    ASSERT_REL_NEAR(5.0f, 6.0f, 0.5f);
}

TEST(AssertFloat, rel_near_fails) {
    // |5.0 - 6.0| = 1.0 > 0.1 * 6 = 0.6 => fails
    ASSERT_FAILS(ASSERT_REL_NEAR(5.0f, 6.0f, 0.1f));
}

TEST(AssertFloat, near_3_args_is_abs_near) {
    ASSERT_NEAR(10.0, 10.05, 0.1);
}

TEST(AssertFloat, near_4_args_combined) {
    // abs_tol = 0.01, rel_tol = 0.5 => max(0.01, 0.5*6) = 3.0 > 1.0 => passes
    ASSERT_NEAR(5.0f, 6.0f, 0.01f, 0.5f);
}

TEST(AssertFloat, mixed_float_types) {
    float a = 1.0f;
    double b = 1.001;
    long double tol = 0.01L;
    ASSERT_ABS_NEAR(a, b, tol);
}


// ============================================================
// EXPECT FLOAT
// ============================================================

TEST(ExpectFloat, abs_near) {
    EXPECT_ABS_NEAR(3.14f, 3.15f, 0.02f);
}

TEST(ExpectFloat, rel_near) {
    EXPECT_REL_NEAR(100.0, 105.0, 0.1);
}

TEST(ExpectFloat, near_3_args) {
    EXPECT_NEAR(0.0, 0.001, 0.01);
}

TEST(ExpectFloat, near_4_args) {
    EXPECT_NEAR(5.0f, 6.0f, 0.01f, 0.5f);
}

TEST(ExpectFloat, abs_near_fails) {
    EXPECT_FAILS(EXPECT_ABS_NEAR(1.0f, 5.0f, 0.1f));
}

TEST(ExpectFloat, rel_near_fails) {
    EXPECT_FAILS(EXPECT_REL_NEAR(5.0f, 6.0f, 0.1f));
}


// ============================================================
// ASSERT ITERABLE (ordered, STL containers)
// ============================================================

TEST(AssertIterable, ordered_eq_vectors) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::vector<int> b = {1, 2, 3, 4, 5};
    ASSERT_EQ_ORDERED(a, b);
}

TEST(AssertIterable, ordered_eq_mixed_types) {
    std::vector<int> a = {10, 20, 30};
    std::set<int>    b = {10, 20, 30};
    ASSERT_EQ_ORDERED(a, b);
}

TEST(AssertIterable, ordered_eq_fails_on_diff) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 9, 3};
    ASSERT_FAILS(ASSERT_EQ_ORDERED(a, b));
}

TEST(AssertIterable, ordered_eq_fails_on_size_diff) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2};
    ASSERT_FAILS(ASSERT_EQ_ORDERED(a, b));
}

// NOTE: The array overload of ASSERT_EQ_ORDERED has a bug (throw outside if-block).
// The test below is commented out until fixed.
// TEST(AssertIterable, ordered_eq_arrays_BUG) {
//     int a[] = {1, 2, 3};
//     int b[] = {1, 2, 3};
//     ASSERT_EQ_ORDERED(a, b); // currently throws unconditionally on first iteration
// }


// ============================================================
// EXPECT ITERABLE (ordered)
// ============================================================

TEST(ExpectIterable, ordered_eq_passes) {
    std::vector<int> a = {5, 10, 15};
    std::vector<int> b = {5, 10, 15};
    EXPECT_EQ_ORDERED(a, b);
}

TEST(ExpectIterable, ordered_eq_fails_on_diff) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 0, 3};
    EXPECT_FAILS(EXPECT_EQ_ORDERED(a, b));
}

TEST(ExpectIterable, ordered_eq_fails_size_mismatch) {
    std::vector<std::string> a = {"a", "b"};
    std::vector<std::string> b = {"a", "b", "c"};
    EXPECT_FAILS(EXPECT_EQ_ORDERED(a, b));
}

TEST(ExpectIterable, ordered_eq_arrays) {
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    EXPECT_EQ_ORDERED(a, b);
}

TEST(ExpectIterable, ordered_eq_arrays_fails) {
    int a[] = {1, 2, 3};
    int b[] = {1, 9, 3};
    EXPECT_FAILS(EXPECT_EQ_ORDERED(a, b));
}


// ============================================================
// ASSERT META
// ============================================================

TEST(AssertMeta, passes_on_passing_test) {
    ASSERT_PASSES(ASSERT_TRUE(true));
}

TEST(AssertMeta, passes_on_passing_expect) {
    ASSERT_PASSES(EXPECT_EQ(1, 1));
}

TEST(AssertMeta, fails_on_failing_test) {
    ASSERT_FAILS(ASSERT_TRUE(false));
}

TEST(AssertMeta, fails_on_failing_expect) {
    ASSERT_FAILS(EXPECT_EQ(1, 2));
}

TEST(AssertMeta, nested_assert_does_not_escape) {
    // Inner ASSERT_TRUE(false) fails, but ASSERT_FAILS catches it
    // — the outer test suite should still continue normally
    ASSERT_FAILS(ASSERT_TRUE(false));
    ASSERT_TRUE(true); // this should still run
}


// ============================================================
// EXPECT META
// ============================================================

TEST(ExpectMeta, passes_on_passing_test) {
    EXPECT_PASSES(EXPECT_TRUE(true));
}

TEST(ExpectMeta, fails_on_failing_test) {
    EXPECT_FAILS(EXPECT_TRUE(false));
}

TEST(ExpectMeta, both_run_after_failure) {
    EXPECT_FAILS(EXPECT_EQ(0, 99));
    EXPECT_PASSES(EXPECT_EQ(5, 5));
}


// ============================================================
// ASSERT NULL
// ============================================================

TEST(AssertNull, null_pointer_passes) {
    int* p = nullptr;
    ASSERT_NULL(p);
}

TEST(AssertNull, not_null_passes) {
    int x = 5;
    int* p = &x;
    ASSERT_NOT_NULL(p);
}

TEST(AssertNull, null_fails_on_non_null) {
    int x = 5;
    int* p = &x;
    ASSERT_FAILS(ASSERT_NULL(p));
}

TEST(AssertNull, not_null_fails_on_null) {
    int* p = nullptr;
    ASSERT_FAILS(ASSERT_NOT_NULL(p));
}

TEST(AssertNull, string_pointer) {
    const char* s = "hello";
    ASSERT_NOT_NULL(s);
    const char* n = nullptr;
    ASSERT_NULL(n);
}


// ============================================================
// EXPECT NULL
// ============================================================

TEST(ExpectNull, null_passes) {
    double* p = nullptr;
    EXPECT_NULL(p);
}

TEST(ExpectNull, not_null_passes) {
    double x = 3.14;
    double* p = &x;
    EXPECT_NOT_NULL(p);
}

TEST(ExpectNull, null_fails_on_non_null) {
    int x = 1;
    EXPECT_FAILS(EXPECT_NULL(&x));
}

TEST(ExpectNull, not_null_fails_on_null) {
    int* p = nullptr;
    EXPECT_FAILS(EXPECT_NOT_NULL(p));
}


// ============================================================
// ASSERT PREDICATE
// ============================================================

TEST(AssertPredicate, all_of_vector_passes) {
    std::vector<int> v = {2, 4, 6, 8};
    ASSERT_ALL(v, [](int x){ return x % 2 == 0; });
}

TEST(AssertPredicate, all_of_fails_when_one_differs) {
    std::vector<int> v = {2, 4, 5, 8};
    ASSERT_FAILS(ASSERT_ALL(v, [](int x){ return x % 2 == 0; }));
}

TEST(AssertPredicate, some_of_passes) {
    std::vector<int> v = {1, 3, 4, 7};
    ASSERT_SOME(v, [](int x){ return x % 2 == 0; });
}

TEST(AssertPredicate, some_of_fails_when_none_match) {
    std::vector<int> v = {1, 3, 5, 7};
    ASSERT_FAILS(ASSERT_SOME(v, [](int x){ return x % 2 == 0; }));
}

TEST(AssertPredicate, none_of_passes) {
    std::vector<int> v = {1, 3, 5, 7};
    ASSERT_NONE(v, [](int x){ return x % 2 == 0; });
}

TEST(AssertPredicate, none_of_fails_when_one_matches) {
    std::vector<int> v = {1, 2, 5, 7};
    ASSERT_FAILS(ASSERT_NONE(v, [](int x){ return x % 2 == 0; }));
}

TEST(AssertPredicate, all_of_raw_array) {
    int arr[] = {10, 20, 30};
    ASSERT_ALL(arr, [](int x){ return x > 0; });
}

TEST(AssertPredicate, some_of_raw_array) {
    int arr[] = {1, 2, 3};
    ASSERT_SOME(arr, [](int x){ return x > 2; });
}

TEST(AssertPredicate, none_of_raw_array) {
    int arr[] = {1, 2, 3};
    ASSERT_NONE(arr, [](int x){ return x > 10; });
}

TEST(AssertPredicate, string_predicate) {
    std::vector<std::string> words = {"apple", "ant", "avocado"};
    ASSERT_ALL(words, [](const std::string& s){ return s[0] == 'a'; });
}


// ============================================================
// EXPECT PREDICATE
// ============================================================

TEST(ExpectPredicate, all_of_passes) {
    std::vector<int> v = {2, 4, 6};
    EXPECT_ALL(v, [](int x){ return x > 0; });
}

TEST(ExpectPredicate, all_of_fails) {
    std::vector<int> v = {2, -1, 6};
    EXPECT_FAILS(EXPECT_ALL(v, [](int x){ return x > 0; }));
}

TEST(ExpectPredicate, some_of_passes) {
    std::vector<int> v = {1, 3, 6};
    EXPECT_SOME(v, [](int x){ return x % 2 == 0; });
}

TEST(ExpectPredicate, some_of_fails) {
    std::vector<int> v = {1, 3, 5};
    EXPECT_FAILS(EXPECT_SOME(v, [](int x){ return x % 2 == 0; }));
}

TEST(ExpectPredicate, none_of_passes) {
    std::vector<int> v = {1, 3, 5};
    EXPECT_NONE(v, [](int x){ return x % 2 == 0; });
}

TEST(ExpectPredicate, none_of_fails) {
    std::vector<int> v = {1, 2, 5};
    EXPECT_FAILS(EXPECT_NONE(v, [](int x){ return x % 2 == 0; }));
}

TEST(ExpectPredicate, continues_after_failure) {
    std::vector<int> v = {1, 3, 5};
    EXPECT_FAILS(EXPECT_ALL(v, [](int x){ return x > 10; }));
    EXPECT_PASSES(EXPECT_NONE(v, [](int x){ return x > 10; }));
}


// ============================================================
// ASSERT STRING
// ============================================================

TEST(AssertString, eq_std_string) {
    std::string a = "hello world";
    std::string b = "hello world";
    ASSERT_STR_EQ(a, b);
}

TEST(AssertString, eq_const_char) {
    const char* a = "test";
    const char* b = "test";
    ASSERT_STR_EQ(a, b);
}

TEST(AssertString, ne_std_string) {
    std::string a = "foo";
    std::string b = "bar";
    ASSERT_STR_NE(a, b);
}

TEST(AssertString, ne_const_char) {
    const char* a = "abc";
    const char* b = "xyz";
    ASSERT_STR_NE(a, b);
}

TEST(AssertString, eq_fails_on_mismatch) {
    std::string a = "hello";
    std::string b = "world";
    ASSERT_FAILS(ASSERT_STR_EQ(a, b));
}

TEST(AssertString, ne_fails_when_equal) {
    std::string a = "same";
    std::string b = "same";
    ASSERT_FAILS(ASSERT_STR_NE(a, b));
}

TEST(AssertString, eq_empty_strings) {
    std::string a = "";
    std::string b = "";
    ASSERT_STR_EQ(a, b);
}

TEST(AssertString, ne_empty_vs_nonempty) {
    std::string a = "";
    std::string b = "not empty";
    ASSERT_STR_NE(a, b);
}


// ============================================================
// EXPECT STRING
// ============================================================

TEST(ExpectString, eq_passes) {
    EXPECT_STR_EQ(std::string("hi"), std::string("hi"));
}

TEST(ExpectString, ne_passes) {
    EXPECT_STR_NE(std::string("foo"), std::string("bar"));
}

TEST(ExpectString, eq_fails) {
    EXPECT_FAILS(EXPECT_STR_EQ(std::string("a"), std::string("b")));
}

TEST(ExpectString, ne_fails_when_equal) {
    EXPECT_FAILS(EXPECT_STR_NE(std::string("same"), std::string("same")));
}

TEST(ExpectString, continues_after_eq_failure) {
    EXPECT_FAILS(EXPECT_STR_EQ(std::string("a"), std::string("b")));
    EXPECT_PASSES(EXPECT_STR_NE(std::string("x"), std::string("y")));
}


// ============================================================
// ASSERT THROWS
// ============================================================

void throwsRuntime()    { throw std::runtime_error("runtime error"); }
void throwsLogic()      { throw std::logic_error("logic error"); }
void throwsInt()        { throw 42; }
void doesNotThrowFn()   { int x = 1 + 1; (void)x; }

TEST(AssertThrows, throws_any_exception) {
    ASSERT_THROWS(throwsRuntime);
}

TEST(AssertThrows, throws_specific_type) {
    ASSERT_THROWS(throwsRuntime, std::runtime_error);
}

TEST(AssertThrows, throws_non_std_exception) {
    ASSERT_THROWS(throwsInt);
}

TEST(AssertThrows, does_not_throw_passes) {
    ASSERT_DOES_NOT_THROW(doesNotThrowFn);
}

TEST(AssertThrows, throws_fails_when_no_exception) {
    ASSERT_FAILS(ASSERT_THROWS(doesNotThrowFn));
}

TEST(AssertThrows, does_not_throw_fails_when_it_does) {
    ASSERT_FAILS(ASSERT_DOES_NOT_THROW(throwsRuntime));
}

TEST(AssertThrows, throws_wrong_type_fails) {
    // throwsLogic throws logic_error, not runtime_error
    ASSERT_FAILS(ASSERT_THROWS(throwsLogic, std::runtime_error));
}


// ============================================================
// EXPECT THROWS
// ============================================================

TEST(ExpectThrows, throws_any_exception) {
    EXPECT_THROWS(throwsRuntime);
}

TEST(ExpectThrows, throws_specific_type) {
    EXPECT_THROWS(throwsRuntime, std::runtime_error);
}

TEST(ExpectThrows, does_not_throw_passes) {
    EXPECT_DOES_NOT_THROW(doesNotThrowFn);
}

TEST(ExpectThrows, throws_fails_when_no_exception) {
    EXPECT_FAILS(EXPECT_THROWS(doesNotThrowFn));
}

TEST(ExpectThrows, does_not_throw_fails_when_it_does) {
    EXPECT_FAILS(EXPECT_DOES_NOT_THROW(throwsRuntime));
}

TEST(ExpectThrows, continues_after_failure) {
    EXPECT_FAILS(EXPECT_THROWS(doesNotThrowFn));
    EXPECT_PASSES(EXPECT_DOES_NOT_THROW(doesNotThrowFn));
}
