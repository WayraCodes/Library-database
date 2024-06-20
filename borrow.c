#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void write_borrow(FILE *fp, char *name, book_t *book) {
  assert(fp != NULL);
  assert(book != NULL);

  fprintf(fp, "%s;%s;%s\n", name, book->title, book->author);
}


borrow_t *read_borrow(FILE *fp) {
  assert(fp != NULL);

  fseek(fp, 0, SEEK_SET);

  char name[MAX_LEN] = "";
  char title[MAX_LEN] = "";
  char author[MAX_LEN] = "";

  borrow_t *head = NULL;
  borrow_t *prev = NULL;

  while (fscanf(fp, "%249[^;];%249[^;];%249[^\n]\n", name, title, author) == 3) {
    borrow_t *current = malloc(sizeof(borrow_t));
    assert(current != NULL);
    strcpy(current->name, name);
    strcpy(current->title, title);
    strcpy(current->author, author);
    current->next = NULL;

    if (!head) {
      head = current;
    }
    else {
      prev->next = current;
    }

    prev = current;
  }

  return head;
}


void delete_borrow(char *file_name, borrow_t *book) {
  assert(file_name != NULL);
  assert(book != NULL);

  FILE *fp = fopen(file_name, "r+");
  assert(fp != NULL);
  FILE *second_fp = fopen("temp.txt", "w");
  assert(second_fp != NULL);

  fseek(fp, 0, SEEK_SET);
  fseek(second_fp, 0, SEEK_SET);

  int checker = 0;
  char name[MAX_LEN] = "";
  char title[MAX_LEN] = "";
  char author[MAX_LEN] = "";

  while ((fscanf(fp, "%249[^;];%249[^;];%249[^\n]\n", name, title, author) == 3)) {
    if ((strcmp(name, book->name) == 0) && (strcmp(title, book->title) == 0) && (strcmp(author, book->author) == 0)) {
      checker++;
      continue;
    }

    fprintf(second_fp, "%s;%s;%s\n", name, title, author);
  }

  fclose(fp);
  fclose(second_fp);

  if (checker != 0) {
    remove(file_name);
    rename("temp.txt", file_name);
  }
  else {
    remove("temp.txt");
  }
}



