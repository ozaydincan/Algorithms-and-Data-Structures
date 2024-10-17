#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 101

typedef struct string {
  char *str;
} string;

string *read_strings(const char *filename, int *count);
int write_cities(const char *filename, string *cities, int count);
void free_cities(string *cities, int count);
void sort_cities(string *cities, int count);

void swap(string *a, string *b) {
  string temp = *a;
  *a = *b;
  *b = temp;
}

int partition(string *cities, int low, int high) {
  char *pivot = cities[high].str;
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (strcmp(cities[j].str, pivot) < 0) {
      i++;
      swap(&cities[i], &cities[j]);
    }
  }
  swap(&cities[i + 1], &cities[high]);
  return i + 1;
}

void quicksort(string *cities, int low, int high) {
  if (low < high) {
    int pi = partition(cities, low, high);
    quicksort(cities, low, pi - 1);
    quicksort(cities, pi + 1, high);
  }
}

void sort_cities(string *cities, int count) { quicksort(cities, 0, count - 1); }

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Inusfficient command line arguments!");
    return EXIT_FAILURE;
  }

  int count = 0;

  string *cities = read_strings(argv[1], &count);
  sort_cities(cities, count);
  int flag = write_cities(argv[2], cities, count);
  if (flag == 1){
    perror("Writing to file");
  }
  free_cities(cities, count);

  return EXIT_SUCCESS;
}

string *read_strings(const char *filename, int *count) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file");
  }

  fscanf(fp, "%d", count);
  string *cities = (string *)malloc(sizeof(char *) * *count);

  if (!cities) {
    perror("Malloc error");
    fclose(fp);
    return NULL;
  }
  int i = 0;
  while (!feof(fp)) {
    char buff[MAX_STR_LEN];
    fscanf(fp, "%s", buff);
    cities[i].str = strdup(buff);
    i++;
  }

  fclose(fp);

  return cities;
}

int write_cities(const char *filename, string *cities, int count) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening output file!");
    return 1;
  }

  fprintf(fp, "%d\n", count);

  for (int i = 0; i < count; i++) {
    fprintf(fp, "%s\n", cities[i].str);
  }

  fclose(fp);
  return 0;
}

void free_cities(string *cities, int count) {
  for (int i = 0; i < count; i++) {
    free(cities[i].str);
  }

  free(cities);
}
