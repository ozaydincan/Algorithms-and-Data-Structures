#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 1001

typedef struct words {
  char *str;
  int freq;
  struct words *next;
} words;

char *lowercase(char *str) {
  char *lower = strdup(str);
  for (int i = 0; str[i] != '\0'; i++) {
    lower[i] = tolower((unsigned char)str[i]);
  }
  return lower;
}

words *create_node(char *str) {
  words *new = (words *)malloc(sizeof(words));
  if (new == NULL) {
    fprintf(stderr, "Malloc error creating new node!");
    return NULL;
  }
  new->str = strdup(str);
  if (new->str == NULL) {
    fprintf(stderr, "Memory error creating node str!");
    free(new);
    return NULL;
  }
  new->freq = 1;
  new->next = NULL;
  return new;
}

words *find_node(words *head, char *str) {
  while (head != NULL) {
    if (strcmp(head->str, str) == 0) {
      return head;
    }
    head = head->next;
  }
  return NULL;
}

words *tail_insert(words *head, char *str) {
  words *new = create_node(str);
  if (new == NULL)
    return head;

  if (head == NULL) {
    return new;
  }

  words *tail = head;
  while (tail->next != NULL) {
    tail = tail->next;
  }
  tail->next = new;
  return head;
}

words *get_words(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Input file not found!");
    return NULL;
  }

  words *head = NULL;
  char line[MAX_ROW];
  const char delim[] = " \n\r\t.,!?;:";

  while (fgets(line, MAX_ROW, fp)) {
    char *token = strtok(line, delim);
    while (token != NULL) {
      char *lower = lowercase(token);
      words *found = find_node(head, lower);
      if (found) {
        found->freq++;
      } else {
        head = tail_insert(head, lower);
      }
      free(lower);
      token = strtok(NULL, delim);
    }
  }
  fclose(fp);
  return head;
}

int free_words(words *head) {
  words *current = head;
  while (current != NULL) {
    words *temp = current;
    current = current->next;
    free(temp->str);
    temp->str = NULL;
    if (temp->str != NULL) {
      fprintf(stderr, "Error freeing string!");
      return -1;
    }
    free(temp);
    temp = NULL;
    if (temp != NULL) {
      fprintf(stderr, "Error freeing list node!");
      return -1;
    }
  }
  return 0;
}

int write_frequencies(const char *filename, words *head) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Output file could not be opened!");
    return -1;
  }
  words *current = head;
  while (current != NULL) {
    fprintf(fp, "%s: %d\n", current->str, current->freq);
    current = current->next;
  }
  fclose(fp);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "The program: %s, doesn't have <input_file> <output_file> entered!",
            argv[0]);
    return EXIT_FAILURE;
  }

  words *head = get_words(argv[1]);
  if (head == NULL) {
    return EXIT_FAILURE;
  }

  if (write_frequencies(argv[2], head) != 0) {
    free_words(head);
    return EXIT_FAILURE;
  }

  if (free_words(head) == -1) {
    fprintf(stderr, "Freeing error!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
