#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 100
#define TABLE_SIZE 100
#define MAX_WORD_SIZE 20

typedef struct HashMap {
  char *key;
  int value;
} HashMap;

// The methods or functions for the hash table

HashMap *word_map[TABLE_SIZE];

// djb2 hash function idk why we use 5381 or 33
unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  // The while loop checks for when c is assigned to the null char
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) +
           c; // same as hash*33+c but bit operations are faster when there
              // isn't compiler optimizations
  }
  return hash %
         TABLE_SIZE; // we are normalising the hash return for the table size
}

void init_hashmap() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    word_map[i] = NULL;
  }
}

int hashmap_insert(char *key) {
  if (key == NULL) {
    return -1;
  }

  unsigned long index = hash((unsigned char *)key);

  // Check if the key already exists or if we need to find a new spot
  for (int i = 0; i < TABLE_SIZE; i++) {
    unsigned long try = (index + i) % TABLE_SIZE;

    if (word_map[try] == NULL) {
      // Allocate memory for a new HashMap entry
      HashMap *new_pair = (HashMap *)malloc(sizeof(HashMap));
      new_pair->key = strdup(key); // Duplicate the key
      new_pair->value = 0;         // Set the value to 1

      word_map[try] = new_pair;
      return 0;
    } else if (strcmp(word_map[try]->key, key) == 0) {
      return 0;
    }
  }
  // Hash table is full
  printf("Error: Hashmap is full, cannot insert key '%s'\n", key);
  return -1;
}

int hashmap_increment(char *key) {
  if (key == NULL) {
    return -1;
  }

  unsigned long index = hash((unsigned char *)key);
  for (int i = 0; i < TABLE_SIZE; i++) {
    unsigned long try = (index + i) % TABLE_SIZE;
    if (word_map[try] != NULL && strcmp(word_map[try]->key, key) == 0) {
      word_map[try]->value++;
      return 0;
    }
    break;
  }
  // We can also have a print statement here for the value not being present or
  // we can call the inserts but for this exercise it isn't necessary
  return -1;
}

void print_freqs() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (word_map[i] != NULL) {
      printf("%s  =>  %d\n", word_map[i]->key, word_map[i]->value);
    }
  }
}

void free_map() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (word_map[i] != NULL) {
      free(word_map[i]->key);
      free(word_map[i]);
    }
  }
}

// Now the functions for the exercise
void process_word(char *word) {
  int i = 0;
  int j = 0;

  while (word[i] != '\0') {
    if (isalpha(word[i])) {
      word[j++] = tolower(word[i]);
    }
    i++;
  }
  word[j] = '\0';
  ;
}

void read_words(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Error opening words.txt!");
    exit(EXIT_FAILURE);
  }

  char word[MAX_WORD_SIZE];
  while (fscanf(fp, "%s\n", word) != EOF) {
    process_word(word);
    hashmap_insert(word);
  }
  fclose(fp);
}

void read_text(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Error opening text.txt");
    exit(EXIT_FAILURE);
  }

  char line[MAX_ROW];
  while (fgets(line, MAX_ROW, fp)) {
    char *word = strtok(line, " ");
    while (word) {
      process_word(word);
      if (strlen(word) > 0) {
        hashmap_increment(word);
      }
      word = strtok(NULL, " ");
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Insufficient command line arguments!");
    return EXIT_FAILURE;
  }

  clock_t start, end;
  double time_used;
  start = clock();

  init_hashmap();
  read_words(argv[1]);
  read_text(argv[2]);
  print_freqs();

  free_map();

  end =clock();;
  time_used = ((double) (end -start)) / CLOCKS_PER_SEC * 1000;
  printf("The runtime: %f ms\n", time_used);

  return 0;
}
