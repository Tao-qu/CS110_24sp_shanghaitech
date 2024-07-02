#include "../inc/hash_chaining.h"
#include "../inc/hash_func.h"

hash_chaining *hash_chaining_init(uint32_t size)
{
  // TODO
  hash_chaining *table = malloc(sizeof(hash_chaining));
  table->size = size;
  table->slots = malloc(size * sizeof(list_node *));
  for (uint32_t i = 0; i < size; ++i)
  {
    table->slots[i] = NULL;
  }
  table->parameters = generate_hash_parameters();
  return table;
}

void hash_chaining_insert(hash_chaining *table, uint32_t key)
{
  // TODO
  uint32_t hash = hash_func(key, table->parameters, table->size);
  list_node *node = malloc(sizeof(list_node));
  node->key = key;
  node->next = table->slots[hash];
  table->slots[hash] = node;
  return;
}

bool hash_chaining_search(hash_chaining *table, uint32_t key)
{
  // TODO
  uint32_t hash = hash_func(key, table->parameters, table->size);
  list_node *node = table->slots[hash];
  while (node != NULL)
  {
    if (node->key == key)
    {
      return true;
    }
    node = node->next;
  }
  return false;
}

void hash_chaining_destroy(hash_chaining *table)
{
  // TODO
  for (uint32_t i = 0; i < table->size; ++i)
  {
    list_node *node = table->slots[i];
    while (node != NULL)
    {
      list_node *temp = node;
      node = node->next;
      free(temp);
    }
  }
  free(table->slots);
  free(table);
  return;
}
