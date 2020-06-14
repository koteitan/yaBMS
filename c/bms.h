#pragma once
typedef int Int;
typedef struct{
  int xs;
  int ys;
  Int *m;
  Int b;
}Bm;

int parse(Bm *bm, char *str);
void printbm(Bm *bm);
