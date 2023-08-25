#ifndef CAT_CAT_H_
#define CAT_CAT_H_

struct CatInfo {
  int b_flag;
  int n_flag;
  int s_flag;
  int v_flag;
  int E_flag;
  int T_flag;
  int count_n;
  int count_b;
  int file_count;
};

void CatNoArgs(int fd);
void search_flag(int argc, char **argv, struct CatInfo *cat_flags);

#endif  // CAT_CAT_H_
