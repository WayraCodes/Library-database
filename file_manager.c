#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int book_list_counter(book_t *head) {
  assert(head != NULL);

  int counter = 0;

  for (book_t *i = head; i != NULL; i = i->next) {
    counter++;
  }

  return counter;
}


void write_books(FILE *fp, book_t *head) {
  assert(fp != NULL);

  if (!head) {
    FILE *empty = fopen("catalogue.txt", "wb");
    fclose(empty);

    return;
  }

  fseek(fp, 0, SEEK_SET);
  int total_books = book_list_counter(head);

  fwrite(&total_books, sizeof(int), 1, fp);

  for (book_t *i = head; i != NULL; i = i->next) {
    printf("WRITE RUN\n");

    fwrite(&i->year, sizeof(int), 1, fp);
    fwrite(&i->status, sizeof(status_t), 1, fp);
    fwrite(&i->rating, sizeof(float), 1, fp);

    size_t title_len = strlen(i->title) + 1;
    size_t author_len = strlen(i->author) + 1;

    fwrite(&title_len, sizeof(size_t), 1, fp);
    fwrite(i->title, sizeof(char), title_len, fp);

    fwrite(&author_len, sizeof(size_t), 1, fp);
    fwrite(i->author, sizeof(char), author_len, fp);
  }
}


void read_books(FILE *fp, book_t **head) {
  assert(fp != NULL);
  assert(head != NULL);

  fseek(fp, 0, SEEK_SET);

  int total_books = 0;

  if (fread(&total_books, sizeof(int), 1, fp) != 1) {
    printf("PROBLEM\n");
  }

  book_t *prev = NULL;
  printf("COUNT: %d\n", total_books);

  for (int i = 0; i < total_books; i++) {
    book_t *current = malloc(sizeof(book_t));
    assert(current != NULL);

    if (i != 0) {
      prev->next = current;
    }
    else {
      *head = current;
    }

    fread(&current->year, sizeof(int), 1, fp);
    fread(&current->status, sizeof(status_t), 1, fp);
    fread(&current->rating, sizeof(float), 1, fp);

    size_t title_len = 0;
    size_t author_len = 0;

    fread(&title_len, sizeof(size_t), 1, fp);
    current->title = malloc(title_len);
    assert(current->title != NULL);
    fread(current->title, sizeof(char), title_len, fp);

    fread(&author_len, sizeof(size_t), 1, fp);
    current->author = malloc(author_len);
    assert(current->author != NULL);
    fread(current->author, sizeof(char), author_len, fp);

    current->next = NULL;

    prev = current;
  }
}
