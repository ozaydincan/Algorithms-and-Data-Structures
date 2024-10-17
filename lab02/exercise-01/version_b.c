#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 101

char **read_cities(const char *filename, int *count);
int write_cities(const char *filename, int count, char **cities);
void free_cities(char **cities, int count);
void sort_cities(char **cities, int count);

void swap(char **a, char **b) {
  char *temp = *a;
  *a = *b;
  *b = temp;
}

int partition(char **cities, int low, int high) {
  char *pivot = cities[high];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (strcmp(cities[j], cities[high]) < 0) {
      i++;
      swap(&cities[i], &cities[j]);
    }
  }
  swap(&cities[i + 1], &cities[high]);
  return (i + 1);
}

void quicksort(char **cities, int low, int high) {
  if (low < high) {
    int pi = partition(cities, low, high);
    quicksort(cities, low, pi - 1);
    quicksort(cities, pi + 1, high);
  }
}

void sort_alphabetically(char **cities, int count) {
  quicksort(cities, 0, count - 1);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Insufficient command line arguments!\n");
    return EXIT_FAILURE;
  }

  int count = 0;
  char **cities = read_cities(argv[1], &count);

  sort_alphabetically(cities, count);

  int flag = write_cities(argv[2], count, cities);

  if (flag == 1) {
    fprintf(stderr, "Error writing to file!");
    free_cities(cities, count);
    return EXIT_FAILURE;
  }

  free_cities(cities, count);

  return EXIT_SUCCESS;
}

char** read_cities(const char* filename, int *count){
  FILE *fp = fopen(filename, "r");
  if(!fp){
    perror("Error opening input file");
    return NULL;
  }

  fscanf(fp, "%d", count);

  char **cities = (char**) malloc(sizeof(char* ) * (*count));
  if(cities == NULL){
    perror("Malloc error");
    fclose(fp);
    return NULL;
  }
  for (int i = 0 ; i < *count; i++){
    char buff[MAX_STR_LEN];
    fscanf(fp, "%s", buff);
    cities[i] = strdup(buff);
    if (cities[i] == NULL){
      perror("Memory allocation error for the string!");
      free_cities(cities, i);
      fclose(fp);
      return NULL;
    }
  }

  fclose(fp);
  return cities;
}


int write_cities(const char *filename, int count, char **cities){
  FILE *fp = fopen(filename, "w");
  if(fp == NULL){
    perror("Error opening output file");
    return 1;
  }
  fprintf(fp, "%d\n", count);
  for(int i = 0; i < count; i++){
    fprintf(fp, "%s\n", cities[i]);
  }

  fclose(fp);
  return 0;
}

void free_cities(char **cities, int count){
  for(int i = 0; i < count; i++){
    free(cities[i]);
  }
  free(cities);
}
