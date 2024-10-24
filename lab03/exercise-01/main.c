#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 256

typedef struct word {
  char *str;
  int freq;
  struct word *next;
} word;

word *create_node(char *str) {
  word *new_word = (word *)malloc(sizeof(word));
  if (new_word == NULL) {
    fprintf(stderr, "Malloc error creating new list node!");
    exit(EXIT_FAILURE);
  }

  new_word->str = strdup(str);
  if (new_word->str == NULL) {
    fprintf(stderr, "Memory allocation error for node area str!");
    exit(EXIT_FAILURE);
  }

  new_word->freq = 1;
  new_word->next = NULL;

  return new_word;
}

word *insert_head(word *head, char *str) {
  word *new_head = create_node(str);
  new_head->next = head;
  return new_head;
}

word *find_word(word *head, char *str) {
  word *current = head;
  while (current != NULL) {
    if (strcmp(str, current->str) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

char *lowercase(char *str) {
  char *lower_str = strdup(str);
  int i = 0;
  while (str[i] != '\0') {
    lower_str[i] = tolower((unsigned char)str[i]);
    i++;
  }
  return lower_str;
}

// File I/O  and sorting stuff

word *read_word(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Input file error!");
    return NULL;
  }
  word *head = NULL;
  char buff[BUFFER_LEN];
  const char delims[] = " \n\t\r.,!?";
  while (fgets(buff, BUFFER_LEN, fp)) {
    char *tok = strtok(buff, delims);
    while (tok != NULL) {
      char *word_buff = lowercase(tok);
      word *exists = find_word(head, word_buff);

      if (exists == NULL) {
        head = insert_head(head, word_buff);
      } else {
        exists->freq++;
      }
      free(word_buff);
      tok = strtok(NULL, delims);
    }
  }
  fclose(fp);
  return head;
}

int free_list(word *head) {
  word *current = head;
  while (current != NULL) {
    word *temp = current;
    current = current->next;
    free(temp->str);
    free(temp);
    if (temp != NULL && temp->str != NULL && temp->next != NULL) {
      return 1;
    }
  }
  return 0;
}

int write_word(word *head, const char *filename) {
  FILE *f_out = fopen(filename, "w");
  if (f_out == NULL) {
    perror("Error opening output file!");
    return EXIT_FAILURE;
  }

  word *curr = head;
  while (curr) {
    fprintf(f_out, "%s: %d\n", curr->str, curr->freq);
    curr = curr->next;
  }
  fclose(f_out);
  free_list(head);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Insufficient command line arguments!");
    return EXIT_FAILURE;
  }
  
  word* head = read_word(argv[1]);
  if(write_word(head, argv[2]) == 1){
    fprintf(stdout, "Error writing to file!");
    return EXIT_FAILURE;
  }
    

  return EXIT_SUCCESS;
}
