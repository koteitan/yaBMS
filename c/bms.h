#pragma once
typedef struct{
  int xs; /* number of columns */
  int ys; /* number of rows */
  int *m; /* matrix body */
  int b;  /* bracket */
}Bm;/* Bashicu Matrix (BM) with bracket */

/** Bm* initbm(void)
 * @brief allocate memory for new BM and m and returns the pointer of BM 
 * @return the pointer for the new BM */
Bm* initbm(void);

/** Bm* parse(char *str)
 * @brief Allocate memory for new BM and parse string str into Bm.
 * @param str = string expression of BM. For example, str="(0,0,0)(1,1,1)[12]". 
 * @return BM represented in str. */
Bm* parse(char *str);

/** void printbm(Bm *bm)
 * @brief print string expression of bm to stdout. 
 * @param bm = BM to print */
void printbm(Bm *bm);

/** Bm* expand(Bm* bm0)
 * @brief Allocate memory for new BM, expand bm0 into new BM memory, and return its pointer.
 * @param bm0 = initial matrix.
 * @return the pointer for the expansion result.*/
Bm* expand(Bm* bm0);
