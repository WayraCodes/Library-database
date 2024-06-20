#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


book_t *create_book(char *book_title, char *author_name, int year, float rating) {
  assert(book_title != NULL);
  assert(author_name != NULL);

  if ((rating < 0.0) || (rating > MAX_RATING)) {
    printf("Touches\n");
    return NULL;
  }

  book_t *temp = malloc(sizeof(book_t));
  assert(temp != NULL);

  temp->title = malloc(strlen(book_title) + 1);
  assert(temp->title != NULL);
  strcpy(temp->title, book_title);

  temp->author = malloc(strlen(author_name) + 1);
  assert(temp->author != NULL);
  strcpy(temp->author, author_name);

  temp->year = year;
  temp->status = AVAILABLE;
  temp->rating = rating;
  temp->next = NULL;

  return temp;
} /* create_book() */



int add_book(book_t **head, book_t *new, FILE *fp) {
  assert(head != NULL);
  assert(new != NULL);
  assert(fp != NULL);

  book_t *prev = NULL;
  int check = 0;

  for (book_t *i = *head; i != NULL; i = i->next) {
    if (strcmp(new->title, i->title) < 0) {
      //printf("new: %s\n", new->title);
      //printf("old: %s\n", i->title);

      if (prev) {
        prev->next = new;
      }
      else {
        *head = new;
      }

      new->next = i;
      check++;
      break;
    }

    if (!i->next) {
      i->next = new;
      check++;
      break;
    }

    prev = i;
  }

  if (check == 0) {
    printf("ERROR!!!!\n");
    return ERROR;
  }

  write_books(fp, *head);

  return OK;
} /* add_book() */


int remove_book(book_t **head, book_t *book, FILE *fp) {
  assert(book != NULL);

  book_t *prev = NULL;

  for (book_t *i = *head; i != NULL; i = i->next) {
    if ((strcmp(book->title, i->title) == 0) && (strcmp(book->author, i->author) == 0) && (book->year == i->year)) {
      if (prev) {
        prev->next = i->next;
      }
      else {
        *head = i->next;
      }

      i->next = NULL;
      free(i->author);
      free(i->title);
      free(i);

      write_books(fp, *head);
      return OK;
    }

    prev = i;
  }

  return ERROR;
} /* remove_book() */


void view_details(book_t *head) {
  assert(head != NULL);

  char *status = (head->status == AVAILABLE) ? ("Available") : ("Borrowed");

  printf("Title: %s\n", head->title);
  printf("Author: %s\n", head->author);
  printf("Year: %d\n", head->year);
  printf("Rating: %.1f\n", head->rating);
  printf("Status: %s\n", status);
} /* view_details() */


void print_catalogue(book_t *head) {
  int counter = 0;

  for (book_t *i = head; i != NULL; i = i->next) {
    counter++;
    printf("%d: %s  –  %s\n", counter, i->title, i->author);
  }

  if (counter == 0) {
    printf("[There are no book available]\n");
  }

  printf("\nTotal books: %d\n", counter);
} /* print_catalogue() */




#if 0
// TODO: Delete this after testing
void recover(FILE *fp) {
  assert(fp != NULL);

  book_t *head = NULL;

  read_books(fp, &head);

  //printf("First title: %s\n", head->title);
  //printf("Holded data (year month rating): %d %d %f\n", head->year, head->month, head->rating);
  for (book_t *i = head; i != NULL; i = i->next) {
    printf("-> %s by %s\n", i->title, i->author);
    printf("---> (year month rating) %d %d %f\n", i->year, i->month, i->rating);
  }
}


int main() {
  char *file_name = "catalogue.txt";
  FILE *fp = fopen(file_name, "rb+");

  book_t *b1 = create_book("Harry Potter", "J. K. Rowling", 1999, 3, 9.5);
  book_t *b2 = create_book("The prince", "Machiavelli", 1513, 5, 7.0);
  book_t *b3 = create_book("Classroom of the elite", "Kinugasa", 2016, 4, 10);
  book_t *b4 = create_book("Antifragile", "Nassim Taleb", 2011, 3, 10);

  book_t *head = b1;

  add_book(&head, b2, fp);
  add_book(&head, b3, fp);
  add_book(&head, b4, fp);

  remove_book(&head, b1, fp);

  recover(fp);

  fclose(fp);
  return 0;
}

#endif









