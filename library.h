#ifndef LIBRARY_HEADER
#define LIBRARY_HEADER

#include <stdio.h>

#define MAX_RATING (10.0)
#define MAX_LEN    (250)

#define OK         (0)
#define ERROR      (-1)

typedef enum {
  AVAILABLE,
  BORROWED,
} status_t;

typedef struct book {
  int year;
  float rating;

  status_t status;

  char *title;
  char *author;

  struct book *next;
} book_t;

typedef struct borrow {
  char name[MAX_LEN];
  char title[MAX_LEN];
  char author[MAX_LEN];

  struct borrow *next;
} borrow_t;


book_t *create_book(char *, char *, int, float);
borrow_t *read_borrow(FILE *);
int add_book(book_t **, book_t *, FILE *);
int remove_book(book_t **, book_t *, FILE *);
int book_list_counter(book_t *);
void write_books(FILE *, book_t *);
void read_books(FILE *, book_t **);
void print_catalogue(book_t *);
void view_details(book_t *);
void write_borrow(FILE *, char *, book_t *);
void delete_borrow(char *, borrow_t *);



#endif
