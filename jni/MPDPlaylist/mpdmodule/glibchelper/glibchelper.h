/**  TBD: add STM Header
 */
#ifndef __GLIBCHELPER_H__
#define __GLIBCHELPER_H__

#include "gtypes.h"
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


gpointer g_slice_copy (gsize block_size, gconstpointer mem_block);
gchar *g_value_set_string (const gchar * v_string);
void g_value_free_string (gchar * v_string);

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define g_strdup strdup
#define g_slice_alloc(size)    	((gpointer) malloc(size))
#define g_slice_alloc0(size)    ((gpointer) malloc(size))
#define  g_slice_new(type)      ((type*) g_slice_alloc (sizeof (type)))
#define  g_slice_new0(type)     ((type*) g_slice_alloc0 (sizeof (type)))
#define g_slice_free1(size, ptr) 	free(ptr)
#define g_return_val_if_fail(expr,val)	do{(void *) 0;}while(0)
#define g_free free

/* MemoryBlockType *
 *       g_slice_dup                    (MemoryBlockType,
 *	                                 MemoryBlockType *mem_block);
 *       g_slice_free                   (MemoryBlockType,
 *	                                 MemoryBlockType *mem_block);
 *       g_slice_free_chain             (MemoryBlockType,
 *                                       MemoryBlockType *first_chain_block,
 *                                       memory_block_next_field);
 * pseudo prototypes for the macro
 * definitions following below.
 */

/* we go through extra hoops to ensure type safety */
#define g_slice_dup(type, mem)                                  \
  (1 ? (type*) g_slice_copy (sizeof (type), (mem))              \
     : ((void) ((type*) 0 == (mem)), (type*) 0))
#define g_slice_free(type, mem)				do {	\
  if (1) g_slice_free1 (sizeof (type), (mem));			\
  else   (void) ((type*) 0 == (mem)); 				\
} while (0)

#if 0
#define g_slice_free_chain(type, mem_chain, next)	do {	\
  if (1) g_slice_free_chain_with_offset (sizeof (type),		\
                 (mem_chain), G_STRUCT_OFFSET (type, next)); 	\
  else   (void) ((type*) 0 == (mem_chain));			\
} while (0)

#endif


#ifdef __cplusplus
}
#endif

#endif /* __GLIBCHELPER_H__ */

