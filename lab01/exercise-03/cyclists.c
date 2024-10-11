#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 30

typedef struct Cyclists {
  char *name;
  int id, lap_count;
  float *laps;
  float average_lap;
} Cyclists;

Cyclists *get_list(const char *filename, int *count, Cyclists *best);
void list_cyclists(Cyclists *arr, int count);
void show_details(Cyclists *arr, int count, char *name);
void free_cyclists(Cyclists* arr, int count);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Wrong command line inputs!");
    return EXIT_FAILURE;
  }

  int count = 0;
  Cyclists best = {NULL};
  Cyclists *cyclist_arr = get_list(argv[1], &count, &best);
  char cmd[256];

  while (1) {
    fprintf(stdout, "Command? ");
    scanf("%s", cmd);
    if (strcmp(cmd, "list") == 0) {
      list_cyclists(cyclist_arr, count);
    } else if (strcmp(cmd, "best") == 0) {
      printf("Name: %s #Id:%d #Laps:%d Times: ", best.name, best.id,
             best.lap_count);
      for (int i = 0; i < best.lap_count; i++) {
        printf("%.2f ", best.laps[i]);
      }
      printf("(Average: %.2f)\n", best.average_lap);
    } else if (strcmp(cmd, "details") == 0) {
      char name[MAX_NAME + 1];
      scanf("%s", name);
      show_details(cyclist_arr, count, name);
    } else if (strcmp(cmd, "stop") == 0) {
      fprintf(stdout, "Stopping program\n ...\n");
      break;
    } else {
      fprintf(stdout, "Unknown command: %s\n", cmd);
    }
  }
  return EXIT_SUCCESS;
}

Cyclists *get_list(const char *filename, int *count, Cyclists *best) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Error opening file!");
    exit(EXIT_FAILURE);
  }

  fscanf(fp, "%d", count);

  Cyclists *cyclists = (Cyclists *)malloc(sizeof(Cyclists) * *count);
  if (cyclists == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  int best_idx = 0;
  float best_avg = FLT_MAX;

  for (int i = 0; i < *count; i++) {
    char *name_buff = (char *)malloc(sizeof(char) * (MAX_NAME + 1));
    if (name_buff == NULL) {
      perror("Buffer allocation failed!");
      exit(EXIT_FAILURE);
    }
    int id_buff, lap_buff;

    fscanf(fp, "%s %d %d", name_buff, &id_buff, &lap_buff);
    cyclists[i].name = strdup(name_buff);
    cyclists[i].id = id_buff;
    cyclists[i].lap_count = lap_buff;
    cyclists[i].laps = (float *)malloc(sizeof(float) * lap_buff);
    float lap_sum = 0;
    for (int j = 0; j < lap_buff; j++) {
      float buff;
      fscanf(fp, "%f", &buff);
      cyclists[i].laps[j] = buff;
      lap_sum += buff;
    }
    cyclists[i].average_lap = lap_sum / lap_buff;
    if (best_avg > cyclists[i].average_lap) {
      printf("Prev best: %.2f\n", best_avg);
      best_avg = cyclists[i].average_lap;
      printf("found smaller avg: %.2f  Name: %s\n", best_avg,cyclists[i].name);
      best_idx = i;
    }
  }

  *best = cyclists[best_idx];

  fclose(fp);

  return cyclists;
}

void list_cyclists(Cyclists *arr, int count) {
  printf("Number of athletes: %d\n", count);
  for (int i = 0; i < count; i++) {
    printf("Name: %s #Id: %d #Laps: %d\n", arr[i].name, arr[i].id,
           arr[i].lap_count);
  }
}

void show_details(Cyclists *arr, int count, char *name) {
  int flag = 0;
  for (int i = 0; i < count; i++) {
    if (strcmp(arr[i].name, name) == 0) {
      flag = 1;
      printf("#Id: %d #Laps: %d Times: ", arr[i].id, arr[i].lap_count);
      for (int j = 0; j < arr[i].lap_count; j++) {
        printf("%.2f ", arr[i].laps[j]);
      }
      printf("Average time: %.2f\n", arr[i].average_lap);
    }
  }

  if (!flag) {
    printf("Name: %s not found in list\n", name);
  }
}

void free_cyclists(Cyclists* arr, int count){
  for(int i = 0; i < count; i++){
    free(arr[i].name);
    free(arr[i].laps);
  }
  free(arr);
  if (!arr){
    fprintf(stdout, "Success freeing array");
  }
}
