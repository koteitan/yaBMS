#pragma once
typedef int Int;
typedef struct{
  int xs;
  int ys;
  Int *m;
  Int b;
}Bm;

Bm* initbm(void);
Bm* parse(char *str);
void printbm(Bm *bm);
Bm* expand(Bm* bm0);
