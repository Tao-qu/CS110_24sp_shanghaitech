#include "../inc/fks_level1.h"
#include "../inc/hash_func.h"
#include <stdio.h>

static int get_list_len(list_node *node)
{
  // TODO
  int len = 0;
  while (node != NULL)
  {
    ++len;
    node = node->next;
  }
  return len;
}

fks_level1 *fks_level1_build(hash_chaining *hash_chaining_table)
{
  // TODO
  fks_level1 *table = malloc(sizeof(fks_level1));
  table->size = hash_chaining_table->size;
  table->level2_tables = malloc(table->size * sizeof(fks_level2 *));
  for (uint32_t i = 0; i < table->size; ++i)
  {
    int len = get_list_len(hash_chaining_table->slots[i]);
    len = len * len;
    if (len == 0)
    {
      table->level2_tables[i] = NULL;
      continue;
    }
    while (true)
    {
      hash_parameters parameters = generate_hash_parameters();
      fks_level2 *level2_table = fks_level2_build(hash_chaining_table->slots[i], len, parameters);
      if (level2_table != NULL)
      {
        table->level2_tables[i] = level2_table;
        break;
      }
    }
  }
  table->parameters = hash_chaining_table->parameters;
  return table;
}

bool fks_level1_search(fks_level1 *table, uint32_t key)
{
  // TODO
  uint32_t hash = hash_func(key, table->parameters, table->size);
  return fks_level2_search(table->level2_tables[hash], key);
}

void fks_level1_destroy(fks_level1 *table)
{
  // TODO
  for (uint32_t i = 0; i < table->size; ++i)
  {
    fks_level2_destroy(table->level2_tables[i]);
  }
  free(table->level2_tables);
  free(table);
}
