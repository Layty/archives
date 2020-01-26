#include <stdio.h>
#include <string.h>
#include "cntt_proftree.h"


cntt_int32 cntt_TreeOpenNode(CNTT_HANDLE hNode, CNTT_PCSTR lpNode, cntt_uint32 type, CNTT_PHANDLE phnResult) 
{
    
    if (hNode == NULL)
    {
        TREECONTEXT_T *treecontext;

        treecontext = (TREECONTEXT_T*)malloc(sizeof(TREECONTEXT_T));
        if (treecontext == NULL)
            return ERROR_MEMORY_LACK;

        strcpy(treecontext->filepath, lpNode);
        treecontext->tree = treecontext;

        *phnResult = treecontext;
    }
    else
    {
        NODECONTEXT_T* ncontext, *subncontext;

        ncontext = (NODECONTEXT_T*)hNode;
        
        subncontext = (NODECONTEXT_T*)malloc(sizeof(NODECONTEXT_T));
        if (subncontext == NULL)
            return ERROR_MEMORY_LACK;

        memset(subncontext, 0, sizeof(NODECONTEXT_T));

        if (ncontext->tree != (TREECONTEXT_T*)ncontext)
        {
            strcpy(subncontext->path, ncontext->path);
            strcat(subncontext->path, "/");
        }
        strcat(subncontext->path, lpNode);
        cntt_GetProfileString(subncontext->path, ".child", "", subncontext->children, MTREE_NODECHILD_MAXLEN, ncontext->tree->filepath);
        subncontext->tree = ncontext->tree;

        *phnResult = subncontext;
    }

    return ERROR_SUCCESS;
}

cntt_int32 cntt_TreeCloseNode(CNTT_HANDLE hNode)
{
    if (hNode == NULL)
        return ERROR_INVALID_PARAMETER;

    free(hNode);

    return ERROR_SUCCESS;
}


cntt_int32 cntt_TreeQueryValue(CNTT_HANDLE hNode, CNTT_PCSTR lpValueName, cntt_uint8 *lpData, cntt_uint32 *lpcbData)
{
    NODECONTEXT_T* ncontext;

    if (hNode == NULL)
        return ERROR_INVALID_PARAMETER;
    
    ncontext = (NODECONTEXT_T*)hNode;
     
    if (ncontext->tree == (TREECONTEXT_T*)ncontext)
        return ERROR_INVALID_PARAMETER;
   
    *lpcbData = cntt_GetProfileString(ncontext->path, lpValueName, "", lpData, MTREE_NODEVALUE_MAXLEN, ncontext->tree->filepath);

    return ERROR_SUCCESS;
}
