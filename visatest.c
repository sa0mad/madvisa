// Standard C includes
#include <stdio.h>
#include <stdlib.h>

// Project includes
#include "tb.h"

// Module include
#define MADVISA
#include "visa.h"
#include "visa_internals.h"

TB_VAR(visa);

static void test_print_retval(ViObject vi, ViStatus retval, ViStatus expected_retval)
{
	char * vstr;

	if (retval != expected_retval)
	{
		vstr = (char *)malloc(sizeof(char)*256);
		if (vstr == NULL)
			return;
		retval = viStatusDesc(vi, retval, vstr);
		if (retval != VI_SUCCESS)
		{
			free(vstr);
			return;
		}
		printf("%s\n", vstr);
		free(vstr);
	}
	return;
}

int main()
{
	ViStatus	retval;
	ViAttrState	attrValue;
	ViObject	vi;
	ViVersion	version = 0;
	ViUInt16	vuint16;
	ViString	vstr;
	ViRsrc		vrsrc;
	ViAccessMode	accessmode = 0;
	long		valid;
	ViSession	vsession;
	ViUInt32	vuint32;
	ViUInt32	vuint64;
	ViKeyId		requested_key = NULL;
	ViKeyId		requested_key2 = NULL;
	ViKeyId		requested_key3 = NULL;
	ViKeyId		access_key = NULL;
	ViKeyId		access_key2 = NULL;
	int		i;
	ViUInt16	excl_count0, excl_count1;
	ViUInt16	shrd_count0, shrd_count1;
	ViAccessMode	access_mode;

	TB_VAR_RESET(visa);

	// viClose tests
	// Rule 3.3.2
	retval = viClose(VI_NULL);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_WARN_NULL_OBJECT, "VISA RULE 3.3.2");
	TB_TEST_EXPECT_M_LUINT(visa, vi_next_(), 1UL, "Initial vi value");
	retval = viGetFakeSession(&vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "viGetFakeSession");
	TB_TEST_EXPECT_M_LUINT(visa, vi_next_(), 2UL, "Next vi value");
	TB_TEST_EXPECT_M_LUINT(visa, vi, 1UL, "Assigned vi value");
	retval = viClose(vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.3.1.1");	

	// viGetAttribute tests
	retval = viGetAttribute(0,0,&attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_SESSION, "VISA 3.4.1.1 VI_ERROR_INV_SESSION");
	retval = viGetFakeSession(&vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "viGetFakeSession");
	retval = viGetAttribute(vi,0,&attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_NSUP_ATTR, "VISA 3.4.1.1 VI_ERROR_NSUP_ATTR");
	retval = viGetAttribute(vi,VI_ATTR_TCPIP_SERVER_CERT,&attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_SIZE, "VISA 3.4.1.1 VI_ERROR_INV_SIZE 1");
	retval = viGetAttribute(vi,VI_ATTR_TCPIP_SERVER_CERT_SIZE,&attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.4.1.1 VI_ERROR_INV_SIZE 2");
	retval = viGetAttribute(vi,VI_ATTR_TCPIP_SERVER_CERT,&attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.4.1.1 VI_ERROR_INV_SIZE 3");

	// viSetAttribute tests
	// Rule 3.4.2
	retval = viSetAttribute(0,0,attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_SESSION, "VISA 3.4.1.1 VI_ERROR_INV_SESSION");
	retval = viSetAttribute(vi,0,attrValue);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_NSUP_ATTR, "VISA 3.4.1.1 VI_ERROR_NSUP_ATTR");
	retval = viSetAttribute(vi,VI_ATTR_MAX_QUEUE_LENGTH,1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.4.2");
	retval = viSetAttribute(vi,VI_ATTR_MAX_QUEUE_LENGTH,0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_NSUP_ATTR_STATE, "VISA Rule 3.4.2");
	retval = viSetAttribute(vi,VI_ATTR_RSRC_IMPL_VERSION,0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_ATTR_READONLY, "VISA 3.4.1.2");

	// Template Required Attributes
	// Rule 3.2.1
	// Rule 3.2.2
	// Rule 3.2.3
	// Rule 3.2.7
	// Rule 3.2.8
	// Rule 3.2.9
	// Rule 3.2.10
	retval = viGetAttribute(vi,VI_ATTR_RSRC_IMPL_VERSION,&version);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_IMPL_VERSION");
	// Rule 3.6.2
	retval = viGetAttribute(vi,VI_ATTR_RSRC_LOCK_STATE,&accessmode);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_LOCK_STATE");
	if ((accessmode == VI_NO_LOCK) || (accessmode == VI_EXCLUSIVE_LOCK) || (accessmode == VI_SHARED_LOCK))
		valid = 1;
	else
	{
		printf("Invalid access mode %08lxñ", accessmode);
		valid = 0;
	}
	TB_TEST_EXPECT_M_LINT(visa, valid, 1L, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_LOCK_STATE");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_MANF_ID,&vuint16);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_MANF_ID");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_MANF_NAME,&vstr);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_MANF_NAME");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_NAME,&vrsrc);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_NAME");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SPEC_VERSION,&version);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_SPEC_VERSION");
	TB_TEST_EXPECT_M_LINT(visa, (long int)version, 0x00700200L, "VISA Rule 3.2.3 VI_ATTR_RSRC_SPEC_VERSION");
	retval = viGetAttribute(vi,VI_ATTR_RM_SESSION,&vsession);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RM_SESSION");
	//TB_TEST_EXPECT_M_LINT(visa, (long int)vsession, vi, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RM_SESSION");
	retval = viGetAttribute(vi,VI_ATTR_MAX_QUEUE_LENGTH,&vuint32);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_MAX_QUEUE_LENGTH");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_CLASS,&vstr);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_RSRC_CLASS");
	if (_VISA_ENV_IS_32_BIT == 1)
	{
		TB_TEST_EXPECT_M_LINT(visa, (long int)VI_ATTR_USER_DATA, (long int)VI_ATTR_USER_DATA_32, "VISA RULE 3.2.7");
	}
	if (_VISA_ENV_IS_64_BIT == 1)
	{
		TB_TEST_EXPECT_M_LINT(visa, (long int)VI_ATTR_USER_DATA, (long int)VI_ATTR_USER_DATA_64, "VISA RULE 3.2.8");
	}
	if (_VISA_ENV_IS_32_BIT == 1)
	{
		#ifdef VI_ATTR_USER_DATA_64
		valid = 0;
		#else
		valid = 1;
		#endif
		TB_TEST_EXPECT_M_LINT(visa, valid, 1L, "VISA RULE 3.2.9");
	}
	if (_VISA_ENV_IS_32_BIT == 1)
	{
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA,&vuint32);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_USER_DATA");
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA_32,&vuint32);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_USER_DATA_32");
	}
	if (_VISA_ENV_IS_64_BIT == 1)
	{
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA,&vuint64);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_USER_DATA");
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA_32,&vuint32);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_USER_DATA_32");
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA_64,&vuint64);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.1 and 3.2.2 VI_ATTR_USER_DATA_64");
		retval = viSetAttribute(vi,VI_ATTR_USER_DATA_32,0xDEADBABE);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.10");
		retval = viGetAttribute(vi,VI_ATTR_USER_DATA_64,&vuint64);
		TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.2.10");
		TB_TEST_EXPECT_M_LUINT(visa, vuint64, 0xDEADBABEUL, "VISA Rule 3.2.10");
	}

	// 3.4.1.3 viStatusDesc()
	// Rule 3.4.3
	// Rule 3.4.4
	vstr = (char *)malloc(sizeof(char)*256);
	if (vstr == NULL)
		return 0;
	*vstr = (char)1;
	retval = viStatusDesc(vi, 1, vstr);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_WARN_UNKNOWN_STATUS, "VISA Rule 3.4.3");
	TB_TEST_EXPECT_M_LINT(visa, (long)*vstr, 0L, "VISA Rule 3.4.3");
	retval = viStatusDesc(vi, VI_SUCCESS, vstr);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.4.3");
	free(vstr);

	// 3.5.1
	retval = viTerminate(vi, VI_NULL, VI_NULL);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.5.1");

	// Rule 3.6.3
	retval = viLock(0,0,0,VI_NULL,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_SESSION, "VISA 3.6.3 VI_ERROR_INV_SESSION");
	retval = viLock(vi,5,0,VI_NULL,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_LOCK_TYPE, "VISA 3.6.3 VI_ERROR_INV_SESSION");
	// Rule 3.6.9
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	// Rule 3.6.13
	// Rule 3.6.27
	retval = viLock(vi,VI_EXCLUSIVE_LOCK,0,VI_NULL,VI_NULL);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.6.13 VI_SUCCESS");
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.6.27 VI_SUCCESS");
	// Rule 3.6.10
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, excl_count1, excl_count0+1, "VISA 3.6.10");
	// Rule 3.6.24
	retval = viGetAttribute(vi,VI_ATTR_RSRC_LOCK_STATE,&access_mode);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_LUINT(visa, access_mode, (long unsigned int)VI_EXCLUSIVE_LOCK, "VISA 3.6.24");
	// Rule 3.6.14
	// Rule 3.6.28
	requested_key = (char *)malloc(sizeof(char)*512);
	if (requested_key == NULL)
	{
		return -1;
	}
	requested_key[0] = 'f';
	requested_key[1] = 'o';
	requested_key[2] = 'o';
	requested_key[3] = (ViChar)0;
	requested_key2 = (char *)malloc(sizeof(char)*512);
	if (requested_key2 == NULL)
	{
		free(requested_key);
		return -1;
	}
        requested_key2[0] = 'b';
	requested_key2[1] = 'a';
	requested_key2[2] = 'r';
	requested_key2[3] = (ViChar)0;
	for (i=4; i < 511; i++)
	{
		requested_key[i] = (ViChar)0;
		requested_key2[i] = (ViChar)0;
	}
	access_key = (char *)malloc(sizeof(char)*512);
	if (access_key == NULL)
	{
		free(requested_key2);
		free(requested_key);
		return -1;
	}
	access_key2 = (char *)malloc(sizeof(char)*512);
	if (access_key2 == NULL)
	{
		free(access_key);
		free(requested_key2);
		free(requested_key);
		return -1;
	}
	for (i=0; i < 511; i++)
		access_key[i] = (ViChar)0;
	retval = viLock(vi,VI_EXCLUSIVE_LOCK,0,VI_NULL,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_EXCLUSIVE, "VISA 3.6.14 VI_SUCCESS_NEXTED_EXCLUSIVE");
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_EXCLUSIVE, "VISA 3.6.28 VI_SUCCESS_NESTED_EXCLUSIVE");
	TB_TEST_EXPECT_M_INT(visa, (int)*access_key, (int)0, "VISA 3.6.14 VI_SUCCESS");
	// Clean-up
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	// Rule 3.6.32
	// Rule 3.6.34
	retval = viUnlock(vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_EXCLUSIVE, "VISA 3.6.32");
	// Rule 3.6.10
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, excl_count1, excl_count0-1, "VISA 3.6.34");
	// Rule 3.6.32
	// Rule 3.6.35
	// Rule 3.6.36
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	retval = viUnlock(vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_EXCL_LOCK_COUNT,&excl_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, excl_count1, excl_count0-1, "VISA 3.6.35");
	TB_TEST_EXPECT_M_UINT(visa, excl_count1, 0, "VISA 3.6.36");
	
	// Rule 3.6.25
	retval = viGetAttribute(vi,VI_ATTR_RSRC_LOCK_STATE,&access_mode);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_LUINT(visa, access_mode, (long unsigned int)VI_NO_LOCK, "VISA 3.6.25");
	// Rule 3.6.17
	requested_key3 = (char *)malloc(sizeof(char)*512);
	if (requested_key3 == NULL)
	{
		free(access_key2);
		free(access_key);
		free(requested_key2);
		free(requested_key);
		return -1;
	}
	for (i = 0; i < 256; i++)
		requested_key3[i] = 'f';
	requested_key3[256] = (ViChar)0;
	retval = viLock(vi,VI_SHARED_LOCK,0,requested_key3,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_ACCESS_KEY, "VISA 3.6.17 VI_ERROR_INV_ACCESS_KEY");
	requested_key3[255] = (ViChar)0;
	// Rule 3.6.9
	// Rule 3.6.29
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	retval = viLock(vi,VI_SHARED_LOCK,0,requested_key3,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.6.17 VI_SUCCESS");
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.6.29 VI_SUCCESS");
	// Rule 3.6.25
	retval = viGetAttribute(vi,VI_ATTR_RSRC_LOCK_STATE,&access_mode);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_LUINT(visa, access_mode, (long unsigned int)VI_SHARED_LOCK, "VISA 3.6.25");
	// Rule 3.6.11
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, shrd_count1, shrd_count0+1, "VISA 3.6.10");
	// Rule 3.6.18
	for (i = 0; i < 256; i++)
		if (access_key[i] != requested_key3[i])
			break;
	TB_TEST_EXPECT_M_INT(visa, i, 256, "VISA 3.6.18");
	// Rule 3.6.30
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	retval = viLock(vi,VI_SHARED_LOCK,0,requested_key3,access_key2);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_SHARED, "VISA 3.6.30 VI_SUCCESS_NESTED_SHARED");
	// Rule 3.6.19
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_SHARED, "VISA 3.6.19 VI_SUCCESS_NESTED_SHARED");
	// Rule 3.6.20
	TB_TEST_EXPECT_M_STR(visa, access_key, access_key2, "VISA 3.6.20");
	// Rule 3.6.11
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, shrd_count1, shrd_count0+1, "VISA 3.6.10");
	// Rule 3.6.31
	retval = viLock(vi,VI_SHARED_LOCK,0,requested_key2,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_ACCESS_KEY, "VISA 3.6.31 VI_ERROR_INV_ACCESS_KEY");
	
	test_print_retval(vi, retval, VI_ERROR_INV_ACCESS_KEY);
	
	// Rule 3.6.33
	// Rule 3.6.35
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	retval = viUnlock(vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS_NESTED_SHARED, "VISA 3.6.33 VI_SUCCESS_NESTED_SHARED");
	retval = viGetAttribute(vi,VI_ATTR_RSRC_SHRD_LOCK_COUNT,&shrd_count1);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA Rule 3.6.9");
	TB_TEST_EXPECT_M_UINT(visa, shrd_count1, shrd_count0-1, "VISA 3.6.35");
	
	retval = viUnlock(0);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_INV_SESSION, "VISA 3.6.3 VI_ERROR_INV_SESSION");

	// Rule 3.6.12
	retval = viLock(vi,VI_EXCLUSIVE_LOCK,0,VI_NULL,VI_NULL);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.6.13 VI_SUCCESS");
	access_key[0] = (ViChar)0;
	retval = viLock(vi,VI_SHARED_LOCK,0,VI_NULL,access_key);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_ERROR_RSRC_LOCKED, "VISA 3.6.12 VI_ERROR_RSRC_LOCKED");
	// Rule 3.6.15
	TB_TEST_EXPECT_N_UCHAR(visa, access_key[0], (ViChar)0, "VISA 3.6.15");

	free(requested_key);
	free(requested_key2);
	free(requested_key3);
	free(access_key);
	free(access_key2);
	
	// Rules not testable
	// Rule 3.2.4
	// Rule 3.3.1
	// Rule 3.5.1
	// Rule 3.6.1
	// Rule 3.6.16

	// Indirectly testable
	// Rule 3.2.11

	// Remaining rules to implement
	// Rule 3.6.8
	// Rule 3.6.21
	// Rule 3.6.22
	// Rule 3.6.23
	// Rule 3.6.26

	// Rule 3.6.36
	// Rule 3.6.37
	// Rule 3.6.38
	// Rule 3.6.39

	// Awaits events
	// Rule 3.2.5
	// Rule 3.2.6
	
	// Awaits HiSLIP
	// Rule 3.4.1
	// Rule 3.4.1-a
	// Rule 3.4.1-b
	// Rule 3.6.4
	// Rule 3.6.5
	// Rule 3.6.6
	// Rule 3.6.7
	
	// Clean up
	retval = viClose(vi);
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.4.1.1 VI_ERROR_INV_SIZE 3");	

	//vi_flush_pool();
	retval = viCleanup();
	TB_TEST_EXPECT_M_LINT(visa, retval, VI_SUCCESS, "VISA 3.4.1.1 VI_ERROR_INV_SIZE 3");	
	
	TB_RESULT(visa);
	return 0;
}
