#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LEN 4
#define MAX_LEN 100

typedef struct Rectangle {
  float x_1;
  float y_1;
  float x_2;
  float y_2;
  char *id;
  float area;
  float perimeter;

} Rectangle;

void write_file(const char *filename, Rectangle *recs, int count);
Rectangle *fill_rectangle(const char *infile, int *count);
void initialize_recs(Rectangle *recs);
void quicksort_area(Rectangle *recs, int low, int high);
int partition_area(Rectangle *recs, int low, int high);
void quicksort_perimeter(Rectangle *recs, int low, int high);
int partition_perimeter(Rectangle *recs, int low, int high);
void swap(Rectangle *a, Rectangle *b);
void free_arr(Rectangle *recs, int count);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Wrong command line inputs!");
    return EXIT_FAILURE;
  }

  int count = 0;
  Rectangle *recs = fill_rectangle(argv[1], &count);
  if (!recs) {
    perror("Memory allocation for rectangles has failed!");
    return EXIT_FAILURE;
  }

  quicksort_area(recs, 0, count - 1);
  write_file(argv[2], recs, count);

  quicksort_perimeter(recs, 0, count - 1);
  write_file(argv[3], recs, count);

  free_arr(recs, count);

  return 0;
}

void initialize_recs(Rectangle *recs) {
  for (int i = 0; i < MAX_LEN; i++) {
    recs[i].id = (char *)malloc(sizeof(char) * (ID_LEN + 1));
  }
}

void print_recs(Rectangle *recs, int count) {
  for (int i = 0; i < count; i++) {
    printf("%s\nArea => %.2f\nPerimeter => %.2f\n", recs[i].id, recs[i].area,
           recs[i].perimeter);
  }
}

Rectangle *fill_rectangle(const char *infile, int *count) {
  FILE *fp = fopen(infile, "r");

  if (!fp) {
    perror("Couldn't open input file!");
    exit(EXIT_FAILURE);
  }

  Rectangle *recs = (Rectangle *)malloc(MAX_LEN * sizeof(Rectangle));
  if (!recs) {
    perror("Memory allocation failed!");
    fclose(fp);
    exit(EXIT_FAILURE);
  }
  initialize_recs(recs);
  float x, y;
  char id[ID_LEN + 1];
  *count = 0;
  while (fscanf(fp, "%4s %f %f", id, &x, &y) == 3) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
      if (strcmp(recs[i].id, id) == 0) {
        recs[i].x_2 = x;
        recs[i].y_2 = y;
        float side_x = (recs[i].x_1 - recs[i].x_2) > 0
                           ? (recs[i].x_1 - recs[i].x_2)
                           : -(recs[i].x_1 - recs[i].x_2);

        float side_y = (recs[i].y_1 - recs[i].y_2) > 0
                           ? (recs[i].y_1 - recs[i].y_2)
                           : -(recs[i].y_1 - recs[i].y_2);

        recs[i].area = side_y * side_x;
        recs[i].perimeter = 2 * (side_y + side_x);

        found = 1;
        break;
      }
    }
    if (found == 0 && *count < MAX_LEN) {
      recs[*count].id = strdup(id);
      recs[*count].x_1 = x;
      recs[*count].y_1 = y;
      (*count)++;
    }
  }

  fclose(fp);
  return recs;
}

void write_file(const char *filename, Rectangle *recs, int count) {
  FILE *f_out = fopen(filename, "w");
  if (!f_out) {
    perror("Error opening file to write perimeter!");
    exit(EXIT_FAILURE);
  }
  for (int i = count-1; i > 0; i--){
    fprintf(f_out, "%s\n", recs[i].id);
  }
  fclose(f_out);
}
void swap(Rectangle *a, Rectangle *b) {
  Rectangle temp = *a;
  *a = *b;
  *b = temp;
}
void quicksort_area(Rectangle *recs, int low, int high) {
  if (low < high) {
    int part = partition_area(recs, low, high);
    quicksort_area(recs, low, part - 1);
    quicksort_area(recs, part + 1, high);
  }
}

int partition_area(Rectangle *recs, int low, int high) {
  float pivot = recs[high].area;
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (recs[j].area <= pivot) {
      i++;
      swap(&recs[i], &recs[j]);
    }
  }

  swap(&recs[i + 1], &recs[high]);
  return i + 1;
}

void quicksort_perimeter(Rectangle *recs, int low, int high) {
  if (low < high) {
    int part = partition_perimeter(recs, low, high);
    quicksort_perimeter(recs, low, part - 1);
    quicksort_perimeter(recs, part + 1, high);
  }
}

int partition_perimeter(Rectangle *recs, int low, int high) {
  float pivot = recs[high].perimeter;
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (recs[j].perimeter <= pivot) {
      i++;
      swap(&recs[i], &recs[j]);
    }
  }

  swap(&recs[i + 1], &recs[high]);
  return i + 1;
}

void free_arr(Rectangle *recs, int count) {
  for (int i = 0; i < count; i++) {
    free(recs[i].id);
  }
  free(recs);
}
