#pragma once

#ifdef __cplusplus
extern "C" {
#endif

PyObject *init_mmap_file_allocator();

void *tc_malloc(size_t size);
void tc_free(void *ptr);
void *tc_realloc(void *ptr, size_t size);
void *tc_calloc(size_t nmemb, size_t size);
void set_file_allocator_directory(const char *path);

#ifdef __cplusplus
}
#endif