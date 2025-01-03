
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vector {
  int capacity;
  int index;
  int *array;
  void (*append)(struct Vector *self, int item);
  void (*expand)(struct Vector *self);
  void (*sort)(struct Vector *self);
  int (*countSorted)(struct Vector *self, int needle);
} Vector;

int comp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void sort(Vector *self) {
  qsort(self->array, self->index, sizeof(self->array[0]), comp);
}

int countSorted(Vector *self, int needle) {
  int count = 0;

  for (int i = 0; i < self->index; i++) {
    int value = self->array[i];
    if (value > needle) {
      return count;
    }

    if (value == needle)
      count++;
  }

  return count;
}

void expand(Vector *self) {
  int newSize = self->capacity * 2;
  int *newArray = (int *)malloc(newSize * sizeof(int));

  if (!newArray) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  // initiate with known values
  memset(newArray, 0, newSize);

  // copy old array into the new one
  memcpy(newArray, self->array, self->capacity * sizeof(int));

  // release old array
  free(self->array);

  self->array = newArray;
  self->capacity = newSize;
}

void append(Vector *self, int item) {
  if (self->index >= self->capacity) {
    self->expand(self);
  }

  self->array[self->index++] = item;
  return;
}

Vector makeVector(int initialCapacity) {
  Vector vector;
  vector.capacity = initialCapacity;
  vector.index = 0;
  vector.array = (int *)malloc(vector.capacity * sizeof(int));

  if (!vector.array) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  // flush memory
  memset(vector.array, 0, vector.capacity * sizeof(int));

  vector.expand = expand;
  vector.append = append;
  vector.sort = sort;
  vector.countSorted = countSorted;

  return vector;
}

int *parse_data(FILE *file_ptr) {
  char buffer[8];

  int left[32];
  int right[32];

  return 0;
}

void print(Vector *vector) {
  printf("[ ");
  for (int i = 0; i < vector->index; i++) {
    printf("%d, ", vector->array[i]);
  }
  printf("]\n");
}

int main(void) {

  FILE *file_ptr;

  Vector numbers[2] = {makeVector(10), makeVector(10)};

  char ch;
  char buffer[8];

  file_ptr = fopen("/workspaces/advent-of-code-in-c-2024/src/input.txt", "r");

  if (file_ptr == NULL) {
    printf("File cannot be opened");
    return EXIT_FAILURE;
  }

  char bufferIndex = 0;
  char numberSide = 0;

  while (1) {
    ch = fgetc(file_ptr);

    if (ch == ' ' || ch == '\n' || ch == EOF) {
      if (bufferIndex == 0)
        continue;

      int value = atoi(buffer);

      bufferIndex = 0;
      memset(buffer, 0, sizeof(buffer));

      numbers[numberSide].append(&numbers[numberSide], value);

      if (numberSide > 0)
        numberSide = -1;
      numberSide++;

      if (ch == EOF) {
        break;
      }

      continue;
    }

    buffer[bufferIndex] = ch;
    bufferIndex++;
  }

  fclose(file_ptr);

  for (int i = 0; i < 2; i++) {
    numbers[i].sort(&numbers[i]);
  }

  // print(&left);
  // print(&right);

  int out = 0;

  for (int i = 0; i < numbers[0].index; i++) {
    out += abs(numbers[0].array[i] - numbers[1].array[i]);
  }

  printf("Part 1: %d\n\n", out);

  out = 0;
  for (int i = 0; i < numbers[0].index; i++) {
    int left_value = numbers[0].array[i];
    out += left_value * numbers[1].countSorted(&numbers[1], left_value);
  }

  printf("Part 2: %d\n\n", out);

  return 0;
}