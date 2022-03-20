#include "allocator.h"

typedef struct
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} Allocator;

NPY_NO_EXPORT void *
shift_alloc(Allocator *ctx, size_t sz)
{
    char *real = (char *)ctx->malloc(sz + 64);
    if (real == NULL)
    {
        return NULL;
    }
    snprintf(real, 64, "originally allocated %ld", (unsigned long)sz);
    printf("shift_alloc: %ld\n", (unsigned long)sz);
    return (void *)(real + 64);
}

NPY_NO_EXPORT void *
shift_zero(Allocator *ctx, size_t sz, size_t cnt)
{
    char *real = (char *)ctx->calloc(sz + 64, cnt);
    if (real == NULL)
    {
        return NULL;
    }
    snprintf(real, 64, "originally allocated %ld via zero",
             (unsigned long)sz);
    printf("shift_zero: %ld\n", (unsigned long)sz);
    return (void *)(real + 64);
}

NPY_NO_EXPORT void
shift_free(Allocator *ctx, void *p, npy_uintp sz)
{
    printf("shift_free: %ld\n", (unsigned long)sz);
    if (p == NULL)
    {
        return;
    }
    char *real = (char *)p - 64;
    if (strncmp(real, "originally allocated", 20) != 0)
    {
        fprintf(stdout, "uh-oh, unmatched shift_free, "
                        "no appropriate prefix\\n");
        /* Make C runtime crash by calling free on the wrong address */
        ctx->free((char *)p + 10);
        /* ctx->free(real); */
    }
    else
    {
        npy_uintp i = (npy_uintp)atoi(real + 20);
        if (i != sz)
        {
            fprintf(stderr, "uh-oh, unmatched shift_free"
                            "(ptr, %ld) but allocated %ld\\n",
                    sz, i);
            /* This happens when the shape has a 0, only print */
            ctx->free(real);
        }
        else
        {
            ctx->free(real);
        }
    }
}

NPY_NO_EXPORT void *
shift_realloc(Allocator *ctx, void *p, npy_uintp sz)
{
    printf("shift_realloc: %ld", (unsigned long)sz);
    if (p != NULL)
    {
        char *real = (char *)p - 64;
        if (strncmp(real, "originally allocated", 20) != 0)
        {
            fprintf(stdout, "uh-oh, unmatched shift_realloc\\n");
            return realloc(p, sz);
        }
        return (void *)((char *)ctx->realloc(real, sz + 64) + 64);
    }
    else
    {
        char *real = (char *)ctx->realloc(p, sz + 64);
        if (real == NULL)
        {
            return NULL;
        }
        snprintf(real, 64, "originally allocated "
                           "%ld  via realloc",
                 (unsigned long)sz);
        return (void *)(real + 64);
    }
}

static Allocator new_handler_ctx = {
    malloc,
    calloc,
    realloc,
    free};

static PyDataMem_Handler secret_data_handler = {
    "secret_data_allocator",
    1,
    {
        &new_handler_ctx,
        shift_alloc,   /* malloc */
        shift_zero,    /* calloc */
        shift_realloc, /* realloc */
        shift_free     /* free */
    }};

PyObject *set_secret_data_policy()
{
    import_array();
    PyObject *secret_data =
        PyCapsule_New(&secret_data_handler, "mem_handler", NULL);
    if (secret_data == NULL)
    {
        return NULL;
    }
    PyObject *old = PyDataMem_SetHandler(secret_data);
    Py_DECREF(secret_data);
    return old;
}