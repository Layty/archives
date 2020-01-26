#include <stdio.h>
#include "cntt_proftree.h"

int main()
{
    CNTT_HANDLE basehandle, roothandle, uri;
    char value[32];
    int count;

    cntt_TreeOpenNode(NULL, "mt.ini", NODE_TYPE_ALL_ACCESS, &basehandle);

    cntt_TreeOpenNode(basehandle, "Root", NODE_TYPE_ALL_ACCESS, &roothandle);

    cntt_TreeQueryValue(roothandle, "acl", value, &count);

    cntt_TreeOpenNode(roothandle, "DevDetail/URI", NODE_TYPE_ALL_ACCESS, &uri);

    return 0;
}