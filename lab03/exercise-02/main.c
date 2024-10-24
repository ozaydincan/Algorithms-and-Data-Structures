#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 51

typedef struct Employee {
  char *name;
  char id[17];
  char date[11];
  int salary;
  struct Employee *next;
  struct Employee *prev;
} Employee;

Employee *create_node() {
  Employee *new = (Employee *)malloc(sizeof(Employee));
  if (new == NULL) {
    fprintf(stderr, "Malloc error!");
    return NULL;
  }
  new->name = NULL;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

Employee *find_employee(const char *str, Employee *head) {
  Employee *current = head;
  while (current != NULL) {
    if (strcmp(str, current->name) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

Employee *read_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Input file error!");
    return NULL;
  }

  Employee *head = NULL;
  char name[MAX_NAME], date_buff[11], id[17];
  int salary;

  while (fscanf(fp, "%50s %16s %10s %d", name, id, date_buff, &salary) == 4) {
    Employee *new = create_node();
    new->name = strdup(name);
    strcpy(new->id, id);
    strcpy(new->date, date_buff);
    new->salary = salary;
    new->next = head;

    if (head != NULL) {
      head->prev = new;
    }

    head = new;
  }

  fclose(fp);
  return head;
}

void print_employee(Employee *current) {
  fprintf(stdout, "%s %s %s %d\n", current->name, current->id, current->date,
          current->salary);
}

void free_employee_list(Employee *head) {
  Employee *current = head;
  while (current != NULL) {
    Employee *next = current->next;
    free(current->name);
    free(current);
    current = next;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Insufficient CLA provided!\n");
    exit(EXIT_FAILURE);
  }

  char *commands = strdup(argv[3]);
  Employee *head = read_file(argv[1]);
  Employee *current_employee = find_employee(argv[2], head);
  if (current_employee == NULL) {
    fprintf(stderr, "Employee not found\n");
    free_employee_list(head);
    exit(1);
  }
  int i = 0;
  print_employee(current_employee);

  while (commands[i] != '\0') {
    if (commands[i] == '+' && current_employee->next != NULL) {
      current_employee = current_employee->next;
    } else if (commands[i] == '-' && current_employee->prev != NULL) {
      current_employee = current_employee->prev;
    }
    print_employee(current_employee);
    i++;
  }
  free_employee_list(head);

  return EXIT_SUCCESS;
}
