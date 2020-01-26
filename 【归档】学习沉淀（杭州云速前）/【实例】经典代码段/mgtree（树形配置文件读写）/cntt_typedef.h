#ifndef __CNTT_TYPEDEF_H__
#define __CNTT_TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef char                     cntt_string;
typedef unsigned char            cntt_ucs2string;
typedef unsigned char            cntt_uint8;
typedef unsigned short int	     cntt_uint16;
typedef unsigned long  int       cntt_uint32;
typedef signed   char            cntt_int8;
typedef signed   short int       cntt_int16;
typedef signed   long  int       cntt_int32;
typedef unsigned int             cntt_size;
typedef cntt_int32               cntt_int;


typedef enum
{
    cntt_false = 0,
    cntt_true = 1
}cntt_bool;


typedef cntt_string             *CNTT_PSTR;
typedef const cntt_string       *CNTT_PCSTR;

typedef int   CNTT_RESULT;
typedef void* CNTT_HANDLE;
typedef cntt_uint32 CNTT_EVENT;
typedef CNTT_HANDLE* CNTT_PHANDLE;


typedef unsigned long CNTT_COLORREF;

typedef enum
{
    CNTT_FALSE = 0,
    CNTT_TRUE = 1
}CNTT_BOOL;


#ifdef __cplusplus
}
#endif

#endif
