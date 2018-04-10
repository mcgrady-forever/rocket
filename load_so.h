#ifndef _LOAD_SO_H_
#define _LOAD_SO_H_

namespace thunder {

typedef int (* thunder_handle_init_t)(void*);
typedef int (* thunder_handle_input_t)(void*);
typedef int (* thunder_handle_route_t)(unsigned, void*, void*);
typedef int (* thunder_handle_process_t)(void*);
typedef void (* thunder_handle_fini_t)(void*);

typedef struct {
    void* handle;
    thunder_handle_init_t    thunder_handle_init;
    thunder_handle_input_t   thunder_handle_input;
    thunder_handle_route_t   thunder_handle_route;
    thunder_handle_process_t thunder_handle_process;
    thunder_handle_fini_t    thunder_handle_fini;

} thunder_dll_func_t;

extern thunder_dll_func_t thunder_dll;

int load_so(const char* file);

}  // namespace thunder

#endif
