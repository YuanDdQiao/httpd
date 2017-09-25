/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * httpdunit.h: a collection of test helper macros designed to reduce test
 * boilerplate and help the build system autogenerate test case definitions.
 *
 * Here's how the magic works:
 *
 * Every test case under test/unit declares test functions using Check's
 * START_TEST or the HTTPD_START_LOOP_TEST macro. The build system searches each
 * source file for those macros and generates an appropriate stub, called
 * test/unit/<filename>.tests, that will add each test function to the test
 * case. This stub is then pulled in by three lines of boilerplate at the end of
 * every test case's source file (see the HTTPD_BEGIN_TEST_CASE documentation,
 * below).
 *
 * The build system uses that same three-line boilerplate to determine the names
 * of all the test cases, and adds them automatically to the main test suite
 * using a similar generate-stub-and-include process.
 */

#include "apr.h"   /* for pid_t on Windows, needed by Check */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wstrict-prototypes"

#include "check.h"

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

/*
 * Boilerplate Macros
 */

/**
 * Declares a test case. The build system uses this macro to generate a stub,
 * which will automatically pull the new test case into the main suite.
 *
 * After beginning a test case, you must #include the autogenerated testcase
 * stub ("test/unit/<filename>.tests") and then end the test case with
 * HTTPD_END_TEST_CASE. For example, for a test case called foobar.c:
 *
 *    HTTPD_BEGIN_TEST_CASE(foobar)
 *    #include "test/unit/foobar.tests"
 *    HTTPD_END_TEST_CASE
 *
 * The NAME identifying the test case must be globally unique within the test
 * suite; if in doubt, just use the filename (minus extension).
 */
#define HTTPD_BEGIN_TEST_CASE(NAME) \
TCase * NAME##_test_case(void); \
TCase * NAME##_test_case(void) \
{ \
    TCase *testcase = tcase_create(#NAME);

/**
 * Like HTTPD_BEGIN_TEST_CASE, but additionally declares a pair of setup and
 * teardown functions for the test case. The setup function is run once before
 * every test function, and the teardown function is run directly after. Keep in
 * mind that expensive fixtures will heavily impact test runtime.
 *
 * Both setup and teardown take no parameters and return void.
 *
 * These functions are passed to Check's tcase_add_checked_fixture(). See the
 * Check documentation for details; the gist is that checked fixures run after
 * Check forks the test process, and therefore they cannot influence each other
 * even if something smashes the heap.
 */
#define HTTPD_BEGIN_TEST_CASE_WITH_FIXTURE(NAME, SETUP, TEARDOWN) \
HTTPD_BEGIN_TEST_CASE(NAME) \
    tcase_add_checked_fixture(testcase, (SETUP), (TEARDOWN));

/**
 * Bookend for HTTPD_BEGIN_TEST_CASE[_WITH_FIXTURE], which finishes the test
 * case function.
 */
#define HTTPD_END_TEST_CASE \
    return testcase; \
}

/*
 * Test Declaration Macros
 */

/**
 * Use this macro instead of Check's START_TEST when you want to run a test
 * function multiple times. The build system will add the test function to the
 * test case using Check's tcase_add_loop_test().
 *
 * The loop index is available to the test function as the special variable _i.
 * _i will start at 0 and increment to a maximum of (NUM_ITERATIONS - 1).
 */
#define HTTPD_START_LOOP_TEST(NAME, NUM_ITERATIONS) START_TEST(NAME)
