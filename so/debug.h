#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define DEBUGP(fmt, args...) \
    printf("[%s][%d][%s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)


#endif
