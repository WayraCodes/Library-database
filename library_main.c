#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int menu() {
  int input = 0;
  system("clear");
  printf("Library Database\n\n");
  printf("1. View catalogue\n");
  printf("2. Add book\n");
  printf("3. Remove book\n");
  printf("4. View borrowed books\n");
  printf("5. Borrow book\n");
  printf("6. Return book\n");
  printf("0. Exit\n\n");
  scanf("%d", &input);

  return input;
}


void add_input_book(book_t **head, FILE *fp) {
  assert(fp != NULL);

  system("clear");

  char title[MAX_LEN];
  char author[MAX_LEN];
  int year = 0;
  float rating = 0.0;

  printf("Adding a book\n");
  getchar();
  printf("\nEnter the title: ");
  scanf("%[^\n]s", title);
  getchar();

  printf("\nEnter the author name: ");
  scanf("%[^\n]s", author);
  getchar();

  printf("\nEnter the year of publication: ");
  scanf("%d", &year);

  printf("\nEnter the rating (out of %.1f) ", MAX_RATING);
  scanf("%f", &rating);

  book_t *new = create_book(title, author, year, rating);

  if (*head != NULL) {
    add_book(head, new, fp);
  }
  else {
    *head = new;
    write_books(fp, *head);
  }
}


book_t *get_book(book_t *head, int option) {
  book_t *current = head;

  for (int i = 0; i < option; i++) {
    if (i == option - 1) {
      break;
    }

    if (current->next != NULL) {
      current = current->next;
    }
    else {
      return NULL;
    }
  }

  return current;
}


void remove_input_book(book_t **head, FILE *fp) {
  system("clear");

  int option = 0;

  printf("Remove a book\n\n");

  print_catalogue(*head);

  printf("\nIndex of the book you want to remove (0 to cancel): ");
  scanf("%d", &option);

  if (option == 0) {
    return;
  }

  book_t *current = get_book(*head, option);

  if (!current) {
    return;
  }

  remove_book(head, current, fp);
}


void detail_input(book_t *head) {
  int input = 0;

  printf("\nEnter number for book details (0 to exit): ");

  scanf("%d", &input);

  if (input == 0) {
    return;
  }

  book_t *current_book = get_book(head, input);

  if (!current_book) {
    return;
  }

  system("clear");

  printf("Details for %s\n\n", current_book->title);

  view_details(current_book);
  printf("\nEnter 0 to exit: ");
  scanf("%d", &input);
}


void borrow_input(FILE *fp, FILE *book_fp, book_t *head) {
  assert(head != NULL);

  system("clear");

  printf("Borrow a book\n\n");
  print_catalogue(head);
  printf("\nIndex of the book you want to borrow (0 to cancel): \n");

  int option = 0;
  scanf("%d", &option);

  char name[MAX_LEN] = "";
  getchar();
  printf("\nEnter your name: \n");
  scanf("%[^\n]s", name);

  book_t *book = get_book(head, option);

  if (!book) {
    return;
  }

  if (book->status == BORROWED) {
    return;
  }
  else {
    book->status = BORROWED;
  }

  write_books(book_fp, head);
  write_borrow(fp, name, book);
}


void return_input(FILE *fp_a, char *borrow_file_name, book_t *book_head) {
  assert(fp_a != NULL);
  assert(borrow_file_name != NULL);
  assert(book_head != NULL);

  system("clear");

  printf("Book return\n\n");

  char name[MAX_LEN] = "";

  getchar();
  printf("Enter your name: \n");
  scanf("%[^\n]s", name);

  system("clear");
  printf("Book return\n\n");

  borrow_t *head = read_borrow(fp_a);

  int counter = 0;

  borrow_t *borrow_head = NULL;
  borrow_t *prev = NULL;
  borrow_t *selected = NULL;

  for (borrow_t *i = head; i != NULL; i = i->next) {
    if (strcmp(i->name, name) == 0) {
      counter++;
      printf("%d: %s  –  %s\n", counter, i->title, i->author);

      if (!borrow_head) {
        borrow_head = i;
        prev = i;
      }
      else {
        prev->next = i;
        prev = i;
      }
    }
  }


  printf("\nIndex of the book you want to return (0 to cancel): ");
  int returning_option = 0;
  scanf("%d", &returning_option);

  if (returning_option > counter) {
    return;
  }

  if (returning_option == 0) {
    return;
  }

  selected = borrow_head;


  for (int i = 0; i < returning_option; i++) {
    if (i == (returning_option - 1)) {
      break;
    }

    if (selected->next != NULL) {
      selected = selected->next;
    }
    else {
      return;
    }
  }

  delete_borrow(borrow_file_name, selected);

  for (book_t *i = book_head; i != NULL; i = i->next) {
    if ((strcmp(selected->title, i->title) == 0) && (strcmp(selected->author, i->author) == 0)) {
      i->status = AVAILABLE;
      break;
    }
  }
}


void view_borrow_input(FILE *fp) {
  assert(fp != NULL);

  system("clear");

  borrow_t *head = read_borrow(fp);
  if (head == NULL) {
    return;
  }

  printf("View your book rentals\n\n");

  char name[MAX_LEN] = "";

  getchar();
  printf("Enter your name: \n");
  scanf("%[^\n]s", name);

  system("clear");

  printf("View your book rentals\n\n");

  int counter = 0;

  for (borrow_t *i = head; i != NULL; i = i->next) {
    if (strcmp(name, i->name) == 0) {
      counter++;
      printf("%d: %s  —  %s\n", counter, i->title, i->author);
    }
  }

  int num = 0;
  printf("\nEnter 0 to exit: \n");
  scanf("%d", &num);
}


void synchronizer(FILE *fp_a, book_t *head) {
  assert(fp_a != NULL);

  borrow_t *borrow_head = read_borrow(fp_a);

  for (book_t *i = head; i != NULL; i = i->next) {
    int check = 0;

    for (borrow_t *j = borrow_head; j != NULL; j = j->next) {
      if (strcmp(i->title, j->title) == 0) {
        i->status = BORROWED;
        check++;
      }
    }

    if (check == 0) {
      i->status = AVAILABLE;
    }
  }
}


int main() {
  char *book_file = "catalogue.txt";
  FILE *book_fp = fopen(book_file, "rb+");

  char *borrow_file = "sample.txt";
  FILE *borrow_fp_a = fopen(borrow_file, "a+");

  book_t *book_head = NULL;

  read_books(book_fp, &book_head);

  synchronizer(borrow_fp_a, book_head);

  int menu_option = menu();

  for (;;) {
    switch (menu_option) {
      case 0:
        return 0;

      case 1:
        system("clear");
        printf("Catalogue\n\n");
        print_catalogue(book_head);
        detail_input(book_head);
        break;

      case 2:
        add_input_book(&book_head, book_fp);
        break;

      case 3:
        remove_input_book(&book_head, book_fp);
        break;

      case 5:
        borrow_input(borrow_fp_a, book_fp, book_head);
        break;

      case 6:
        return_input(borrow_fp_a, borrow_file, book_head);
        break;

      case 4:
        view_borrow_input(borrow_fp_a);
        break;

      default:
        break;
    }

    menu_option = menu();
  }
}
