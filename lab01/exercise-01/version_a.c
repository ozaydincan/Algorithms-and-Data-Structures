#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 20

typedef struct {
    char word[MAX_WORD_LENGTH + 1]; 
    int occurrences;               
} index_t;

index_t *word_list; // Dynamic array to store words and frequencies
int word_count = 0; // Total number of words

void init_word_map(int count) {
    word_list = (index_t *)malloc(count * sizeof(index_t));
    if (word_list == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    word_count = count;
    for (int i = 0; i < word_count; i++) {
        word_list[i].occurrences = 0;
    }
}

void free_word_list() {
    free(word_list);
}

void process_word(char *word) {
    int i = 0, j = 0;
    while (word[i] != '\0') {
        if (isalpha(word[i])) {
            word[j++] = tolower(word[i]);
        }
        i++;
    }
    word[j] = '\0';
}

void read_words(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening word list file");
        exit(EXIT_FAILURE);
    }

    int count;
    fscanf(fp, "%d", &count);
    init_word_map(count);

    for (int i = 0; i < count; i++) {
        fscanf(fp, "%s", word_list[i].word);
        word_list[i].occurrences = 0; // Initialize occurrences to 0
    }

    fclose(fp);
}

void increment_word_count(char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word_list[i].word, word) == 0) {
            word_list[i].occurrences++;
            break;
        }
    }
}

void read_text(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening text file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_WORD_LENGTH];
    while (fscanf(fp, "%s", line) != EOF) {
        process_word(line);
        increment_word_count(line);
    }

    fclose(fp);
}

void print_freqs() {
    for (int i = 0; i < word_count; i++) {
        printf("%s => %d\n", word_list[i].word, word_list[i].occurrences);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <word file> <text file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    clock_t start, end;
    double time_used;
    start = clock();

    read_words(argv[1]);
    read_text(argv[2]);
    print_freqs();

    free_word_list();

    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("The runtime: %f ms\n", time_used);

    return 0;
}

