#include "cat.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

void output(FILE *file, struct CatInfo *cat_flags);

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  struct CatInfo cat_flags = {0, 0, 0, 0, 0, 0, 1, 1, 0};
  if (argc == 1) {
    CatNoArgs(argc);
  } else {
    search_flag(argc, argv, &cat_flags);
    for (int i = optind; i < argc; i++) {
      file = fopen(argv[i], "r");
      if (file) {
        cat_flags.file_count++;
        output(file, &cat_flags);
        fclose(file);
      } else {
        perror("Error ");
      }
    }
  }
  return 0;
}

void CatNoArgs(int fd) {
  char buf[4096];
  int bytes_read;
  bytes_read = read(fd, buf, 4096);
  while (bytes_read > 0) {
    printf("%.*s", bytes_read, buf);
    bytes_read = read(fd, buf, 4096);
  }
}

void search_flag(int argc, char **argv, struct CatInfo *cat_flags) {
  int rez = 0;
  int optindex = 0;
  const char *short_flags = "benstvET";
  const struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'n'},
                                      {"squeeze-blank", 0, NULL, 's'},
                                      {NULL, 0, NULL, 0}};
  while ((rez = getopt_long(argc, argv, short_flags, long_flags, &optindex)) !=
         -1) {
    switch (rez) {
      case 'b':
        cat_flags->b_flag = 1;
        break;
      case 'e':
        cat_flags->E_flag = 1;
        cat_flags->v_flag = 1;
        break;
      case 'n':
        cat_flags->n_flag = 1;
        break;
      case 's':
        cat_flags->s_flag = 1;
        break;
      case 't':
        cat_flags->T_flag = 1;
        cat_flags->v_flag = 1;
        break;
      case 'v':
        cat_flags->v_flag = 1;
        break;
      case 'E':
        cat_flags->E_flag = 1;
        break;
      case 'T':
        cat_flags->T_flag = 1;
        break;
    }
  }
}

void output(FILE *file, struct CatInfo *cat_flags) {
  int cur_char = -1;
  int prev_char = -1;
  int prev_prev_char = -1;
  while ((cur_char = fgetc(file)) != EOF) {
    // s_flag
    if (cur_char == '\n' && prev_char == '\n' &&
        (prev_prev_char == '\n' || prev_prev_char == -1) &&
        cat_flags->s_flag == 1) {
      if (cat_flags->file_count > 1 && cur_char == '\n' && prev_char == '\n' &&
          (prev_prev_char == '\n' || prev_prev_char == -1))
        cat_flags->file_count = 1;
      else
        continue;
    }
    // b_flag
    if (((cur_char != '\n') && (prev_char == -1 || prev_char == '\n')) &&
        cat_flags->b_flag == 1) {
      printf("%6d\t", cat_flags->count_b);
      cat_flags->count_b++;
    }
    // n_flag
    if ((prev_char == '\n' || prev_char == -1) && cat_flags->n_flag == 1 &&
        cat_flags->b_flag != 1) {
      if (cur_char == '\n' && prev_char == -1 && prev_prev_char == -1 &&
          cat_flags->file_count > 1)
        ;
      else {
        printf("%6d\t", cat_flags->count_n);
        cat_flags->count_n++;
      }
    }
    // T_flag
    if (cur_char == '\t' && cat_flags->T_flag == 1) {
      printf("^I");
    }
    // E_flag
    if (cur_char == '\n' && (cat_flags->E_flag == 1)) {
      printf("$");
    }
    // v_flag
    if (cur_char == '\n' || (cur_char == '\t' && cat_flags->T_flag != 1) ||
        (cat_flags->T_flag != 1 && cat_flags->E_flag != 1 &&
         cat_flags->v_flag != 1)) {
      printf("%c", cur_char);
    } else {
      if (cur_char < 32 && cur_char != 9) {
        printf("^%c", cur_char + 64);
      } else if (cur_char == 127) {
        printf("^?");
      } else if (cur_char > 127 && cur_char < 160) {
        printf("M-^%c", cur_char - 64);
      } else if (cur_char > 159 && cur_char < 255) {
        printf("M-%c", cur_char - 128);
      } else if (cur_char == 255) {
        printf("M-^?");
      } else if (cur_char != '\t') {
        printf("%c", cur_char);
      }
    }
    prev_prev_char = prev_char;
    prev_char = cur_char;
  }
}
