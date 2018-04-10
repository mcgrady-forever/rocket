#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <dlfcn.h>

#include "load_so.h"

namespace thunder {

#define PROCTECT_AREA_SIZE (512*1024)

thunder_dll_func_t thunder_dll = { NULL };

int load_so(const char* file)
{
    if (thunder_dll.handle != NULL)
    {
        dlclose(thunder_dll.handle);
    }

    memset(&thunder_dll, 0x0, sizeof(thunder_dll_func_t));

    int flag = RTLD_NOW;

    /* 提早发现部分内存写乱现象 */
    mmap(NULL, PROCTECT_AREA_SIZE, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    void* handle = dlopen(file, flag);
    mmap(NULL, PROCTECT_AREA_SIZE, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

    if (!handle)
    {
        fprintf(stderr, "dlopen %s failed, errmsg: %s\n", file, dlerror());
        exit(EXIT_FAILURE);
    }

    void* func = dlsym(handle, "thunder_handle_init");

    if (func != NULL)
    {
        thunder_dll.thunder_handle_init = (thunder_handle_init_t)dlsym(handle, "thunder_handle_init");
        thunder_dll.thunder_handle_input = (thunder_handle_input_t)dlsym(handle, "thunder_handle_input");
        thunder_dll.thunder_handle_route = (thunder_handle_route_t)dlsym(handle, "thunder_handle_route");
        thunder_dll.thunder_handle_process = (thunder_handle_process_t)dlsym(handle, "thunder_handle_process");
        thunder_dll.thunder_handle_fini = (thunder_handle_fini_t)dlsym(handle, "thunder_handle_fini");

        if (thunder_dll.thunder_handle_input == NULL)
        {
            fprintf(stderr, "thunder_handle_input not implemented\n");
            exit(EXIT_FAILURE);
        }

        if (thunder_dll.thunder_handle_process == NULL)
        {
            fprintf(stderr, "thunder_handle_process not implemented\n");
            exit(EXIT_FAILURE);
        }

        if (thunder_dll.thunder_handle_fini == NULL)
        {
            fprintf(stderr, "thunder_handle_fini not implemented\n");
            exit(EXIT_FAILURE);
        }

        thunder_dll.handle = handle;

        return 0;
    }
    else
    {
        fprintf(stderr, "cannot find thunder_handle_init in module\n");
        return -1;
    }
}

}  // namespace thunder
