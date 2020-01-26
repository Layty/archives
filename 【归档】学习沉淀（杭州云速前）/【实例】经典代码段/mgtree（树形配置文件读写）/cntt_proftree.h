#ifndef __CNTT_PROFTREE_H__
#define __CNTT_PROFTREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cntt_profile.h"


#define NODE_QUERY_VALUE                   0
#define NODE_SET_VALUE                     1
#define NODE_READ                          2
#define NODE_TYPE_ALL_ACCESS               3


#define ERROR_SUCCESS                      0
#define ERROR_ACCESS_DENIED                -1
#define ERROR_FILE_NOT_FOUND               -2
#define ERROR_INVALID_PARAMETER            -3
#define ERROR_MEMORY_LACK                  -4


#define MTREE_FILENAME_MAXLEN              200 
#define MTREE_NODENAME_MAXLEN              100 
#define MTREE_NODEVALUE_MAXLEN             100 
#define MTREE_NODECHILD_MAXLEN             100 


    
typedef struct _TREECONTEXT_T
{
    struct _TREECONTEXT_T *tree;
    char filepath[MTREE_FILENAME_MAXLEN+1];
  
}TREECONTEXT_T;



typedef struct
{
    TREECONTEXT_T *tree;
    char path[MTREE_NODENAME_MAXLEN+1];
    char children[MTREE_NODECHILD_MAXLEN+1];
  
}NODECONTEXT_T;

cntt_int32 cntt_TreeOpenNode(CNTT_HANDLE hNode, CNTT_PCSTR lpNode, cntt_uint32 type, CNTT_PHANDLE phnResult); 
cntt_int32 cntt_TreeQueryValue(CNTT_HANDLE hNode, CNTT_PCSTR lpValueName, cntt_uint8 *lpData, cntt_uint32 *lpcbData);




#ifdef __cplusplus
}
#endif

#endif
