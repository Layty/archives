#ifndef __CNTT_PROFILE_H__
#define __CNTT_PROFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cntt_typedef.h"


CNTT_BOOL   cntt_WriteProfileString(CNTT_PCSTR secName, CNTT_PCSTR keyName, CNTT_PCSTR keyString, CNTT_PCSTR fileName);
cntt_uint32 cntt_GetProfileString(CNTT_PCSTR secName, CNTT_PCSTR keyName, CNTT_PCSTR defaultString, CNTT_PSTR keyString, cntt_uint32 bufferSize, CNTT_PCSTR fileName);
CNTT_BOOL   cntt_WriteProfileInt(CNTT_PCSTR secName, CNTT_PCSTR keyName, cntt_int32 keyValue, CNTT_PCSTR fileName);
cntt_int32  cntt_GetProfileInt(CNTT_PCSTR secName, CNTT_PCSTR keyName, cntt_int32  defKeyValue, CNTT_PCSTR fileName);

#ifdef __cplusplus
}
#endif

#endif
