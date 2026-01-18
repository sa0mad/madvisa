#ifndef __TB_H
#define __TB_H
#include <math.h>
#include <string.h>

#define TB_VAR(suite) static int suite##_tb_tests, suite##_tb_fails

#define TB_VAR_RESET(suite) suite##_tb_tests = 0; suite##_tb_fails = 0

#define TB_RESULT(suite) 	printf("%s TB RESULT: ", #suite); \
	if (suite##_tb_fails == 0) \
		printf("SUCCESS"); \
	else \
		printf("FAILURE"); \
	printf(" %i/%i\n", suite##_tb_fails, suite##_tb_tests)

#define TB_HEADER(str) \
	printf("%s:%u (%s) %s test-bench\n", __FILE__, __LINE__, __func__, str)

#define	TB_TEST_SILENT(suite) suite##_tb_tests++

#define TB_TEST(suite, test, str) \
	if (test) \
	{ \
		printf("%s:%u (%s) ERROR %s\n", __FILE__, __LINE__, __func__, str); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_CHAR(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %i (expected %i)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_CHAR(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %i (expected other than %i)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_UCHAR(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %u (expected %u)\n", __FILE__, __LINE__, __func__, str, (unsigned int)val, (unsigned int)expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_UCHAR(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %u (expected other than %u)\n", __FILE__, __LINE__, __func__, str, (unsigned int)val, (unsigned int)expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_INT(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %i (expected %i)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_INT(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %i (expected other than %i)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_LINT(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %li (expected %li)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_LINT(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %li (expected other than %li)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_UINT(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %u (expected %u)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_UINT(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %u (expected other than %u)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_LUINT(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %lu (expected %lu)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_LUINT(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %lu (expected other than %lu)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_DBL(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected %f)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_DBL(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected other than %f)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_DBL_RE(suite, val, expect, err, str)	\
	if (fabs(val - expect) > (val*err))			\
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected %f)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_DBL_RE(suite, val, expect, str)	\
	if (fabs(val - expect) <= (val*err))			\
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected other than %f)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_DBLNAN(suite, val, str)	\
	if (isnan(val) != 1)				\
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected %f)\n", __FILE__, __LINE__, __func__, str, val, NAN); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_DBLNAN(suite, val, str)	\
	if (isnan(val) == 1)			\
	{ \
		printf("%s:%u (%s) ERROR %s %f (expected other than %f)\n", __FILE__, __LINE__, __func__, str, val, NAN); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_PTR(suite, val, expect, str)	\
	if (val != expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %p (expected %p)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_PTR(suite, val, expect, str)	\
	if (val == expect) \
	{ \
		printf("%s:%u (%s) ERROR %s %p (expected other than %p)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_M_STR(suite, val, expect, str)	\
	if (strcmp(val, expect) != 0)			\
	{ \
		printf("%s:%u (%s) ERROR %s %p (expected %p)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#define TB_TEST_EXPECT_N_STR(suite, val, expect, str)	\
	if (strcmp(val, expect) == 0)			\
	{ \
		printf("%s:%u (%s) ERROR %s %p (expected other than %p)\n", __FILE__, __LINE__, __func__, str, val, expect); \
		suite##_tb_fails++; \
	} \
	suite##_tb_tests++

#endif // __TB_H
