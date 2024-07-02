#include "../inc/fks_level2.h"
#include "../inc/hash_func.h"
#include <string.h>

fks_level2 *fks_level2_init(uint32_t size, hash_parameters parameters)
{
  // TODO
  fks_level2 *table = malloc(sizeof(fks_level2));
  table->size = size;
  table->slots = malloc(size * sizeof(uint32_t *));
  for (uint32_t i = 0; i < size; ++i)
  {
    table->slots[i] = FKS_LEVEL2_EMPTY;
  }
  table->parameters = parameters;
  return table;
}

fks_level2 *fks_level2_build(list_node *head, uint32_t size,
                             hash_parameters parameters)
{
  // TODO
  fks_level2 *table = fks_level2_init(size, parameters);
  while (head != NULL)
  {
    uint32_t hash = hash_func(head->key, parameters, size);
    if (table->slots[hash] != FKS_LEVEL2_EMPTY && table->slots[hash] != head->key)
    {
      fks_level2_destroy(table);
      return NULL;
    }
    table->slots[hash] = head->key;
    head = head->next;
  }
  return table;
}

bool fks_level2_insert(fks_level2 *table, uint32_t key)
{
  // TODO
  uint32_t hash = hash_func(key, table->parameters, table->size);
  if (table->slots[hash] != FKS_LEVEL2_EMPTY)
  {
    return false;
  }
  table->slots[hash] = key;
  return true;
}

bool fks_level2_search(fks_level2 *table, uint32_t key)
{
  // TODO
  uint32_t hash = hash_func(key, table->parameters, table->size);
  if (table->slots[hash] == key)
  {
    return true;
  }
  return false;
}

void fks_level2_destroy(fks_level2 *table)
{
  // TODO
  if (table != NULL)
  {
    free(table->slots);
    free(table);
  }
  return;
}
