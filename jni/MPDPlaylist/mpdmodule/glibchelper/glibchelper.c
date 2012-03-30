/**  TBD: add STM Header
 */


#include "glibchelper.h"

gpointer
g_slice_copy (gsize block_size, gconstpointer mem_block)
{
  gpointer tmp = g_slice_alloc (block_size);
  memcpy (tmp, mem_block, block_size);
  return tmp;
}

gchar *
g_value_set_string (const gchar * v_string)
{
  gchar *new_string;

  new_string = g_slice_alloc (strlen (v_string));
  strcpy (new_string, v_string);
  return new_string;
}

void
g_value_free_string (gchar * v_string)
{
  if (v_string)
    g_free (v_string);

  return;
}
