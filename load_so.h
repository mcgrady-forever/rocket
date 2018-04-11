#ifndef _LOAD_SO_H_
#define _LOAD_SO_H_

namespace rocket {

typedef int (* rocket_handle_init_t)(void*);
typedef int (* rocket_handle_input_t)(void*);
typedef int (* rocket_handle_route_t)(unsigned, void*, void*);
typedef int (* rocket_handle_process_t)(void*);
typedef void (* rocket_handle_fini_t)(void*);

typedef struct {
    void* handle;
    rocket_handle_init_t    rocket_handle_init;
    rocket_handle_input_t   rocket_handle_input;
    rocket_handle_route_t   rocket_handle_route;
    rocket_handle_process_t rocket_handle_process;
    rocket_handle_fini_t    rocket_handle_fini;

} rocket_dll_func_t;

extern rocket_dll_func_t rocket_dll;

int load_so(const char* file);

}  // namespace rocket

#endif
