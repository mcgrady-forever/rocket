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

namespace rocket {

#define PROCTECT_AREA_SIZE (512*1024)

rocket_dll_func_t rocket_dll = { NULL };

int load_so(const char* file)
{
    if (rocket_dll.handle != NULL)
    {
        dlclose(rocket_dll.handle);
    }

    memset(&rocket_dll, 0x0, sizeof(rocket_dll_func_t));

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

    void* func = dlsym(handle, "rocket_handle_init");

    if (func != NULL)
    {
        rocket_dll.rocket_handle_init = (rocket_handle_init_t)dlsym(handle, "rocket_handle_init");
        rocket_dll.rocket_handle_input = (rocket_handle_input_t)dlsym(handle, "rocket_handle_input");
        rocket_dll.rocket_handle_route = (rocket_handle_route_t)dlsym(handle, "rocket_handle_route");
        rocket_dll.rocket_handle_process = (rocket_handle_process_t)dlsym(handle, "rocket_handle_process");
        rocket_dll.rocket_handle_fini = (rocket_handle_fini_t)dlsym(handle, "rocket_handle_fini");

        if (rocket_dll.rocket_handle_input == NULL)
        {
            fprintf(stderr, "rocket_handle_input not implemented\n");
            exit(EXIT_FAILURE);
        }

        if (rocket_dll.rocket_handle_process == NULL)
        {
            fprintf(stderr, "rocket_handle_process not implemented\n");
            exit(EXIT_FAILURE);
        }

        if (rocket_dll.rocket_handle_fini == NULL)
        {
            fprintf(stderr, "rocket_handle_fini not implemented\n");
            exit(EXIT_FAILURE);
        }

        rocket_dll.handle = handle;

        return 0;
    }
    else
    {
        fprintf(stderr, "cannot find rocket_handle_init in module\n");
        return -1;
    }
}

}  // namespace rocket
