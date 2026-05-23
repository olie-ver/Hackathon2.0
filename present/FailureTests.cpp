#include "../include/Runner.hpp"
#include "../include/Tests.hpp"
#include <vector>
#include <set>
#include <string>
#include <stdexcept>
 
// ============================================================
//  FailureTests.cpp
//
//  Every test in this file is INTENTIONALLY written to fail.
//  The purpose is to demonstrate what failure output looks
//  like for each Assert and Expect assertion category.
//
//  Assert tests: stop at the first failing assertion in the
//                test function, then move on to the next test.
//
//  Expect tests: collect every failure in the test function
//                before moving on to the next test.
// ============================================================
 
 
// ============================================================
// ASSERT BOOL FAILURES
// ============================================================
 
TEST(AssertBool_Fail, true_given_false) {
    ASSERT_TRUE(false);                 // fails — stops here
    ASSERT_TRUE(true);                  // never reached
}
 
TEST(AssertBool_Fail, false_given_true) {
    ASSERT_FALSE(true);                 // fails — stops here
    ASSERT_FALSE(false);                // never reached
}
 
TEST(AssertBool_Fail, true_expression_fails) {
    int x = 5;
    ASSERT_TRUE(x > 10);               // fails: "5 > 10 to be true"
}
 
 
// ============================================================
// EXPECT BOOL FAILURES
// ============================================================
 
TEST(ExpectBool_Fail, collects_all_failures) {
    EXPECT_TRUE(false);                 // fails — continues
    EXPECT_FALSE(true);                 // also fails — both recorded
    EXPECT_TRUE(1 == 2);               // fails — all three recorded
}
 
 
// ============================================================
// ASSERT COMPARISON FAILURES
// ============================================================
 
TEST(AssertCmp_Fail, eq_mismatch) {
    ASSERT_EQ(1, 99);                   // fails: a=1, b=99
}
 
TEST(AssertCmp_Fail, ne_when_equal) {
    ASSERT_NE(42, 42);                  // fails: a=42, b=42
}
 
TEST(AssertCmp_Fail, lt_when_greater) {
    ASSERT_LT(10, 3);                   // fails: a=10, b=3
}
 
TEST(AssertCmp_Fail, le_when_greater) {
    ASSERT_LE(10, 3);                   // fails: a=10, b=3
}
 
TEST(AssertCmp_Fail, gt_when_less) {
    ASSERT_GT(1, 100);                  // fails: a=1, b=100
}
 
TEST(AssertCmp_Fail, ge_when_less) {
    ASSERT_GE(1, 100);                  // fails: a=1, b=100
}
 
 
// ============================================================
// EXPECT COMPARISON FAILURES
// ============================================================
 
TEST(ExpectCmp_Fail, all_comparisons_fail) {
    EXPECT_EQ(1, 2);                    // fails — continues
    EXPECT_NE(5, 5);                    // fails — continues
    EXPECT_LT(10, 1);                   // fails — continues
    EXPECT_LE(10, 1);                   // fails — continues
    EXPECT_GT(1, 10);                   // fails — continues
    EXPECT_GE(1, 10);                   // fails — all six recorded
}
 
 
// ============================================================
// ASSERT FLOAT FAILURES
// ============================================================
 
TEST(AssertFloat_Fail, abs_near_outside_tolerance) {
    ASSERT_ABS_NEAR(1.0f, 5.0f, 0.1f); // fails: |1-5|=4 > 0.1
}
 
TEST(AssertFloat_Fail, rel_near_outside_tolerance) {
    // |5 - 6| = 1.0 > 0.1 * max(5,6) = 0.6 => fails
    ASSERT_REL_NEAR(5.0, 6.0, 0.1);
}
 
TEST(AssertFloat_Fail, near_3_args_fails) {
    ASSERT_NEAR(1.0, 2.0, 0.5);        // fails: |1-2|=1 > 0.5
}
 
TEST(AssertFloat_Fail, near_4_args_fails) {
    // abs_tol=0.01, rel_tol=0.05 => max(0.01, 0.05*6)=0.3 < |5-6|=1 => fails
    ASSERT_NEAR(5.0f, 6.0f, 0.01f, 0.05f);
}
 
 
// ============================================================
// EXPECT FLOAT FAILURES
// ============================================================
 
TEST(ExpectFloat_Fail, collects_all_float_failures) {
    EXPECT_ABS_NEAR(1.0f, 9.0f, 0.1f); // fails — continues
    EXPECT_REL_NEAR(5.0, 6.0, 0.01);   // fails — continues
    EXPECT_NEAR(0.0, 1.0, 0.001);      // fails — all three recorded
}
 
 
// ============================================================
// ASSERT ITERABLE FAILURES
// ============================================================
 
TEST(AssertIterable_Fail, ordered_eq_value_mismatch) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 9, 3};    // index 1 differs
    ASSERT_ORDERED_EQ(a, b);           // fails at index 1
}
 
TEST(AssertIterable_Fail, ordered_eq_size_mismatch) {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2};
    ASSERT_ORDERED_EQ(a, b);           // fails: different sizes
}
 
TEST(AssertIterable_Fail, ordered_eq_string_vectors) {
    std::vector<std::string> a = {"foo", "bar"};
    std::vector<std::string> b = {"foo", "baz"};
    ASSERT_ORDERED_EQ(a, b);           // fails at index 1
}
 
 
// ============================================================
// EXPECT ITERABLE FAILURES
// ============================================================
 
TEST(ExpectIterable_Fail, ordered_eq_reports_all_mismatches) {
    std::vector<int> a = {1, 2, 3, 4};
    std::vector<int> b = {1, 9, 3, 8}; // index 1 and 3 differ
    EXPECT_ORDERED_EQ(a, b);           // both mismatches recorded
}
 
TEST(ExpectIterable_Fail, ordered_eq_array_mismatch) {
    int a[] = {10, 20, 30};
    int b[] = {10, 99, 30};
    EXPECT_ORDERED_EQ(a, b);           // fails at index 1
}
 
 
// ============================================================
// ASSERT META FAILURES
// ============================================================
 
TEST(AssertMeta_Fail, passes_but_test_fails) {
    // ASSERT_PASSES fails because the inner test fails
    ASSERT_PASSES(ASSERT_TRUE(false));
}
 
TEST(AssertMeta_Fail, fails_but_test_passes) {
    // ASSERT_FAILS fails because the inner test passes
    ASSERT_FAILS(ASSERT_TRUE(true));
}
 
 
// ============================================================
// EXPECT META FAILURES
// ============================================================
 
TEST(ExpectMeta_Fail, passes_but_test_fails) {
    EXPECT_PASSES(EXPECT_TRUE(false));  // fails — inner test failed
}
 
TEST(ExpectMeta_Fail, fails_but_test_passes) {
    EXPECT_FAILS(EXPECT_TRUE(true));    // fails — inner test passed
}
 
 
// ============================================================
// ASSERT NULL FAILURES
// ============================================================
 
TEST(AssertNull_Fail, null_on_non_null) {
    int x = 5;
    ASSERT_NULL(&x);                    // fails: &x is not nullptr
}
 
TEST(AssertNull_Fail, not_null_on_null) {
    int* p = nullptr;
    ASSERT_NOT_NULL(p);                 // fails: p is nullptr
}
 
 
// ============================================================
// EXPECT NULL FAILURES
// ============================================================
 
TEST(ExpectNull_Fail, both_null_failures) {
    int x = 1;
    int* p = nullptr;
    EXPECT_NULL(&x);                    // fails — continues
    EXPECT_NOT_NULL(p);                 // fails — both recorded
}
 
 
// ============================================================
// ASSERT PREDICATE FAILURES
// ============================================================
 
TEST(AssertPredicate_Fail, all_fails_with_odd_element) {
    std::vector<int> v = {2, 4, 5, 8};
    ASSERT_ALL(v, [](int x){ return x % 2 == 0; }); // fails: 5 is odd
}
 
TEST(AssertPredicate_Fail, some_fails_with_no_match) {
    std::vector<int> v = {1, 3, 5, 7};
    ASSERT_SOME(v, [](int x){ return x % 2 == 0; }); // fails: no evens
}
 
TEST(AssertPredicate_Fail, none_fails_with_a_match) {
    std::vector<int> v = {1, 2, 5, 7};
    ASSERT_NONE(v, [](int x){ return x % 2 == 0; }); // fails: 2 matches
}
 
TEST(AssertPredicate_Fail, all_fails_raw_array) {
    int arr[] = {2, 4, 7, 8};
    ASSERT_ALL(arr, [](int x){ return x % 2 == 0; }); // fails: 7 is odd
}
 
 
// ============================================================
// EXPECT PREDICATE FAILURES
// ============================================================
 
TEST(ExpectPredicate_Fail, all_three_predicates_fail) {
    std::vector<int> v = {1, 2, 3};
    EXPECT_ALL(v, [](int x){ return x > 10; });  // fails — continues
    EXPECT_SOME(v, [](int x){ return x > 10; }); // fails — continues
    // EXPECT_NONE: v has no elements > 10, so it would pass — skip
    EXPECT_ALL(v, [](int x){ return x < 0; });   // fails — all three recorded
}
 
 
// ============================================================
// ASSERT STRING FAILURES
// ============================================================
 
TEST(AssertString_Fail, eq_std_string_mismatch) {
    std::string a = "hello";
    std::string b = "world";
    ASSERT_STR_EQ(a, b);               // fails
}
 
TEST(AssertString_Fail, ne_equal_strings) {
    std::string a = "same";
    std::string b = "same";
    ASSERT_STR_NE(a, b);               // fails: a == b
}
 
TEST(AssertString_Fail, eq_const_char_mismatch) {
    const char* a = "abc";
    const char* b = "xyz";
    ASSERT_STR_EQ(a, b);               // fails
}
 
 
// ============================================================
// EXPECT STRING FAILURES
// ============================================================
 
TEST(ExpectString_Fail, both_string_failures) {
    EXPECT_STR_EQ(std::string("foo"), std::string("bar")); // fails — continues
    EXPECT_STR_NE(std::string("eq"), std::string("eq"));   // fails — both recorded
}
 
 
// ============================================================
// ASSERT THROWS FAILURES
// ============================================================
 
void noThrow()    { int x = 1 + 1; (void)x; }
void throwsRuntime2() { throw std::runtime_error("oops"); }
void throwsLogic2()   { throw std::logic_error("logic"); }
 
TEST(AssertThrows_Fail, throws_but_nothing_thrown) {
    ASSERT_THROWS(noThrow);            // fails: noThrow() doesn't throw
}
 
TEST(AssertThrows_Fail, does_not_throw_but_it_does) {
    ASSERT_DOES_NOT_THROW(throwsRuntime2); // fails: throwsRuntime() throws
}
 
TEST(AssertThrows_Fail, throws_wrong_exception_type) {
    // throwsLogic throws logic_error, not runtime_error
    ASSERT_THROWS(throwsLogic2, std::runtime_error); // fails: wrong type
}
 
 
// ============================================================
// EXPECT THROWS FAILURES
// ============================================================
 
TEST(ExpectThrows_Fail, all_throw_failures) {
    EXPECT_THROWS(noThrow);               // fails — continues
    EXPECT_DOES_NOT_THROW(throwsRuntime2); // fails — continues
    EXPECT_THROWS(throwsLogic2, std::runtime_error); // fails — all three recorded
}