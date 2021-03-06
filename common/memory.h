//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "types.h"

void memory_init(void);
void memory_malloc_secure(void **new_ptr, unsigned long size, char *info);
void *memory_malloc(unsigned long size, char *info);
void memory_free(void *ptr);
void memory_memoryUsage(FILE *fp);


#endif
