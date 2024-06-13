// Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Module includes
#define MADVISA
#include "visa.h"
#include "visa_internals.h"

#define VI_RSRC_IMPL_VERSION	((ViVersion)0x00100000L)

typedef struct vi_rsrc_s vi_rsrc;

struct vi_rsrc_s
{
	// Resource Lock state
	ViUInt16	excl_lock_count;
	ViUInt16	shared_lock_count;
	ViKeyId		lock_key;
	
	// Resource Attributes
	ViString	rsrc_name;
	ViAccessMode	access_mode;
	ViString	rsrc_class;

	// Linked neighbor resources
	/*@null@*/ vi_rsrc *	rsrc_head;
	/*@null@*/ vi_rsrc *	rsrc_tail;
};

static ViStatus vi_rsrc_free(vi_rsrc * rsrc);

typedef struct vi_rm_s vi_rm;

struct vi_rm_s
{
	// List of resources
	/*@null@*/ vi_rsrc *	rsrc_head;
	/*@null@*/ vi_rsrc *	rsrc_tail;
};

// Default Resource Manager
/*@null@*/ static vi_rm *	vi_rm_default = NULL;

typedef ViInt16		ViObjType;

#define _VI_OBJTYPE_SESSION	((ViObjType)1)
#define _VI_OBJTYPE_FINDLIST	((ViObjType)2)
#define _VI_OBJTYPE_EVENT	((ViObjType)3)

typedef struct vi_attr_s vi_attr;

struct vi_attr_s
{
	// Link
	/*@null@*/ vi_attr * 	next;

	// Attribute and state
	ViAttr		attr;
	ViAttrState	state;
};

typedef struct vi_s vi_t;

struct vi_s
{
	// Link
	vi_t * next;

	// Type
	ViObjType	objtype;
	
	// Id
	ViSession vi;

	// Resource connected to session
	vi_rsrc *	rsrc;
	
	// VI_ATTR_TCPIP_SERVER_CERT_SIZE get status
	int	vi_attr_tcpip_server_cert_size_get_status;

	// VISA Template Required Attributes
	ViUInt32	max_queue_length;
	ViUInt64	user_data;

	/*@null@*/ vi_attr * attr_head;
	/*@null@*/ vi_attr * attr_tail;
};

// The head and tail of vi session list
/*@null@*/ static vi_t * vi_head = NULL;
/*@null@*/ static vi_t * vi_tail = NULL;

// Session pool of objects
/*@null@*/ static vi_t * vi_pool = NULL;
static ViSession vi_next = 1;

ViSession vi_next_()
{
	return vi_next;
}

// Attribute pool
/*@null@*/ static vi_attr * vi_attr_pool = NULL;

/* Resource Manager */

static ViStatus vi_rm_alloc()
{
	if (vi_rm_default != NULL)
		return VI_SUCCESS;
	vi_rm_default = (vi_rm *)malloc(sizeof(vi_rm));
	if (vi_rm_default == NULL)
		return VI_ERROR_ALLOC;
	vi_rm_default->rsrc_head = NULL;
	vi_rm_default->rsrc_tail = NULL;
	return VI_SUCCESS;
}

static ViStatus vi_rm_free()
{
	vi_rm *		rm = vi_rm_default;
	/*@null@*/ vi_rsrc *	rsrc;
	/*@null@*/ vi_rsrc *	next;
	ViStatus	retval;
	
	if (rm == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc = rm->rsrc_head;
	while (rsrc != NULL)
	{
		next = rsrc->rsrc_tail;
		retval = vi_rsrc_free(rsrc);
		if (retval != VI_SUCCESS)
			return retval;
		rsrc = next;
	}
	free(rm);
	vi_rm_default = NULL;
	return VI_SUCCESS;
}

static ViStatus vi_rm_add(vi_rsrc * rsrc)
{
	ViStatus retval;
	vi_rsrc *	prev;
	retval = vi_rm_alloc();
	if (retval != VI_SUCCESS)
		return retval;
	if (vi_rm_default == NULL)
		return VI_ERROR_INV_OBJECT;
	// Link resources
	prev = vi_rm_default->rsrc_tail;
	if (prev != NULL)
		prev ->rsrc_tail = rsrc;
	rsrc->rsrc_head = vi_rm_default->rsrc_tail;
	rsrc->rsrc_tail = NULL;
	// Link to resource manager
	if (vi_rm_default->rsrc_head == NULL)
		vi_rm_default->rsrc_head = rsrc;
	vi_rm_default->rsrc_tail = rsrc;
	return VI_SUCCESS;
}

/* Resource management */

static ViStatus vi_rsrc_alloc(vi_rsrc **rsrc_p)
{
	vi_rsrc *	rsrc;
	rsrc = (vi_rsrc *)malloc(sizeof(vi_rsrc));
	if (rsrc == NULL)
		return VI_ERROR_ALLOC;
	rsrc->rsrc_head = NULL;
	rsrc->rsrc_tail = NULL;
	*rsrc_p = rsrc;
	return vi_rm_add(rsrc);
}

static ViStatus vi_rsrc_create(ViString rsrc_name)
{
	vi_rsrc *	rsrc = NULL;
	ViStatus	retval;

	retval = vi_rsrc_alloc(&rsrc);
	if (retval < 0)
		return retval;
	if (rsrc == NULL)
		return VI_ERROR_ALLOC;
	rsrc->excl_lock_count = 0;
	rsrc->shared_lock_count = 0;
	rsrc->lock_key = NULL;
	rsrc->rsrc_name = rsrc_name;
	rsrc->access_mode = VI_NO_LOCK;
	rsrc->rsrc_class = "INSTR";
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_free(vi_rsrc * rsrc)
{
	//free(rsrc->access_key);
	free(rsrc);
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_impl_version(vi_rsrc * rsrc, ViVersion * version)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*version = VI_RSRC_IMPL_VERSION;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_lock_state(vi_rsrc * rsrc, ViAccessMode * lock_state)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*lock_state = rsrc->access_mode;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_manf_id(vi_rsrc * rsrc, ViUInt16 * manf_id)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*manf_id = 0;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_manf_name(vi_rsrc * rsrc, ViString * manf_name)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*manf_name = "NA";
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_spec_version(vi_rsrc * rsrc, ViVersion * version)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*version = VI_SPEC_VERSION;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_class(vi_rsrc * rsrc, ViString * class)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*class = rsrc->rsrc_class;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_name(vi_rsrc * rsrc, ViString * rsrc_name)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	if (rsrc_name == NULL)
		return VI_ERROR_INV_OBJECT;
	*rsrc_name = rsrc->rsrc_name;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_excl_lock_count(vi_rsrc * rsrc, ViUInt16 * rsrc_lock_count)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	if (rsrc_lock_count == NULL)
		return VI_ERROR_INV_OBJECT;
	*rsrc_lock_count = rsrc->excl_lock_count;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_inc_excl_lock_count(vi_rsrc * rsrc)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc->excl_lock_count++;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_dec_excl_lock_count(vi_rsrc * rsrc)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc->excl_lock_count--;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_shared_lock_count(vi_rsrc * rsrc, ViUInt16 * rsrc_lock_count)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	if (rsrc_lock_count == NULL)
		return VI_ERROR_INV_OBJECT;
	*rsrc_lock_count = rsrc->shared_lock_count;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_inc_shared_lock_count(vi_rsrc * rsrc)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc->shared_lock_count++;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_dec_shared_lock_count(vi_rsrc * rsrc)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc->shared_lock_count--;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_set_lock_key(vi_rsrc * rsrc, ViKeyId requested_key)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc->lock_key = requested_key;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_get_lock_key(vi_rsrc * rsrc, ViKeyId * lock_key)
{
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	*lock_key = rsrc->lock_key;
	return VI_SUCCESS;
}

static ViStatus vi_rsrc_is_locked(vi_rsrc * rsrc, ViUInt16 * rsrc_is_locked)
{
	ViUInt16 is_locked = 0;
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	if (rsrc_is_locked == NULL)
		return VI_ERROR_INV_OBJECT;
	if (rsrc->excl_lock_count != 0)
		is_locked = 1;
	if (rsrc->shared_lock_count != 0)
		is_locked = 1;
	*rsrc_is_locked = is_locked;
	return VI_SUCCESS;
}

/* Session/Event/FindList support */

// Only use vi_locate_vi(), vi_alloc(), vi_free() and vi_flush() outside of
// this section!

static ViStatus vi_locate_vi_inner(ViSession vi, /*@null@*/ vi_t ** vipp, /*@null@*/ vi_t ** vipp_prev)
{
	/*@null@*/ vi_t * vip_prev = NULL;
	vi_t * vip = vi_head;
	if (vipp != NULL)
	{
		*vipp = NULL;
	}
	if (vipp_prev != NULL)
	{
		*vipp_prev = NULL;
	}
	if (vip == NULL)
		return VI_ERROR_INV_SESSION;
	while (vip->vi != vi)
	{
		vip_prev = vip;
		vip = vip->next;
		if (vip == NULL)
			return VI_ERROR_INV_SESSION;
	}
	if (vipp != NULL)
		*vipp = vip;
	if (vipp_prev != NULL)
		*vipp_prev = vip_prev;
	return VI_SUCCESS;
}

static ViStatus vi_locate_vi(ViSession vi, vi_t ** vipp)
{
	return vi_locate_vi_inner(vi, vipp, NULL);
}

static ViStatus vi_link_vi(vi_t * vip)
{
	if (vi_head == NULL)
	{
		vi_head = vip;
		vi_tail = vip;
		vip->next = NULL;
	}
	else
	{
		if (vi_tail != NULL)
			vi_tail->next = vip;
		vi_tail = vip;
		vip->next = NULL;
	}
	return VI_SUCCESS;
}

static ViStatus vi_unlink_vi(ViSession vi, vi_t ** vipp)
{
	vi_t * prev = NULL;
	vi_t * cur  = vi_head;
	ViStatus retval;

	retval = vi_locate_vi_inner(vi, &cur, &prev);
	if (retval != VI_SUCCESS)
		return retval;
	if ((prev != NULL) && (cur != NULL))
		prev->next = cur->next;
	if (cur != NULL)
	cur->next = NULL;
	if (vipp != NULL)
		*vipp = cur;
	return VI_SUCCESS;
}

//ViStatus vi_alloc(ViSession * vi, vi_t **vipp)
static ViStatus vi_alloc(ViSession * vi, vi_rsrc * rsrc)
{
	vi_t * 		cur = NULL;
	ViStatus	retval;

	// If possible, allocate a free from pool
	if (vi_pool != NULL)
	{
		cur = vi_pool;
		vi_pool = cur->next;
		cur->next = NULL;
	}
	// else, create a new
	else
	{
		cur = (vi_t *)malloc(sizeof(vi_t));
		if (cur == NULL)
			return VI_ERROR_ALLOC;
		cur->next = NULL;
		// Assign next free vi number
		cur->vi = vi_next++;
	}

	// Initiate rest
	cur->attr_head = NULL;
	cur->attr_tail = NULL;

	// Resource
	cur->rsrc = rsrc;

	// Initiate special status
	cur->vi_attr_tcpip_server_cert_size_get_status = 0;

	// Initiate VISA Template Reqúired Attributes
	cur->max_queue_length = 256;
	cur->user_data = 0;
	
	// Link it into know vi sessions
	retval = vi_link_vi(cur);
	if (retval != VI_SUCCESS)
		return retval;

	// Assign allocated vi
	*vi = cur->vi;
	
	return VI_SUCCESS;
}

static ViStatus vi_free_inner(vi_t * vip)
{
	free(vip);
	return VI_SUCCESS;
}

static ViStatus vi_free(ViSession vi)
{
	vi_t * cur = NULL;
	ViStatus retval;

	retval = vi_unlink_vi(vi, &cur);
	if (retval != VI_SUCCESS)
		return retval;
	// Put structure in pool
	if (cur != NULL)
		cur->next = vi_pool;
	vi_pool = cur;
	return VI_SUCCESS;
}

ViStatus vi_flush_pool()
{
	ViStatus retval;
	vi_t * cur;
	while (vi_pool != NULL)
	{
		cur = vi_pool;
		vi_pool = cur->next;
		retval = vi_free_inner(cur);
		if (retval != VI_SUCCESS)
			return retval;
	}
	return VI_SUCCESS;
}

/* Attribute support */

static ViStatus vi_attr_locate(vi_t * vip, ViAttr attribute, void _VI_PTR attrState)
{
	vi_attr *	cur;
	cur = vip->attr_head;
	while (cur != NULL)
	{
		if (cur->attr == attribute)
		{
			*(ViAttrState *)attrState = cur->state;
			return VI_SUCCESS;
		}
		cur = cur->next;
	}
	return VI_ERROR_NSUP_ATTR;
}

static ViStatus vi_attr_set(vi_t * vip, ViAttr attribute, ViAttrState attrState)
{
	vi_attr *	cur;
	vi_attr *	prev = NULL;
	cur = vip->attr_head;
	while (cur != NULL)
	{
		if (cur->attr == attribute)
			break;
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL)
	{
		cur = (vi_attr *)malloc(sizeof(vi_attr));
		if (cur == NULL)
			return VI_ERROR_ALLOC;
		cur->next = NULL;
	}
	if (prev == NULL)
		vip->attr_head = cur;
	else
		prev->next = cur;
	if (cur->next == NULL)
		vip->attr_tail = cur;
	cur->attr = attribute;
	cur->state = attrState;
	return VI_SUCCESS;
}

static ViStatus vi_attr_free_inner(vi_attr * cur)
{
	free(cur);
	return VI_SUCCESS;
}

static ViStatus vi_attr_free(vi_attr * cur)
{
	cur->next = vi_attr_pool;
	vi_attr_pool = cur;
	return VI_SUCCESS;
}

static ViStatus vi_attr_flush_pool()
{
	ViStatus retval;
	vi_attr * cur;
	while (vi_attr_pool != NULL)
	{
		cur = vi_attr_pool;
		vi_attr_pool = cur->next;
		retval = vi_attr_free_inner(cur);
		if (retval != VI_SUCCESS)
			return retval;
	}
	return VI_SUCCESS;
}

static ViStatus vi_attr_valid(/*@unused@*/ ViSession vi, ViAttr attrName)
{
	if (((attrName >> 16) & 0x3FFF) == 0x3FFF)
		return VI_SUCCESS;
	return VI_ERROR_NSUP_ATTR;
}

static ViStatus vi_attr_state_valid(/*@unused@*/ ViSession vi, ViAttr attrName, ViAttrState attrState)
{
	if ((attrName == VI_ATTR_MAX_QUEUE_LENGTH) && (attrState != 0))
		return VI_SUCCESS;
	if ((attrName == VI_ATTR_USER_DATA) || (attrName == VI_ATTR_USER_DATA_32)
	    || (attrName == VI_ATTR_USER_DATA_64))
		return VI_SUCCESS;
	if (attrName == VI_ATTR_RSRC_IMPL_VERSION)
		return VI_ERROR_ATTR_READONLY;
	return VI_ERROR_NSUP_ATTR_STATE;
}

/* Public functions */

/* vpp43 3.3.1.1 viClose(vi) */

ViStatus viClose(ViSession vi)
{
	ViStatus retval;
	vi_t * vip;

	if (vi == VI_NULL)
		return VI_WARN_NULL_OBJECT;

	// Locate vi
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;
	// Clear content
	while (vip->attr_head != NULL)
	{
		vi_attr * attr;
		attr = vip->attr_head;
		vip->attr_head = attr->next;
		retval = vi_attr_free(attr);
		if (retval != VI_SUCCESS)
			return retval;
	}
	// Finally remove vi session
	retval = vi_free(vi);
	if (retval != VI_SUCCESS)
		return retval;
	return VI_SUCCESS;
}

/* vpp43 3.4.1.1 viGetAttribute(vi) */

ViStatus viGetAttribute(ViObject vi, ViAttr attrName, void _VI_PTR attrValue)
{
	ViStatus retval;
	vi_t * vip;

	// Locate vi
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;

	// VISA Template Required Attributes
	if (attrName == VI_ATTR_RM_SESSION)
	{
		*(ViSession *)attrValue = vip->vi;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_RSRC_IMPL_VERSION)
	{
		retval = vi_rsrc_get_impl_version(vip->rsrc, (ViVersion *)attrValue);
		return retval;   ;
	}
	if (attrName == VI_ATTR_RSRC_LOCK_STATE)
	{
		retval = vi_rsrc_get_lock_state(vip->rsrc, (ViAccessMode *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_MANF_ID)
	{
		retval = vi_rsrc_get_manf_id(vip->rsrc, (ViUInt16 *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_MANF_NAME)
	{
		retval = vi_rsrc_get_manf_name(vip->rsrc, (ViString *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_NAME)
	{
		retval = vi_rsrc_get_name(vip->rsrc, (ViString *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_SPEC_VERSION)
	{
		retval = vi_rsrc_get_spec_version(vip->rsrc, (ViVersion *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_CLASS)
	{
		retval = vi_rsrc_get_class(vip->rsrc, (ViString *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_MAX_QUEUE_LENGTH)
	{
		*(ViUInt32 *)attrValue = vip->max_queue_length;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA)
	{
		*(ViUInt64 *)attrValue = vip->user_data;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA_32)
	{
		*(ViUInt32 *)attrValue = (ViUInt32)vip->user_data;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA_64)
	{
		*(ViUInt64 *)attrValue = vip->user_data;
		return VI_SUCCESS;
	}

	// MADVISA internal
	if (attrName == VI_ATTR_RSRC_SHRD_LOCK_COUNT)
	{
		retval = vi_rsrc_get_shared_lock_count(vip->rsrc, (ViUInt16 *)attrValue);
		return retval;
	}
	if (attrName == VI_ATTR_RSRC_EXCL_LOCK_COUNT)
	{
		retval = vi_rsrc_get_excl_lock_count(vip->rsrc, (ViUInt16 *)attrValue);
		return retval;
	}

	// VI_ATTR_TCPIP_SERVER_CERT_SIZE logic
	if (attrName == VI_ATTR_TCPIP_SERVER_CERT_SIZE)
		vip->vi_attr_tcpip_server_cert_size_get_status = 1;
	if ((attrName == VI_ATTR_TCPIP_SERVER_CERT) && (vip->vi_attr_tcpip_server_cert_size_get_status == 0))
		return VI_ERROR_INV_SIZE;

	// Locate attribute
	retval = vi_attr_locate(vip, attrName, attrValue);
	return retval;
}

/* vpp43 3.4.2.1 viSetAttribute(vi) */

ViStatus viSetAttribute(ViObject vi, ViAttr attrName, ViAttrState attrValue)
{
	ViStatus retval;
	vi_t * vip;

	// Locate vi
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;
	// Validate attribute
	retval = vi_attr_valid(vi, attrName);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_attr_state_valid(vi, attrName, attrValue);
	if (retval != VI_SUCCESS)
		return retval;
	// VISA Template Required Attributes
	if (attrName == VI_ATTR_MAX_QUEUE_LENGTH)
	{
		vip->max_queue_length = (ViUInt32)attrValue;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA)
	{
		vip->user_data = (ViUInt64)attrValue;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA_32)
	{
		vip->user_data = (ViUInt64)(ViUInt32)attrValue;
		return VI_SUCCESS;
	}
	if (attrName == VI_ATTR_USER_DATA_64)
	{
		vip->user_data = (ViUInt64)attrValue;
		return VI_SUCCESS;
	}
	// Locate attribute
	retval = vi_attr_set(vip, attrName, attrValue);
	return retval;
}

ViStatus viStatusDesc(/*@unused@*/ ViObject vi, ViStatus status, ViString desc)
{
	if (status == VI_SUCCESS)
	{
		strcpy(desc, "VI_SUCCESS");
		return VI_SUCCESS;
	}
	if (status == VI_SUCCESS_NESTED_EXCLUSIVE)
	{
		strcpy(desc, "VI_SUCCESS_NESTED_EXCLUSIVE");
		return VI_SUCCESS;
	}
	if (status == VI_SUCCESS_NESTED_SHARED)
	{
		strcpy(desc, "VI_SUCCESS_NESTED_SHARED");
		return VI_SUCCESS;
	}
	*desc = (ViChar)0;
	return VI_WARN_UNKNOWN_STATUS;
}

ViStatus viTerminate(/*@unused@*/ ViObject vi, /*@unused@*/ ViUInt16 degree, /*@unused@*/ ViJobId jobId)
{
	return VI_SUCCESS;
}

ViStatus viLock(ViObject vi, ViAccessMode lock_type, ViUInt32 timeout, ViConstKeyId requested_key_external, ViKeyId access_key)
{
	ViStatus retval;
	vi_t * vip;
	ViUInt16 excl_lock_count, shared_lock_count;
	ViKeyId requested_key = (ViKeyId)requested_key_external;
	ViKeyId lock_key;
	int	requested_key_length;
	int	is_locked = 0;

	// Locate vi
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;
	if ((lock_type == VI_NO_LOCK) || (lock_type == VI_EXCLUSIVE_LOCK)
	    || (lock_type == VI_SHARED_LOCK) || (lock_type == VI_LOAD_CONFIG))
	{
	}
	else
		return VI_ERROR_INV_LOCK_TYPE;

	retval = vi_rsrc_get_excl_lock_count(vip->rsrc, &excl_lock_count);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_rsrc_get_shared_lock_count(vip->rsrc, &shared_lock_count);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_rsrc_get_lock_key(vip->rsrc, &lock_key);
	if (retval != VI_SUCCESS)
		return retval;
	if ((lock_type == VI_EXCLUSIVE_LOCK) && (access_key != NULL))
		*access_key = (ViChar)0;
	if ((lock_type == VI_SHARED_LOCK) && (access_key == NULL))
		return VI_ERROR_ALLOC;
	if ((lock_type == VI_SHARED_LOCK) && (requested_key == NULL) && (access_key != NULL))
	{
		requested_key = access_key;
		retval = snprintf(requested_key, 256, "%p", (void *)vip);
		if (retval < 0)
		{
			return VI_ERROR_ALLOC;
		}
	}
	if ((lock_type == VI_SHARED_LOCK) && (requested_key != NULL))
		requested_key_length = (int)strlen(requested_key);
	else
		requested_key_length = 0;
	if ((lock_type == VI_SHARED_LOCK) && (requested_key != NULL) && (requested_key_length >= 256))
		return VI_ERROR_INV_ACCESS_KEY;
	if ((lock_type == VI_SHARED_LOCK) && (excl_lock_count > 0))
		return VI_ERROR_RSRC_LOCKED;
	if ((lock_type == VI_SHARED_LOCK) && (shared_lock_count > 0) && (strcmp(access_key,lock_key) != 0))
		return VI_ERROR_INV_ACCESS_KEY;
	// Lock possible, copy key
	if ((lock_type == VI_SHARED_LOCK) && (requested_key != NULL) && (is_locked == 0))
		strncpy(access_key,requested_key,255);
	// Lock possible, store key
	if (lock_type == VI_SHARED_LOCK)
	{
		retval = vi_rsrc_set_lock_key(vip->rsrc, requested_key);
		if (retval != VI_SUCCESS)
			return retval;
	}
	// Lock possible, increase lock count
	if (lock_type == VI_EXCLUSIVE_LOCK)
	{
		retval = vi_rsrc_inc_excl_lock_count(vip->rsrc);
		if (retval != VI_SUCCESS)
			return retval;
		if (excl_lock_count > 0)
			return VI_SUCCESS_NESTED_EXCLUSIVE;
	}
	if (lock_type == VI_SHARED_LOCK)
	{
		retval = vi_rsrc_inc_shared_lock_count(vip->rsrc);
		if (retval != VI_SUCCESS)
			return retval;
		if (shared_lock_count > 0)
			return VI_SUCCESS_NESTED_SHARED;
	}
	return VI_SUCCESS;
}

ViStatus viUnlock(ViObject vi)
{
	ViStatus retval;
	vi_t * vip;
	ViUInt16 excl_lock_count, shared_lock_count;

	// Locate vi
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_rsrc_get_excl_lock_count(vip->rsrc, &excl_lock_count);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_rsrc_get_shared_lock_count(vip->rsrc, &shared_lock_count);
	if (retval != VI_SUCCESS)
		return retval;
	// Lock possible, increase lock count
	if (excl_lock_count > 0)
	{
		retval = vi_rsrc_dec_excl_lock_count(vip->rsrc);
		if (retval != VI_SUCCESS)
			return retval;
		if (excl_lock_count > 1)
			return VI_SUCCESS_NESTED_EXCLUSIVE;
	}
	if (shared_lock_count > 0)
	{
		retval = vi_rsrc_dec_shared_lock_count(vip->rsrc);
		if (retval != VI_SUCCESS)
			return retval;
		if (shared_lock_count > 1)
			return VI_SUCCESS_NESTED_SHARED;
	}
	return VI_SUCCESS;
}

ViStatus viGetFakeSession(ViSession * vi_p)
{
	ViStatus retval;
	vi_rsrc *	rsrc;
	ViSession vi;
	vi_t * vip;
	char * fakecert = "fakecert";
	long fakecert_size = 9;
	if (vi_p == NULL)
		return VI_ERROR_INV_OBJECT;
	retval = vi_rsrc_create("INSTR");
	if (retval != VI_SUCCESS)
		return retval;
	if (vi_rm_default == NULL)
		return VI_ERROR_INV_OBJECT;
	rsrc = vi_rm_default->rsrc_head;
	if (rsrc == NULL)
		return VI_ERROR_INV_OBJECT;
	retval = vi_alloc(&vi, rsrc);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_locate_vi(vi, &vip);
	if (retval != VI_SUCCESS)
		return retval;
	// Attributes to support viGetAttribute testing
	retval = vi_attr_set(vip, VI_ATTR_TCPIP_SERVER_CERT, (ViAttrState)fakecert);
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_attr_set(vip, VI_ATTR_TCPIP_SERVER_CERT_SIZE, (ViAttrState)fakecert_size);
	if (retval != VI_SUCCESS)
		return retval;
	*vi_p = vi;
	return VI_SUCCESS;
}

ViStatus viCleanup()
{
	ViStatus retval;
	retval = vi_flush_pool();
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_rm_free();
	if (retval != VI_SUCCESS)
		return retval;
	retval = vi_attr_flush_pool();
	if (retval != VI_SUCCESS)
		return retval;
	return VI_SUCCESS;
}
