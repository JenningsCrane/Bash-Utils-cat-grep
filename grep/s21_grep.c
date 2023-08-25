#define _GNU_SOURCE

#include "grep.h"

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

void GrepCount(FILE *file, char const *filename, regex_t *preg, int file_count,
               Flags flags);
void GrepFileInverse(FILE *file, Flags flags, regex_t *preg,
                     char const *filename, int file_count);
void GrepFile(FILE *file, Flags flags, regex_t *preg, char const *filename,
              int file_count);

int main(int argc, char *argv[]) {
  Flags flags = {0};
  search_flag(argc, argv, &flags);
  Grep(argc, argv, flags);
  return 0;
}

void search_flag(int argc, char **argv, Flags *flags) {
  int rez = 0;
  opterr = 0;
  while ((rez = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (rez) {
      case 'e':
        flags->e_flag = 1;
        break;
      case 'i':
        flags->i_flag |= REG_ICASE;
        break;
      case 'v':
        flags->v_flag = 1;
        break;
      case 'c':
        flags->c_flag = 1;
        break;
      case 'l':
        flags->l_flag = 1;
        break;
      case 'n':
        flags->n_flag = 1;
        break;
      case 'h':
        flags->h_flag = 1;
        break;
      case 's':
        flags->s_flag = 1;
        break;
      case 'f':
        flags->f_flag = 1;
        break;
      case 'o':
        flags->o_flag = 1;
        break;
    }
  }
}

void GrepCount(FILE *file, char const *filename, regex_t *preg, int file_count,
               Flags flags) {
  char *line = 0;
  size_t lenght = 0;
  regmatch_t match;
  int count_match = 0;
  int count_match_v = 0;
  while (getline(&line, &lenght, file) > 0) {
    if (flags.v_flag) {
      if (regexec(preg, line, 1, &match, 0)) ++count_match_v;
    } else {
      if (!regexec(preg, line, 1, &match, 0)) ++count_match;
    }
  }
  if (flags.c_flag) {
    if (file_count == 1 || flags.h_flag == 1) {
      if (flags.v_flag)
        printf("%d\n", count_match_v);
      else
        printf("%d\n", count_match);
    } else {
      if (flags.v_flag)
        printf("%s:%d\n", filename, count_match_v);
      else
        printf("%s:%d\n", filename, count_match);
    }
  }
  free(line);
}

void GrepFileInverse(FILE *file, Flags flags, regex_t *preg,
                     char const *filename, int file_count) {
  char *line = 0;
  size_t lenght = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &lenght, file) > 0) {
    count++;
    if (flags.o_flag)
      ;
    else {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.l_flag) {
          printf("%s\n", filename);
          break;
        } else {
          if (file_count == 1 || flags.h_flag == 1) {
            if (flags.n_flag) {
              printf("%d:%s", count, line);
              if (feof(file)) printf("\n");
            } else {
              printf("%s", line);
              if (feof(file)) printf("\n");
            }
          } else {
            if (flags.n_flag) {
              printf("%s:%d:%s", filename, count, line);
              if (feof(file)) printf("\n");
            } else {
              printf("%s:%s", filename, line);
              if (feof(file)) printf("\n");
            }
          }
        }
      }
    }
  }
  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg, char const *filename,
              int file_count) {
  char *line = 0;
  size_t lenght = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &lenght, file) > 0) {
    count++;
    if (!regexec(preg, line, 1, &match, 0)) {
      if (flags.l_flag) {
        printf("%s\n", filename);
        break;
      } else {
        if (flags.o_flag) {
          if (file_count == 1 || flags.h_flag == 1) {
            if (flags.n_flag)
              printf("%d:%.*s\n", count, (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
            else
              printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
          } else {
            if (flags.n_flag)
              printf("%s:%d:%.*s\n", filename, count,
                     (int)(match.rm_eo - match.rm_so), line + match.rm_so);
            else
              printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
          }
          char *remaining = line + match.rm_eo;
          while (!regexec(preg, remaining, 1, &match, 0)) {
            if (file_count == 1 || flags.h_flag == 1) {
              if (flags.n_flag)
                printf("%d:%.*s\n", count, (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              else
                printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
            } else {
              if (flags.n_flag)
                printf("%s:%d:%.*s\n", filename, count,
                       (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              else
                printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
            }
            remaining = remaining + match.rm_eo;
          }
        } else {
          if (file_count == 1 || flags.h_flag == 1) {
            if (flags.n_flag) {
              printf("%d:%s", count, line);
              if (feof(file)) printf("\n");
            } else {
              printf("%s", line);
              if (feof(file)) printf("\n");
            }
          } else {
            if (flags.n_flag) {
              printf("%s:%d:%s", filename, count, line);
              if (feof(file)) printf("\n");
            } else {
              printf("%s:%s", filename, line);
              if (feof(file)) printf("\n");
            }
          }
        }
      }
    }
  }
  free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  int error = 0;
  for (; pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  if (pattern == end) {
    perror("Error ");
    ++error;
  }
  if (!error) {
    int file_count = 0;
    if (regcomp(preg, *pattern, flags.i_flag)) perror("Error ");
    for (char **filename = pattern + 1; filename != end; ++filename) {
      if (**filename == '-') continue;
      ++file_count;
    }
    for (char **filename = pattern + 1; filename != end; ++filename) {
      if (**filename == '-') continue;
      FILE *file = fopen(*filename, "rb");
      if (file == NULL) {
        if (!flags.s_flag) {
          perror("Error ");
          continue;
        } else
          continue;
      }
      if (flags.c_flag && !flags.l_flag) {
        GrepCount(file, *filename, preg, file_count, flags);
      } else {
        if (flags.v_flag) {
          GrepFileInverse(file, flags, preg, *filename, file_count);
        } else {
          GrepFile(file, flags, preg, *filename, file_count);
        }
      }
      fclose(file);
    }
    regfree(preg);
  }
}
