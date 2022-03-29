
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>

enum LogLevel
{
    Fatal = 0,
    Warn,
    Info,
    Debug
};
static enum LogLevel current_level = Fatal;

static void init_log_level()
{
    const char *env_p = getenv("FILE_ALLOCATOR_LOG_LEVEL");
    if (env_p != NULL)
    {
        if (strcasecmp(env_p, "Fatal") == 0)
        {
            current_level = Fatal;
        }
        else if (strcasecmp(env_p, "Warn") == 0)
        {
            current_level = Warn;
        }
        else if (strcasecmp(env_p, "Info") == 0)
        {
            current_level = Info;
        }
        else if (strcasecmp(env_p, "Debug") == 0)
        {
            current_level = Debug;
        }
    }
    else
    {
        current_level = Warn;
    }
}
#define LOG(level, fmt, ...)                                 \
    {                                                        \
        if (level <= current_level)                          \
        {                                                    \
            printf("[" #level "] " fmt "\n", ##__VA_ARGS__); \
        }                                                    \
    }

void *malloc_wrapper(void *ctx, size_t size)
{
    void *ret = tc_malloc(size);
    LOG(Debug, "tc_malloc size: %zd, ret: %p", size, ret);
    return ret;
}
void *calloc_wrapper(void *ctx, size_t nelem, size_t elsize)
{
    void *ret = tc_calloc(nelem, elsize);
    LOG(Debug, "tc_calloc nelem: %zd, elsize: %zd, ret: %p", nelem, elsize, ret);
    return ret;
}
void *realloc_wrapper(void *ctx, void *ptr, size_t new_size)
{
    void *ret = tc_realloc(ptr, new_size);
    LOG(Debug, "tc_realloc ptr: %p, new_size: %zd, ret: %p", ptr, new_size, ret);
    return ret;
}
void free_wrapper(void *ctx, void *ptr, size_t size)
{
    tc_free(ptr);
    LOG(Debug, "tc_free ptr: %p, size: %zd", ptr, size);
}

PyDataMem_Handler tcmalloc_handler = {
    "tcmalloc_mmap_file_allocator",
    1,
    {NULL,
     malloc_wrapper,
     calloc_wrapper,
     realloc_wrapper,
     free_wrapper}};

PyObject *init_mmap_file_allocator()
{
    import_array();
    init_log_level();
    PyObject *tcmalloc_handler_data =
        PyCapsule_New(&tcmalloc_handler, "mem_handler", NULL);
    if (tcmalloc_handler_data == NULL)
    {
        return NULL;
    }
    PyObject *old = PyDataMem_SetHandler(tcmalloc_handler_data);
    Py_DECREF(tcmalloc_handler_data);
    return old;
}