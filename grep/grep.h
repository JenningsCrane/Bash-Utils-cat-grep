#ifndef GREP_GREP_H_
#define GREP_GREP_H_

typedef struct {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
} Flags;

void search_flag(int argc, char **argv, Flags *flags);
void Grep(int argc, char *argv[], Flags flags);

#endif  // GREP_GREP_H_
