#ifndef __VISATYPE_H
#define __VISATYPE_H

// Standard C includes
#include <stdint.h>

// Definitions based on [1]
// [1] https://www.ivifoundation.org/downloads/Architecture%20Specifications/vpp432_2022-05-19.pdf

// VISA environment is not 32 bit, but 64 bit - currently assumed but added to support 32 bit backport
#define _VISA_ENV_64BIT
#ifdef _VISA_ENV_32BIT
#define _VISA_ENV_IS_32_BIT	(1)
#define _VISA_ENV_IS_64_BIT	(0)
#endif
#ifdef _VISA_ENV_64BIT
#define _VISA_ENV_IS_32_BIT	(0)
#define _VISA_ENV_IS_64_BIT	(1)
#endif

// Defines assume Linux environment
#define _VI_FAR
#define _VI_FUNC
#define _VI_FUNCC
#define _VI_FUNCH
#define _VI_SIGNED	signed
#define _VI_PTR		*

// 3.1 Type assignements
// Table 3.1.1

typedef unsigned long	ViUInt32;
typedef ViUInt32 *	ViPUInt32;
typedef ViUInt32	ViAUInt32[];

typedef signed long	ViInt32;
typedef ViInt32 *	ViPInt32;
typedef ViInt32		ViAInt32[];

typedef uint64_t	ViUInt64;
typedef ViUInt64 *	ViPUInt64;
typedef ViUInt64	ViAUInt64[];

typedef int64_t		ViInt64;
typedef ViInt64 *	ViPInt64;
typedef ViInt64		ViAInt64[];

typedef unsigned short	ViUInt16;
typedef ViUInt16 *	ViPUInt16;
typedef ViUInt16	ViAUInt16[];

typedef signed short	ViInt16;
typedef ViInt16 *	ViPInt16;
typedef ViInt16		ViAInt16[];

typedef unsigned char	ViUInt8;
typedef ViUInt8 *	ViPUInt8;
typedef ViUInt8		ViAUInt8[];

typedef signed char	ViInt8;
typedef ViInt8 *	ViPInt8;
typedef ViInt8		ViAInt8[];

typedef void *		ViAddr;
typedef ViAddr *	ViPAddr;
typedef ViAddr		ViAAddr[];

typedef char		ViChar;
typedef ViChar *	ViPChar;
typedef ViChar		ViAChar[];

typedef unsigned char	ViByte;
typedef ViByte *	ViPByte;
typedef ViByte		ViAByte[];

typedef ViUInt16	ViBoolean;
typedef ViBoolean *	ViPBoolean;
typedef ViBoolean	ViABoolean[];

typedef float		ViReal32;
typedef ViReal32 *	ViPReal32;
typedef ViReal32	ViAReal32[];

typedef double		ViReal64;
typedef ViReal64 *	ViPReal64;
typedef ViReal64	ViAReal64[];

typedef ViPByte		ViBuf;
typedef const ViByte *	ViConstBuf;
typedef ViPByte		ViPBuf;
typedef ViBuf		ViABuf[];

typedef ViPChar		ViString;
typedef const ViChar *	ViConstString;
typedef ViPChar		ViPString;
typedef ViString	ViAString[];

typedef	ViString	ViRsrc;
typedef ViConstString	ViConstRsrc;
typedef	ViString	ViPRsrc;
typedef ViRsrc		ViARsrc[];

typedef ViInt32		ViStatus;
typedef ViStatus *	ViPStatus;
typedef ViStatus	ViAStatus[];

typedef	ViUInt32	ViVersion;
typedef ViVersion *	ViPVersion;
typedef ViVersion	ViAVersion[];

typedef ViUInt32	ViObject;
typedef ViObject *	ViPObject;
typedef ViObject	ViAObject[];

typedef ViObject	ViSession;
typedef ViSession *	ViPSession;
typedef ViSession	ViASession[];

typedef ViUInt32	ViAttr;

#define VI_TRUE		((ViBoolean)1)
#define VI_FALSE	((ViBoolean)0)

#endif // __VISATYPE_H
