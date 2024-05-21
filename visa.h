#ifndef __VISA_H
#define __VISA_H

// Standard C includes
#include <stdarg.h>

// Module includes
#include "visatype.h"

//#if define(__cplusplus) || define(__cplusplus__)
//   extern "C" {
//#endid

// Definitions based on [1]
// [1] https://www.ivifoundation.org/downloads/Architecture%20Specifications/vpp432_2022-05-19.pdf


// 3.1 Type assignements
// Table 3.1.2
// VISA types

typedef ViUInt32		ViAccessMode;
typedef ViAccessMode *		ViPAccessMode;

typedef ViUInt64		ViBusAddress;
typedef ViBusAddress *		ViPBusAddress;

typedef ViUInt64		ViBusAddress64;
typedef ViBusAddress64 *	ViPBusAddress64;

typedef ViUInt64		ViBusSize;

typedef ViUInt64		ViAttrState;
typedef void *			ViPAddrState;

typedef va_list			ViVAList;

typedef ViUInt32		ViEventType;
typedef ViEventType *		ViPEventType;
typedef ViEventType		ViAEventType[];

typedef ViAttr *		ViPAttr;
typedef ViAttr			ViAAttr[];

typedef ViUInt32		ViEventFilter;

typedef ViObject		ViFindList;
typedef ViFindList *		ViPFindList;

typedef ViObject		ViEvent;
typedef ViEvent *		ViPEvent;

typedef ViString		ViKeyId;
typedef ViPString		ViPKeyId;
typedef ViConstString		ViConstKeyId;

typedef ViUInt32		ViJobId;
typedef ViJobId *		ViPJobId;

typedef ViStatus (*ViHndlr)(ViSession, ViEventType, ViEvent, ViAddr);

// 3.2 Operation Prototypes

ViStatus viGetDefaultRM		(ViPSession sesn);
ViStatus viOpenDefaultRM	(ViPSession sesn);
ViStatus viFindRsrc		(ViSession sesn, ViConstString expr,
				 ViPFindList findList, ViPUInt32 retCnt,
				 ViChar _VI_FAR desc[]);
ViStatus viFindNext		(ViFindList findList, ViChar _VI_FAR desc[]);
ViStatus viParseRsrc		(ViSession sesn, ViConstRsrc rsrcName,
				 ViPUInt16 intfType, ViPUInt16 intfNum);
ViStatus viParseRsrcEx		(ViSession Sesn, ViConstRsrc rsrcName,
				 ViPUInt16 intfType, ViPUInt16 intfNum,
				 ViChar _VI_FAR rsrcClass[],
				 ViChar _VI_FAR expandedUnaliasedName[],
				 ViChar _VI_FAR aliasIfExists[]);
ViStatus viOpen			(ViSession sesn, ViConstRsrc name,
				 ViAccessMode mode, ViUInt32 timeout,
				 ViPSession vi);
ViStatus viClose		(ViObject vi);
ViStatus viGetAttribute		(ViObject vi, ViAttr attrName,
				 void _VI_PTR attrValue);
ViStatus viSetAttribute		(ViObject vi, ViAttr attrName,
				 ViAttrState attrValue);
ViStatus viStatusDesc		(ViObject vi, ViStatus status,
				 ViChar _VI_FAR desc[]);
ViStatus viTerminate		(ViObject vi, ViUInt16 degree, ViJobId jobId);
ViStatus viLock			(ViSession vi, ViAccessMode lockType,
				 ViUInt32 timeout, ViConstKeyId requestedKey,
				 ViChar _VI_FAR accessKey[]);
ViStatus viUnlock		(ViSession vi);
ViStatus viEnableEvent		(ViSession vi, ViEventType eventType,
				 ViUInt16 mechanism, ViEventFilter context);
ViStatus viDisableEvent		(ViSession vi, ViEventType eventType,
				 ViUInt16 mechanism);
ViStatus viDiscardEvents	(ViSession vi, ViEventType eventType,
				 ViUInt16 mechanism);
ViStatus viWaitOnEvent		(ViSession vi, ViEventType inEventType,
				 ViUInt32 timeout, ViPEventType outEventType,
				 ViPEvent outContext);
ViStatus viInstallHandler	(ViSession vi, ViEventType eventType,
				 ViHndlr handler, ViAddr userHandle);
ViStatus viUninstallHandler	(ViSession vi, ViEventType eventType,
				 ViHndlr handler, ViAddr userHandle);
ViStatus viRead			(ViSession vi, ViPBuf, ViUInt32 cnt,
				 ViPUInt32 retCnt);
ViStatus viReadAsync		(ViSession vi, ViPBuf, ViUInt32 cnt,
				 ViPJobId jobId);
ViStatus viReadToFile		(ViSession vi, ViConstString filename,
				 ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus viWrite		(ViSession vi, ViConstBuf buf, ViUInt32 cnt,
				 ViPUInt32 retCnt);
ViStatus viWriteAsync		(ViSession vi, ViConstBuf buf, ViUInt32 cnt,
				 ViPJobId jobId);
ViStatus viWriteFromFile	(ViSession vi, ViConstString filename,
				 ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus viAssertTrigger	(ViSession vi, ViUInt16 protocol);
ViStatus viReadSTB		(ViSession vi, ViPUInt16 status);
ViStatus viClear		(ViSession vi);
ViStatus viSetBuf		(ViSession vi, ViUInt16 mask, ViUInt32 size);
ViStatus viFlush		(ViSession vi, ViUInt16 mask);
ViStatus viBufWrite		(ViSession vi, ViConstBuf buf, ViUInt32 cnt,
				 ViPUInt32 retCnt);
ViStatus viBufRead		(ViSession vi, ViPBuf buf, ViUInt32 cnt,
				 ViPUInt32 retCnt);
ViStatus viPrintf		(ViSession vi, ViConstString writeFmt, ...);
ViStatus viVPrintf		(ViSession vi, ViConstString writeFmt,
				 ViVAList params);
ViStatus viSPrintf		(ViSession vi, ViPBuf buf,
				 ViConstString writeFmt, ...);
ViStatus viVSPrintf		(ViSession vi, ViPBuf buf,
				 ViConstString writeFmt, ViVAList params);
ViStatus viScanf		(ViSession vi, ViConstString readFmt, ...);
ViStatus viVScanf		(ViSession vi, ViConstString readFmt,
				 ViVAList params);
ViStatus viSScanf		(ViSession vi, ViConstBuf buf,
				 ViConstString readFmt, ...);
ViStatus viVSScanf		(ViSession vi, ViConstBuf buf,
				 ViConstString readFmt, ViVAList params);
ViStatus viQueryf		(ViSession vi, ViConstString writeFmt,
				 ViConstString readFmt, ...);
ViStatus viVQueryf		(ViSession vi, ViConstString writeFmt,
				 ViConstString readFmt, ViVAList params);
ViStatus viIn8			(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViPUInt8 val8);
ViStatus viOut8			(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViUInt8 val8);
ViStatus viIn16			(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViPUInt16 val16);
ViStatus viOut16		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViUInt16 val16);
ViStatus viIn32			(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViPUInt32 val32);
ViStatus viOut32		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViUInt32 val32);
ViStatus viIn64			(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViPUInt64 val64);
ViStatus viOut64	       	(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViUInt64 val64);
ViStatus viIn8Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViPUInt8 val8);
ViStatus viOut8Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViUInt8 val8);
ViStatus viIn16Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViPUInt16 val16);
ViStatus viOut16Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViUInt16 val16);
ViStatus viIn32Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViPUInt32 val32);
ViStatus viOut32Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViUInt32 val32);
ViStatus viIn64Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViPUInt64 val64);
ViStatus viOut64Ex	       	(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViUInt64 val64);
ViStatus viMoveIn8		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt8 val8);
ViStatus viMoveOut8		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt8 val8);
ViStatus viMoveIn16		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt16 val16);
ViStatus viMoveOut16		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt16 val16);
ViStatus viMoveIn32		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt32 val32);
ViStatus viMoveOut32		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt32 val32);
ViStatus viMoveIn64		(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt64 val64);
ViStatus viMoveOut64	       	(ViSession vi, ViUInt16 space,
				 ViBusAddress offset, ViBusSize length,
				 ViAUInt64 val64);
ViStatus viMoveIn8Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt8 val8);
ViStatus viMoveOut8Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt8 val8);
ViStatus viMoveIn16Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt16 val16);
ViStatus viMoveOut16Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt16 val16);
ViStatus viMoveIn32Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt32 val32);
ViStatus viMoveOut32Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt32 val32);
ViStatus viMoveIn64Ex		(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViPUInt64 val64);
ViStatus viMoveOut64Ex	       	(ViSession vi, ViUInt16 space,
				 ViBusAddress64 offset, ViBusSize length,
				 ViAUInt64 val64);
ViStatus viMove			(ViSession vi, ViUInt16 srcSpace,
				 ViBusAddress srcOffset, ViUInt16 srcWidth,
				 ViUInt16 destSpace, ViBusAddress DestOffset,
				 ViUInt16 destWidth, ViBusSize srcLength);
ViStatus viMoveAsync		(ViSession vi, ViUInt16 srcSpace,
				 ViBusAddress srcOffset, ViUInt16 srcWidth,
				 ViUInt16 destSpace, ViBusAddress DestOffset,
				 ViUInt16 destWidth, ViBusSize srcLength,
				 ViPJobId jobId);
ViStatus viMoveEx		(ViSession vi, ViUInt16 srcSpace,
				 ViBusAddress64 srcOffset, ViUInt16 srcWidth,
				 ViUInt16 destSpace, ViBusAddress64 DestOffset,
				 ViUInt16 destWidth, ViBusSize srcLength);
ViStatus viMoveAsyncEx		(ViSession vi, ViUInt16 srcSpace,
				 ViBusAddress64 srcOffset, ViUInt16 srcWidth,
				 ViUInt16 destSpace, ViBusAddress64 DestOffset,
				 ViUInt16 destWidth, ViBusSize srcLength,
				 ViPJobId jobId);
ViStatus viMapAddress		(ViSession vi, ViUInt16 mapSapce,
				 ViBusAddress mapOffset, ViBusSize mapSize,
				 ViBoolean access, ViAddr suggested,
				 ViPAddr address);
ViStatus viUnmapAddress		(ViSession vi);
ViStatus viMapAddressEx		(ViSession vi, ViUInt16 mapSapce,
				 ViBusAddress64 mapOffset, ViBusSize mapSize,
				 ViBoolean access, ViAddr suggested,
				 ViPAddr address);
void viPeek8			(ViSession vi, ViAddr address, ViPUInt8 val8);
void viPoke8			(ViSession vi, ViAddr address, ViUInt8 val8);
void viPeek16			(ViSession vi, ViAddr address, ViPUInt16 val16);
void viPoke16			(ViSession vi, ViAddr address, ViUInt16 val16);
void viPeek32			(ViSession vi, ViAddr address, ViPUInt32 val32);
void viPoke32			(ViSession vi, ViAddr address, ViUInt32 val32);
void viPeek64			(ViSession vi, ViAddr address, ViPUInt64 val64);
void viPoke64			(ViSession vi, ViAddr address, ViUInt64 val64);
ViStatus viMemAlloc		(ViSession vi, ViBusSize size,
				 ViPBusAddress offset);
ViStatus viMemFree		(ViSession vi, ViBusAddress offset);
ViStatus viMemAllocEx		(ViSession vi, ViBusSize size,
				 ViPBusAddress64 offset);
ViStatus viMemFreeEx		(ViSession vi, ViBusAddress64 offset);
ViStatus viGpibControlREN	(ViSession vi, ViUInt16 mode);
ViStatus viGpibControlATN	(ViSession vi, ViUInt16 mode);
ViStatus viGpibSendIFC		(ViSession vi);
ViStatus viGpibCommand		(ViSession vi, ViConstBuf cmd, ViUInt32 cnt,
				 ViPUInt32 retCnt);
ViStatus viGpibPassControl	(ViSession vi, ViUInt16 primAddr,
				 ViUInt16 secAddr);
ViStatus viVxiCommandQuery	(ViSession vi, ViUInt16 mode, ViUInt32 cmd,
				 ViPUInt32 response);
ViStatus viAssertUtilSignal	(ViSession vi, ViUInt16 line);
ViStatus viAssertIntrSignal	(ViSession vi, ViInt16 mode, ViUInt32 statusId);
ViStatus viMapTrigger		(ViSession vi, ViInt16 trigSrc,
				 ViInt16 trigDest, ViUInt16 mode);
ViStatus viUnmapTrigger		(ViSession vi, ViInt16 trgSrc,
				 ViInt16 trigDest);
ViStatus viUsbControlOut	(ViSession vi, ViInt16 bmRequestType,
				 ViInt16 bRequest, ViUInt16 wValue,
				 ViUInt16 wIndex, ViUInt16 wLength,
				 ViConstBuf buf);
ViStatus viUsbControlOIN	(ViSession vi, ViInt16 bmRequestType,
				 ViInt16 bRequest, ViUInt16 wValue,
				 ViUInt16 wIndex, ViUInt16 wLength,
				 ViConstBuf buf, ViPUInt16 retCnt);
ViStatus viPxiReserveTriggers	(ViSession vi, ViInt16 cnt, ViAInt16 trigBuses,
				 ViAInt16 trigLines, ViPInt16 failureIndex);

// 3.3 Completion and Error Codes
// Table 3.3.1

#define VI_SUCCESS			((ViStatus)0)
#define VI_SUCCESS_EVENT_EN		((ViStatus)0x3FFF0002)
#define VI_SUCCESS_EVENT_DIS		((ViStatus)0x3FFF0003)
#define VI_SUCCESS_QUEUE_EMPTY		((ViStatus)0x3FFF0004)
#define VI_SUCCESS_TERM_CHAR		((ViStatus)0x3FFF0005)
#define VI_SUCCESS_MAX_CNT		((ViStatus)0x3FFF0006)
#define VI_SUCCESS_DEV_NPRESENT		((ViStatus)0x3FFF007D)
#define VI_SUCCESS_TRIG_MAPPED		((ViStatus)0x3FFF007E)
#define VI_SUCCESS_QUEUE_NEMPTY		((ViStatus)0x3FFF0080)
#define VI_SUCCESS_NCHAIN		((ViStatus)0x3FFF0098)
#define VI_SUCCESS_NESTED_SHARED	((ViStatus)0x3FFF0099)
#define VI_SUCCESS_NESTED_EXCLUSIVE	((ViStatus)0x3FFF009A)
#define VI_SUCCESS_SYNC			((ViStatus)0x3FFF009B)

#define VI_WARN_QUEUE_OVERFLOW		((ViStatus)0x3FFF000C)
#define VI_WARN_CONFIG_NLOADED		((ViStatus)0x3FFF0077)
#define VI_WARN_NULL_OBJECT		((ViStatus)0x3FFF0082)
#define VI_WARN_NSUP_ATTR_STATE		((ViStatus)0x3FFF0004)
#define VI_WARN_UNKNOWN_STATUS		((ViStatus)0x3FFF0085)
#define VI_WARN_NSUP_BUF		((ViStatus)0x3FFF0088)
#define VI_WARN_EXT_FUNC_NIMPL		((ViStatus)0x3FFF00A9)
#define VI_WARN_SERVER_CERT_UNTRUSTED	((ViStatus)0x3FFF00F0)
#define VI_WARN_SERVER_CERT_INV_OBJECT	((ViStatus)0x3FFF00F1)

#define VI_ERROR_SYSTEM_ERROR		((ViStatus)0xBFFF0000)
#define VI_ERROR_INV_OBJECT		((ViStatus)0xBFFF000E)
#define VI_ERROR_INV_SESSION		((ViStatus)0xBFFF000E) // FIXME
#define VI_ERROR_RSRC_LOCKED		((ViStatus)0xBFFF000F)
#define VI_ERROR_INV_EXPR		((ViStatus)0xBFFF0010)
#define VI_ERROR_RSRC_NFOUND		((ViStatus)0xBFFF0011)
#define VI_ERROR_INV_RSRC_NAME		((ViStatus)0xBFFF0012)
#define VI_ERROR_INV_ACC_MODE		((ViStatus)0xBFFF0013)
#define VI_ERROR_TMO			((ViStatus)0xBFFF0015)
#define VI_ERROR_CLOSING_FAILED		((ViStatus)0xBFFF0016)
#define VI_ERROR_INV_DEGREE		((ViStatus)0xBFFF001B)
#define VI_ERROR_INV_JOB_ID		((ViStatus)0xBFFF001C)
#define VI_ERROR_NSUP_ATTR		((ViStatus)0xBFFF001D)
#define VI_ERROR_NSUP_ATTR_STATE	((ViStatus)0xBFFF001E)
#define VI_ERROR_ATTR_READONLY		((ViStatus)0xBFFF001F)
#define VI_ERROR_INV_LOCK_TYPE		((ViStatus)0xBFFF0020)
#define VI_ERROR_INV_ACCESS_KEY		((ViStatus)0xBFFF0021)
#define VI_ERROR_INV_EVENT		((ViStatus)0xBFFF0026)
#define VI_ERROR_INV_MECH		((ViStatus)0xBFFF0027)
#define VI_ERROR_HNDLR_NINSTALLED	((ViStatus)0xBFFF0028)
#define VI_ERROR_INV_HNDLR_REF		((ViStatus)0xBFFF0029)
#define VI_ERROR_INV_CONTEXT		((ViStatus)0xBFFF002A)
#define VI_ERROR_NENABLED		((ViStatus)0xBFFF002F)
#define VI_ERROR_ABORT			((ViStatus)0xBFFF0030)
#define VI_ERROR_RAW_WR_PROT_VIOL	((ViStatus)0xBFFF0034)
#define VI_ERROR_RAW_RD_PROT_VIOL	((ViStatus)0xBFFF0035)
#define VI_ERROR_OUTP_PROT_VIOL		((ViStatus)0xBFFF0036)
#define VI_ERROR_INP_PROT_VIOL		((ViStatus)0xBFFF0037)
#define VI_ERROR_BERR			((ViStatus)0xBFFF0038)
#define VI_ERROR_IN_PROGRESS		((ViStatus)0xBFFF0039)
#define VI_ERROR_INV_SETUP		((ViStatus)0xBFFF003A)
#define VI_ERROR_QUEUE_ERROR		((ViStatus)0xBFFF003B)
#define VI_ERROR_ALLOC			((ViStatus)0xBFFF003C)
#define VI_ERROR_INV_MASK		((ViStatus)0xBFFF003D)
#define VI_ERROR_IO			((ViStatus)0xBFFF003E)
#define VI_ERROR_INV_FMT		((ViStatus)0xBFFF003F)
#define VI_ERROR_NSUP_FMT		((ViStatus)0xBFFF0041)
#define VI_ERROR_LINE_IN_USE		((ViStatus)0xBFFF0042)
#define VI_ERROR_LINE_NRESERVED		((ViStatus)0xBFFF0043)
#define VI_ERROR_NSUP_MODE		((ViStatus)0xBFFF0046)
#define VI_ERROR_SRQ_NOCCURRED		((ViStatus)0xBFFF004A)
#define VI_ERROR_INV_SPACE		((ViStatus)0xBFFF004E)
#define VI_ERROR_INV_OFFSET		((ViStatus)0xBFFF0051)
#define VI_ERROR_INV_WIDTH		((ViStatus)0xBFFF0052)
#define VI_ERROR_NSUP_OFFSET		((ViStatus)0xBFFF0054)
#define VI_ERROR_NSUP_VAR_WIDTH		((ViStatus)0xBFFF0055)
#define VI_ERROR_WINDOW_NMAPPED		((ViStatus)0xBFFF0057)
#define VI_ERROR_RESP_PENDING		((ViStatus)0xBFFF0059)
#define VI_ERROR_NLISTERNERS		((ViStatus)0xBFFF005F)
#define VI_ERROR_NCIC			((ViStatus)0xBFFF0060)
#define VI_ERROR_NSYS_CNTLR		((ViStatus)0xBFFF0061)
#define VI_ERROR_NSUP_OPER		((ViStatus)0xBFFF0067)
#define VI_ERROR_INTR_PENDING		((ViStatus)0xBFFF0068)
#define VI_ERROR_ASRL_PARITY		((ViStatus)0xBFFF006A)
#define VI_ERROR_ASRL_FRAMING		((ViStatus)0xBFFF006B)
#define VI_ERROR_ASRL_OVERRUN		((ViStatus)0xBFFF006C)
#define VI_ERROR_TRIG_NMAPPED		((ViStatus)0xBFFF006E)
#define VI_ERROR_NSUP_ALIGN_OFFSET	((ViStatus)0xBFFF0070)
#define VI_ERROR_USER_BUF		((ViStatus)0xBFFF0071)
#define VI_ERROR_RSRC_BUSY		((ViStatus)0xBFFF0072)
#define VI_ERROR_NSUP_WIDTH		((ViStatus)0xBFFF0076)
#define VI_ERROR_INV_PARAMETER		((ViStatus)0xBFFF0078)
#define VI_ERROR_INV_PORT		((ViStatus)0xBFFF0079)
#define VI_ERROR_INV_SIZE		((ViStatus)0xBFFF007B)
#define VI_ERROR_WINDOW_MAPPED		((ViStatus)0xBFFF0080)
#define VI_ERROR_NIMPL_OPER		((ViStatus)0xBFFF0081)
#define VI_ERROR_INV_LENGTH		((ViStatus)0xBFFF0083)
#define VI_ERROR_INV_MODE		((ViStatus)0xBFFF0091)
#define VI_ERROR_SESN_NLOCKED		((ViStatus)0xBFFF009C)
#define VI_ERROR_MEM_NSHARED		((ViStatus)0xBFFF009D)
#define VI_ERROR_LUBRARY_NFOUND		((ViStatus)0xBFFF009E)
#define VI_ERROR_NSUP_INTR		((ViStatus)0xBFFF009F)
#define VI_ERROR_INV_LINE		((ViStatus)0xBFFF00A0)
#define VI_ERROR_FILE_ACCESS		((ViStatus)0xBFFF00A1)
#define VI_ERROR_FILE_IO		((ViStatus)0xBFFF00A2)
#define VI_ERROR_NSUP_LINE		((ViStatus)0xBFFF00A3)
#define VI_ERROR_NSUP_MECH		((ViStatus)0xBFFF00A4)
#define VI_ERROR_INTF_NUM_NCONFIG	((ViStatus)0xBFFF00A5)
#define VI_ERROR_CONN_LOST		((ViStatus)0xBFFF00A6)
#define VI_ERROR_NPERMISSION		((ViStatus)0xBFFF00A8)
#define VI_ERROR_SERVER_CERT		((ViStatus)0xBFFF00B0)
#define VI_ERROR_SERVER_CERT_UNTRUSTED	((ViStatus)0xBFFF00B1)
#define VI_ERROR_SERVER_CERT_EXPIRED	((ViStatus)0xBFFF00B2)
#define VI_ERROR_SERVER_CERT_REVOKED	((ViStatus)0xBFFF00B3)
#define VI_ERROR_SERVER_CERT_INV_SUBJECT	((ViStatus)0xBFFF00B4)

// 3.4 Attribute Values
// Table 3.4.1

#define VI_ATTR_RSRC_CLASS		((ViAttr)0xBFFF0001L)
#define VI_ATTR_RSRC_NAME		((ViAttr)0xBFFF0002L)

#define VI_ATTR_RSRC_IMPL_VERSION	((ViAttr)0x3FFF0003L)
#define VI_ATTR_RSRC_LOCK_STATE		((ViAttr)0x3FFF0004L)
#define VI_ATTR_MAX_QUEUE_LENGTH	((ViAttr)0x3FFF0005L)
#define VI_ATTR_USER_DATA_32		((ViAttr)0x3FFF0007L)
#define VI_ATTR_USER_DATA_64		((ViAttr)0x3FFF000AL)
#ifdef _VISA_ENV_32BIT
#define VI_ATTR_USER_DATA		VI_ATTR_USER_DATA_32
#endif
#ifdef _VISA_ENV_64BIT
#define VI_ATTR_USER_DATA		VI_ATTR_USER_DATA_64
#endif
#define VI_ATTR_FDC_CHNL		((ViAttr)0x3FFF000DL)
#define VI_ATTR_FDC_MODE		((ViAttr)0x3FFF000FL)
#define VI_ATTR_FDC_GEN_SIGNAL_EN	((ViAttr)0x3FFF0011L)
#define VI_ATTR_FDC_USE_PAIR		((ViAttr)0x3FFF0013L)
#define VI_ATTR_SEND_END_EN		((ViAttr)0x3FFF0016L)
#define VI_ATTR_TERMCHAR		((ViAttr)0x3FFF0018L)
#define VI_ATTR_TMO_VALUE		((ViAttr)0x3FFF001AL)
#define VI_ATTR_GPIB_READDR_EN		((ViAttr)0x3FFF001BL)
#define VI_ATTR_IO_PROT			((ViAttr)0x3FFF001CL)
#define VI_ATTR_DMA_ALLOW_EN		((ViAttr)0x3FFF001EL)
#define VI_ATTR_ASRL_BAUD		((ViAttr)0x3FFF0021L)
#define VI_ATTR_ASRL_DATA_BITS		((ViAttr)0x3FFF0022L)
#define VI_ATTR_ASRL_PARITY		((ViAttr)0x3FFF0023L)
#define VI_ATTR_ASRL_STOP_BITS		((ViAttr)0x3FFF0024L)
#define VI_ATTR_ASRL_FLOW_CNTRL		((ViAttr)0x3FFF0025L)
#define VI_ATTR_RD_BUF_OPER_MODE	((ViAttr)0x3FFF002AL)
#define VI_ATTR_RD_BUF_SIZE		((ViAttr)0x3FFF002BL)
#define VI_ATTR_WR_BUF_OPER_MODE	((ViAttr)0x3FFF002DL)
#define VI_ATTR_WR_BUF_SIZE		((ViAttr)0x3FFF002EL)
#define VI_ATTR_SUPPRESS_END_EN		((ViAttr)0x3FFF0036L)
#define VI_ATTR_TERMCHAR_EN		((ViAttr)0x3FFF0038L)
#define VI_ATTR_DEST_ACCESS_PRIV	((ViAttr)0x3FFF0039L)
#define VI_ATTR_DEST_BYTE_ORDER		((ViAttr)0x3FFF003AL)
#define VI_ATTR_SRC_ACCESS_ORIV		((ViAttr)0x3FFF003CL)
#define VI_ATTR_SRC_BYTE_ORDER		((ViAttr)0x3FFF003DL)
#define VI_ATTR_SRC_INCREMENT		((ViAttr)0x3FFF0040L)
#define VI_ATTR_DEST_INCREMENT		((ViAttr)0x3FFF0041L)
#define VI_ATTR_WIN_ACCESS_PRIV		((ViAttr)0x3FFF0045L)
#define VI_ATTR_WIN_BYTE_ORDER		((ViAttr)0x3FFF0047L)
#define VI_ATTR_GPIB_ATN_STATE		((ViAttr)0x3FFF0057L)
#define VI_ATTR_GPIB_ADDR_STATE		((ViAttr)0x3FFF005CL)
#define VI_ATTR_GPIB_CIC_STATE		((ViAttr)0x3FFF005EL)
#define VI_ATTR_GPIB_NDAC_STATE		((ViAttr)0x3FFF0062L)
#define VI_ATTR_GPIB_SRQ_STATE		((ViAttr)0x3FFF0067L)
#define VI_ATTR_GPIB_SYS_CNTRL_STATE	((ViAttr)0x3FFF0068L)
#define VI_ATTR_GPIB_HS488_CBL_LEN	((ViAttr)0x3FFF0069L)
#define VI_ATTR_CMDR_LA			((ViAttr)0x3FFF006BL)
#define VI_ATTR_VXI_DEV_CLASS		((ViAttr)0x3FFF006CL)
#define VI_ATTR_MAINFRAME_LA		((ViAttr)0x3FFF0070L)
#define VI_ATTR_MANF_NAME		((ViAttr)0x3FFF0072L)
#define VI_ATTR_MODEL_NAME		((ViAttr)0x3FFF0077L)
#define VI_ATTR_VXI_VME_INTR_STATUS	((ViAttr)0x3FFF008BL)
#define VI_ATTR_VXI_TRIG_STATUS		((ViAttr)0x3FFF008DL)
#define VI_ATTR_VXI_VME_SYSFAIL_STATE	((ViAttr)0x3FFF0094L)
#define VI_ATTR_WIN_BASE_ADDR		((ViAttr)0x3FFF0098L)
#define VI_ATTR_WIN_SIZE		((ViAttr)0x3FFF009AL)
#define VI_ATTR_ASRL_AVAIL_NUM		((ViAttr)0x3FFF00ACL)
#define VI_ATTR_MEM_BASE		((ViAttr)0x3FFF00ADL)
#define VI_ATTR_ASRL_CTS_STATE		((ViAttr)0x3FFF00AEL)
#define VI_ATTR_ASRL_DCD_STATE		((ViAttr)0x3FFF00AFL)
#define VI_ATTR_ASRL_DSR_STATE		((ViAttr)0x3FFF00B1L)
#define VI_ATTR_ASRL_DTR_STATE		((ViAttr)0x3FFF00B2L)
#define VI_ATTR_ASRL_END_IN		((ViAttr)0x3FFF00B3L)
#define VI_ATTR_ASRL_END_OUT		((ViAttr)0x3FFF00B4L)
#define VI_ATTR_ASRL_REPLACE_CHAR	((ViAttr)0x3FFF00BEL)
#define VI_ATTR_ASRL_RI_STATE		((ViAttr)0x3FFF00BFL)
#define VI_ATTR_ASRL_RTS_STATE		((ViAttr)0x3FFF00C0L)
#define VI_ATTR_ASRL_XON_CHAR		((ViAttr)0x3FFF00C1L)
#define VI_ATTR_ASRL_XOFF_CHAR		((ViAttr)0x3FFF00C2L)
#define VI_ATTR_WIN_ACCESS		((ViAttr)0x3FFF00C3L)
#define VI_ATTR_RM_SESSION		((ViAttr)0x3FFF00C4L)
#define VI_ATTR_VXI_LA			((ViAttr)0x3FFF00D5L)
#define VI_ATTR_MANF_ID			((ViAttr)0x3FFF00D9L)
#define VI_ATTR_MEM_SIZE		((ViAttr)0x3FFF00DDL)
#define VI_ATTR_MEM_SPACE		((ViAttr)0x3FFF00DEL)
#define VI_ATTR_MODEL_CODE		((ViAttr)0x3FFF00DFL)
#define VI_ATTR_SLOT			((ViAttr)0x3FFF00E8L)
#define VI_ATTR_INTF_INST_NAME		((ViAttr)0xBFFF00E9L)
#define VI_ATTR_IMMEDIATE_SERV		((ViAttr)0x3FFF0100L)
#define VI_ATTR_INTF_PARENT_NUM		((ViAttr)0x3FFF0101L)
#define VI_ATTR_RSRC_SPEC_VERSION	((ViAttr)0x3FFF0170L)
#define VI_ATTR_INTF_TYPE		((ViAttr)0x3FFF0171L)
#define VI_ATTR_GPIB_PRIMARY_ADDR	((ViAttr)0x3FFF0172L)
#define VI_ATTR_GPIB_SECONDARY_ADDR	((ViAttr)0x3FFF0173L)
#define VI_ATTR_RSRC_MANF_NAME		((ViAttr)0xBFFF0174L)
#define VI_ATTR_RSRC_MANF_ID		((ViAttr)0x3FFF0175L)
#define VI_ATTR_INTF_NJUM		((ViAttr)0x3FFF0176L)
#define VI_ATTR_TRIG_ID			((ViAttr)0x3FFF0177L)
#define VI_ATTR_GPIB_REN_STATE		((ViAttr)0x3FFF0181L)
#define VI_ATTR_GPIB_UNADDR_EN		((ViAttr)0x3FFF0184L)
#define VI_ATTR_DEV_STATUS_BYTE		((ViAttr)0x3FFF0189L)
#define VI_ATTR_FILE_APPREND_EN		((ViAttr)0x3FFF0192L)
#define VI_ATTR_VXI_TRIG_SUPPORT	((ViAttr)0x3FFF0194L)
#define VI_ATTR_TCPIP_ADDR		((ViAttr)0xBFFF0195L)
#define VI_ATTR_TCPIP_HOSTNAME		((ViAttr)0xBFFF0196L)
#define VI_ATTR_TCPIP_PORT		((ViAttr)0x3FFF0197L)
#define VI_ATTR_TCPIP_DEVICE_NAME	((ViAttr)0xBFFF0199L)
#define VI_ATTR_TCPIP_NODELAY		((ViAttr)0x3FFF019AL)
#define VI_ATTR_TCPIP_KEEPALIVE		((ViAttr)0x3FFF019BL)
#define VI_ATTR_4882_COMPLIANT		((ViAttr)0x3FFF019FL)
#define VI_ATTR_USB_SERIAL_NUM		((ViAttr)0xBFFF01A0L)
#define VI_ATTR_USB_INTFC_NUM		((ViAttr)0x3FFF01A1L)
#define VI_ATTR_USB_PROTOCOL		((ViAttr)0x3FFF01A7L)
#define VI_ATTR_USB_MAX_INTR_SIZE	((ViAttr)0x3FFF01AFL)
#define VI_ATTR_JOB_ID			((ViAttr)0x3FFF4006L)
#define VI_ATTR_EVENT_TYPE		((ViAttr)0x3FFF4010L)
#define VI_ATTR_SIGP_STATUS_ID		((ViAttr)0x3FFF4011L)
#define VI_ATTR_RECV_TRIG_ID		((ViAttr)0x3FFF4012L)
#define VI_ATTR_INTR_STATUS_ID		((ViAttr)0x3FFF4023L)
#define VI_ATTR_STATUS			((ViAttr)0x3FFF4025L)
#define VI_ATTR_RET_COUNT		((ViAttr)0x3FFF4026L)
#define VI_ATTR_BUFFER			((ViAttr)0x3FFF4027L)
#define VI_ATTR_RECV_INTR_LEVEL		((ViAttr)0x3FFF4041L)
#define VI_ATTR_OPER_NAME		((ViAttr)0xBFFF4042L)
#define VI_ATTR_GPIB_RECV_CIC_STATE	((ViAttr)0x3FFF4193L)
#define VI_ATTR_RECV_TCPIP_ADDR		((ViAttr)0xBFFF4198L)
#define VI_ATTR_USB_RECV_INTR_SIZE	((ViAttr)0x3FFF41B0L)
#define VI_ATTR_USB_RECV_INTR_DATA	((ViAttr)0xBFFF41B1L)
#define VI_ATTR_PXI_DEV_NUM		((ViAttr)0x3FFF0201L)
#define VI_ATTR_PXI_FUNC_NUM		((ViAttr)0x3FFF0202L)
#define VI_ATTR_PXI_BUS_NUM		((ViAttr)0x3FFF0205L)
#define VI_ATTR_PXI_CHASSI		((ViAttr)0x3FFF0206L)
#define VI_ATTR_PXI_SLOTPATH		((ViAttr)0x3FFF0207L)
#define VI_ATTR_PXI_SLOT_LBUS_LEFT	((ViAttr)0x3FFF0208L)
#define VI_ATTR_PXI_SLOT_LBUS_RIGHT	((ViAttr)0x3FFF0209L)
#define VI_ATTR_PXI_TRIG_BUS		((ViAttr)0x3FFF020AL)
#define VI_ATTR_PXI_STAR_TRIG_BUS	((ViAttr)0x3FFF020BL)
#define VI_ATTR_PXI_STAR_TRIG_LINE	((ViAttr)0x3FFF020CL)
#define VI_ATTR_PXI_MEM_TYPE_BAR0	((ViAttr)0x3FFF0211L)
#define VI_ATTR_PXI_MEM_TYPE_BAR1	((ViAttr)0x3FFF0212L)
#define VI_ATTR_PXI_MEM_TYPE_BAR2	((ViAttr)0x3FFF0213L)
#define VI_ATTR_PXI_MEM_TYPE_BAR3	((ViAttr)0x3FFF0214L)
#define VI_ATTR_PXI_MEM_TYPE_BAR4	((ViAttr)0x3FFF0215L)
#define VI_ATTR_PXI_MEM_TYPE_BAR5	((ViAttr)0x3FFF0216L)
#define VI_ATTR_PXI_MEM_BASE_BAR0_32	((ViAttr)0x3FFF0221L)
#define VI_ATTR_PXI_MEM_BASE_BAR1_32	((ViAttr)0x3FFF0222L)
#define VI_ATTR_PXI_MEM_BASE_BAR2_32	((ViAttr)0x3FFF0223L)
#define VI_ATTR_PXI_MEM_BASE_BAR3_32	((ViAttr)0x3FFF0224L)
#define VI_ATTR_PXI_MEM_BASE_BAR4_32	((ViAttr)0x3FFF0225L)
#define VI_ATTR_PXI_MEM_BASE_BAR5_32	((ViAttr)0x3FFF0226L)
#define VI_ATTR_PXI_MEM_SIZE_BAR0_32	((ViAttr)0x3FFF0231L)
#define VI_ATTR_PXI_MEM_SIZE_BAR1_32	((ViAttr)0x3FFF0232L)
#define VI_ATTR_PXI_MEM_SIZE_BAR2_32	((ViAttr)0x3FFF0233L)
#define VI_ATTR_PXI_MEM_SIZE_BAR3_32	((ViAttr)0x3FFF0234L)
#define VI_ATTR_PXI_MEM_SIZE_BAR4_32	((ViAttr)0x3FFF0235L)
#define VI_ATTR_PXI_MEM_SIZE_BAR5_32	((ViAttr)0x3FFF0236L)
#define VI_ATTR_PXI_IS_EXPRESS		((ViAttr)0x3FFF0240L)
#define VI_ATTR_PXI_SLOT_LWIDTH		((ViAttr)0x3FFF0241L)
#define VI_ATTR_PXI_MAX_LWIDTH		((ViAttr)0x3FFF0242L)
#define VI_ATTR_PXI_ACTUAL_LWIDTH	((ViAttr)0x3FFF0243L)
#define VI_ATTR_PXI_DSTAR_BUS		((ViAttr)0x3FFF0244L)
#define VI_ATTR_PXI_DSTAR_SET		((ViAttr)0x3FFF0245L)
#define VI_ATTR_PXI_SLOT_WIDTH		((ViAttr)0x3FFF0247L)
#define VI_ATTR_PXI_SLOT_OFFSET		((ViAttr)0x3FFF0248L)
#define VI_ATTR_TCPIP_SERVER_CERT_ISSUER_NAME		((ViAttr)0xBFFF0270L)
#define VI_ATTR_TCPIP_SERVER_CERT_SUBJECT_NAME		((ViAttr)0xBFFF0271L)
#define VI_ATTR_TCPIP_SERVER_CERT_EXPIRATION_DATE	((ViAttr)0xBFFF0272L)
#define VI_ATTR_TCPIP_SERVER_CERT_IS_PERPETUAL		((ViAttr)0x3FFF0273L)
#define VI_ATTR_TCPIP_SASL_MECHANISM	((ViAttr)0xBFFF0274L)
#define VI_ATTR_TCPIP_TLS_CIPHER_SUITE	((ViAttr)0xBFFF0275L)
#define VI_ATTR_TCPIP_SERVER_CERT	((ViAttr)0xBFFF0276L)
#define VI_ATTR_TCPIP_SERVER_CERT_SIZE	((ViAttr)0x3FFF0277L)
#define VI_ATTR_TCPIP_HISLIP_OVERLAP_EN	((ViAttr)0x3FFF0300L)
#define VI_ATTR_TCPIP_HISLIP_VERSION	((ViAttr)0x3FFF0301L)
#define VI_ATTR_TCPIP_HISLIP_MAX_MESSAGE_KB	((ViAttr)0x3FFF0302L)
#define VI_ATTR_TCPIP_IS_HISlip		((ViAttr)0x3FFF0303L)
#define VI_ATTR_TCPIP_HISLIP_ENCRYPTION_EN	((ViAttr)0x3FFF0304L)
#define VI_ATTR_PXI_RECV_INTR_SEQ	((ViAttr)0x3FFF4240L)
#define VI_ATTR_PXI_RECV_INTR_DATA	((ViAttr)0x3FFF4241L)
#define VI_ATTR_PXI_SRC_TRIG_BUS	((ViAttr)0x3FFF020DL)
#define VI_ATTR_PXI_DEST_TRIG_BUS	((ViAttr)0x3FFF020RL)
#define VI_ATTR_PXI_MEM_BASE_BAR0_64	((ViAttr)0x3FFF0228L)
#define VI_ATTR_PXI_MEM_BASE_BAR1_64	((ViAttr)0x3FFF0229L)
#define VI_ATTR_PXI_MEM_BASE_BAR2_64	((ViAttr)0x3FFF022AL)
#define VI_ATTR_PXI_MEM_BASE_BAR3_64	((ViAttr)0x3FFF022BL)
#define VI_ATTR_PXI_MEM_BASE_BAR4_64	((ViAttr)0x3FFF022CL)
#define VI_ATTR_PXI_MEM_BASE_BAR5_64	((ViAttr)0x3FFF022DL)
#define VI_ATTR_PXI_MEM_SIZE_BAR0_64	((ViAttr)0x3FFF0238L)
#define VI_ATTR_PXI_MEM_SIZE_BAR1_64	((ViAttr)0x3FFF0239L)
#define VI_ATTR_PXI_MEM_SIZE_BAR2_64	((ViAttr)0x3FFF023AL)
#define VI_ATTR_PXI_MEM_SIZE_BAR3_64	((ViAttr)0x3FFF023BL)
#define VI_ATTR_PXI_MEM_SIZE_BAR4_64	((ViAttr)0x3FFF023CL)
#define VI_ATTR_PXI_MEM_SIZE_BAR5_64	((ViAttr)0x3FFF023DL)
#define VI_ATTR_PXI_ALLOW_WRITE_COMBINE		((ViAttr)0x3FFF0246L)

// 3.5 Event Type Values
// Table 3.5.1

#define VI_EVENT_IO_COMPLETION		((ViEvent)0x3FFF2009L)
#define VI_EVENT_TRIG			((ViEvent)0xBFFF200AL)
#define VI_EVENT_SERVICE_REQ		((ViEvent)0x3FFF200BL)
#define VI_EVENT_CLEAR			((ViEvent)0x3FFF200DL)
#define VI_EVENT_EXCEPTION		((ViEvent)0xBFFF200EL)
#define VI_EVENT_GPIB_CIC		((ViEvent)0x3FFF2012L)
#define VI_EVENT_GPIB_TALK		((ViEvent)0x3FFF2013L)
#define VI_EVENT_GPIB_LISTEN		((ViEvent)0x3FFF2014L)
#define VI_EVENT_VXI_VME_SYSFAIL	((ViEvent)0x3FFF201DL)
#define VI_EVENT_VXI_VME_SYSRESET	((ViEvent)0x3FFF201EL)
#define VI_EVENT_VXI_SIGP		((ViEvent)0x3FFF2020L)
#define VI_EVENT_VXI_VME_INTR		((ViEvent)0xBFFF2021L)
#define VI_EVENT_TCPIP_CONNECT		((ViEvent)0x3FFF2036L)
#define VI_EVENT_USB_INTR		((ViEvent)0x3FFF2037L)
#define VI_EVENT_PXI_INTR		((ViEvent)0x3FFF2022L)
#define VI_ALL_ENABLED_EVENTS		((ViEvent)0x3FFF7FFFL)

// 3.6 Values and Ranges
// Table 3.6.1

#define VI_FIND_BUFLEN			(256)

#define VI_NULL				(0)

//#define VI_TRUE				(1)
//#define VI_FALSE			(0)

#define VI_INTF_GPIB			(1)
#define VI_INTF_VXI			(2)
#define VI_INTF_GPIB_VXI		(3)
#define VI_INTF_ASRL			(4)
#define VI_INTF_PXI			(5)
#define VI_INTF_TCPIP			(6)
#define VI_INTF_USB			(7)

#define VI_NORMAL			(1)
#define VI_FDC				(2)
#define VI_HS488			(3)
#define	VI_ASRL488			(4)

#define VI_FDC_NORMAL			(1)
#define VI_FDC_STREAM			(2)

#define VI_LOCAL_SPACE			(0)
#define VI_A16_SPACE			(1)
#define VI_A24_SPACE			(2)
#define VI_A32_SPACE			(3)
#define VI_A64_SPACE			(4)
#define VI_PXI_ALLOC_SPACE		(9)
#define VI_PXI_CFG_SPACE		(10)
#define VI_PXI_BAR0_SPACE		(11)
#define VI_PXI_BAR1_SPACE		(12)
#define VI_PXI_BAR2_SPACE		(13)
#define VI_PXI_BAR3_SPACE		(14)
#define VI_PXI_BAR4_SPACE		(15)
#define VI_PXI_BAR5_SPACE		(16)
#define VI_OPAQUE_SPACE			(0xFFFF)

#define VI_UNKNOWN_SLOT			(-1)

#define VI_UNKNOWN_LA			(-1)

#define VI_UNKNOWN_LEVEL		(-1)

#define VI_QUEUE			(1)
#define VI_HNDLR			(2)
#define VI_SUSPEND_HNDLR		(4)
#define VI_ALL_MECH			(0xFFFF)

#define VI_ANY_HNDLR			(0)

#define VI_TRIG_ALL			(-2)
#define VI_TRIG_SW			(-1)
#define VI_TRIG_TTL0			(0)
#define VI_TRIG_TTL1			(1)
#define VI_TRIG_TTL2			(2)
#define VI_TRIG_TTL3			(3)
#define VI_TRIG_TTL4			(4)
#define VI_TRIG_TTL5			(5)
#define VI_TRIG_TTL6			(6)
#define VI_TRIG_TTL7			(7)
#define VI_TRIG_ECL0			(8)
#define VI_TRIG_ECL1			(9)
#define VI_TRIG_ECL2			(10)
#define VI_TRIG_ECL3			(11)
#define VI_TRIG_ECL4			(12)
#define VI_TRIG_ECL5			(13)
#define VI_TRIG_STAR_SLOT1		(14)
#define VI_TRIG_STAR_SLOT2		(15)
#define VI_TRIG_STAR_SLOT3		(16)
#define VI_TRIG_STAR_SLOT4		(17)
#define VI_TRIG_STAR_SLOT5		(18)
#define VI_TRIG_STAR_SLOT6		(19)
#define VI_TRIG_STAR_SLOT7		(20)
#define VI_TRIG_STAR_SLOT8		(21)
#define VI_TRIG_STAR_SLOT9		(22)
#define VI_TRIG_STAR_SLOT10		(23)
#define VI_TRIG_STAR_SLOT11		(24)
#define VI_TRIG_STAR_SLOT12		(25)
#define VI_TRIG_STAR_INSTR		(26)
#define VI_TRIG_PANEL_IN		(27)
#define VI_TRIG_PANEL_OUT		(28)
#define VI_TRIG_STAR_VXI0		(29)
#define VI_TRIG_STAR_VXI1		(30)
#define VI_TRIG_STAR_VXI2		(31)
#define VI_TRIG_TTL8			(32)
#define VI_TRIG_TTL9			(33)
#define VI_TRIG_TTL10			(34)
#define VI_TRIG_TTL11			(35)

#define VI_TRIG_PROT_DEFAULT		(0)
#define VI_TRIG_PROT_ON			(1)
#define VI_TRIG_PROT_OFF		(2)
#define VI_TRIG_PROT_SYNC		(5)
#define VI_TRIG_PROT_RESERVE		(6)
#define VI_TRIG_PROT_UNRESERVE		(7)

#define VI_READ_BUF			(1)
#define VI_WRITE_BUF			(2)
#define VI_READ_BUF_DISCARD		(4)
#define VI_WRITE_BUF_DISCARD		(8)
#define VI_ASRL_IN_BUF			(16)
#define VI_ASRL_OUT_BUF			(32)
#define VI_ASRL_IN_BUF_DISCARD		(64)
#define VI_ASRL_OUT_BUF_DISCARD		(128)

#define VI_FLUSH_ON_ACCESS		(1)
#define VI_FLUSH_WHEN_FULL		(2)
#define VI_FLUSH_DISABLE		(3)

#define VI_NMAPPED			(1)
#define VI_USE_OPERS			(2)
#define VI_DEREF_ADDR			(3)

#define VI_TMO_IMMEDIATE		(0)
#define VI_TMO_INFINITE			(0xFFFFFFFFUL)

#define VI_NO_LOCK			(0)
#define VI_EXCLUSIVE_LOCK		(1)
#define VI_SHARED_LOCK			(2)
#define VI_LOAD_CONFIG			(4)

#define VI_NO_SEC_ADDR			(0xFFFF)

#define VI_ASRL_PAR_NONE		(0)
#define VI_ASRL_PAR_ODD			(1)
#define VI_ASRL_PAR_EVEN		(2)
#define VI_ASRL_PAR_MARK		(3)
#define VI_ASRL_PAR_SPACE		(4)
#define VI_ASRL_STOP_ONE		(10)
#define VI_ASRL_STOP_ONE5		(15)
#define VI_ASRL_STOP_TWO		(20)

#define VI_ASRL_FLOW_NONE		(0)
#define VI_ASRL_FLOW_XON_XOFF		(1)
#define VI_ASRL_FLOW_RTS_CTS		(2)
#define VI_ASRL_FLOW_DTR_DSR		(4)

#define VI_ASRL_END_NONE		(0)
#define VI_ASRL_END_LAST_BIT		(1)
#define VI_ASRL_END_TERMCHAR		(2)
#define VI_ASRL_END_BREAK		(3)

#define VI_BIG_ENDIAN			(0)
#define VI_LITTLE_ENDIAN		(1)

#define VI_WIDTH_8			(1)
#define VI_WIDTH_16			(2)
#define VI_WIDTH_32			(4)
#define VI_WIDTH_64			(8)

#define VI_STATE_ASSERTED		(1)
#define VI_STATE_UNASSERTED		(0)
#define VI_STATE_UNKNOWN		(-1)

#define VI_GPIB_HS488_DISABLED		(0)
#define VI_GPIB_HS488_NIMPL		(-1)

#define VI_VXI_CLASS_MEMORY		(0)
#define VI_VXI_CLASS_EXTENDED		(1)
#define VI_VXI_CLASS_MESSAGE		(2)
#define VI_VXI_CLASS_REGISTER		(3)
#define VI_VXI_CLASS_OTHER		(4)

#define VI_UTIL_ASSERT_SYSRESET		(1)
#define VI_UTIL_ASSERT_SYSFAIL		(2)
#define VI_UTIL_DEASSERT_SYSFAIL	(3)

#define VI_DATA_PRIV			(0)
#define VI_DATA_NPRIV			(1)
#define VI_PROG_PRIV			(2)
#define VI_PROG_NPRIV			(3)
#define VI_BLCK_PRIV			(4)
#define VI_BLCK_NPRIV			(5)
#define VI_D64_PRIV			(6)
#define VI_D64_NPRIV			(7)

#define VI_GPIB_REN_DEASSERT		(0)
#define VI_GPIB_REN_ASSERT		(1)
#define VI_GPIB_REN_DEASSERT_GTL	(2)
#define VI_GPIB_REN_ASSERT_ADDRESS	(3)
#define VI_GPIB_REN_ASSERT_LLO		(4)
#define VI_GPIB_REN_ASSERT_ADDRESS_LLO	(5)
#define VI_GPIB_REN_ADDRESS_GTL		(6)

#define VI_VXI_CMD16			(0x0200)
#define VI_VXI_CMD16_RESP16		(0x0202)
#define VI_VXI_RESP16			(0x0002)
#define VI_VXI_CMD32			(0x0400)
#define VI_VXI_CMD32_RESP16		(0x0402)
#define VI_VXI_CMD32_RESP32		(0x0404)
#define VI_VXI_RESP32			(0x0004)

#define VI_GPIB_ATN_DEASSERT		(0)
#define VI_GPIB_ATN_ASSERT		(1)
#define VI_GPIB_ATN_DEASSERT_HANDSHAKE	(2)
#define VI_GPIB_ATN_ASSERT_IMMEDIATE	(3)

#define VI_ASSERT_SIGNAL		(-1)
#define VI_ASSERT_USE_ASSIGNED		(0)
#define VI_ASSERT_IRQ1			(1)
#define VI_ASSERT_IRQ2			(2)
#define VI_ASSERT_IRQ3			(3)
#define VI_ASSERT_IRQ4			(4)
#define VI_ASSERT_IRQ5			(5)
#define VI_ASSERT_IRQ6			(6)
#define VI_ASSERT_IRQ7			(7)

#define VI_GPIB_UNADDRESSED		(0)
#define VI_GPIB_TALKER			(1)
#define VI_GPIB_LISTENER		(2)

#define VI_PROT_NORMAL			(1)
#define VI_PROT_FDC			(2)
#define VI_PROT_HS488			(3)
#define VI_PROT_4882_STRS		(4)
#define VI_PROT_USBTMC_VENDOR		(5)

#define VI_PXI_ADDR_NONE		(0)
#define VI_PXI_ADDR_MEM			(1)
#define VI_PXI_ADDR_IO			(2)
#define VI_PXI_ADDR_CFG			(3)

#define VI_D64_2EVME			(8)
#define VI_D64_SST160			(9)
#define VI_D64_SST267			(10)
#define VI_D64_SST320			(11)

#define VI_IO_IN_BUF			(16)
#define VI_IO_OUT_BUF			(32)
#define VI_IO_IN_BUF_DISCARD		(64)
#define VI_IO_OUT_BUF_DISCARD		(128)

#define VI_UNKNOWN_CHASSI		(-1)

#define VI_UNKNWON_TRIG			(-1)

#define VI_PXI_LBUS_STAR_TRIG_BUS_0	(1000)
#define VI_PXI_LBUS_STAR_TRIG_BUS_1	(1001)
#define VI_PXI_LBUS_STAR_TRIG_BUS_2	(1002)
#define VI_PXI_LBUS_STAR_TRIG_BUS_3	(1003)
#define VI_PXI_LBUS_STAR_TRIG_BUS_4	(1004)
#define VI_PXI_LBUS_STAR_TRIG_BUS_5	(1005)
#define VI_PXI_LBUS_STAR_TRIG_BUS_6	(1006)
#define VI_PXI_LBUS_STAR_TRIG_BUS_7	(1007)
#define VI_PXI_LBUS_STAR_TRIG_BUS_8	(1008)
#define VI_PXI_LBUS_STAR_TRIG_BUS_9	(1009)
#define VI_PXI_STAR_TRIG_CONTROLLER	(1413)

// 3..8 Miscellaneous

#define VI_SPEC_VERSION		(0x00700200UL)

//#if define(__cplusplus) || define(__cplusplus__)
//   }
//#endif

#endif // __VISA_H
