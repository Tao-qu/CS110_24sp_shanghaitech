#include "vector.h"
#include <stdlib.h>

Vector *vector_create(void)
{
  Vector *vector = malloc(sizeof(Vector));
  if (vector == NULL)
  {
    return NULL; // malloc failed
  }
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;
  vector->data = malloc(sizeof(double) * vector->capacity);
  if (vector->data == NULL)
  {
    free(vector);
    return NULL; // malloc failed
  }
  return vector;
}

void vector_push(Vector *vector, double element)
{
  if (vector == NULL)
  {
    return; // vector is NULL
  }
  if (vector->size == vector->capacity)
  {
    vector->capacity *= 2;
    double *temp = realloc(vector->data, sizeof(double) * vector->capacity);
    if (temp == NULL)
    {
      return; // realloc failed
    }
    vector->data = temp;
  }
  vector->data[vector->size++] = element;
}

double vector_get(const Vector *vector, int index)
{
  if (vector == NULL || index < 0 || index >= vector->size)
  {
    return 0; // vector is NULL
  }
  return vector->data[index];
}

void vector_free(Vector *vector)
{
  if (vector == NULL)
  {
    return; // vector is NULL
  }
  free(vector->data);
  free(vector);
}

void another_vector_free(Vector **vector)
{
  if (vector == NULL || *vector == NULL)
  {
    return; // vector is NULL
  }
  free((*vector)->data);
  free(*vector);
  *vector = NULL;
}
