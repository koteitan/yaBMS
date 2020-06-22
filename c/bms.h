#pragma once

#define BMS_ELEMS_MAX 256
#define BMS_BRACKETS_MAX 256
typedef struct{
  int xs; /** number of columns */
  int ys; /** number of rows */
  int m[BMS_ELEMS_MAX]; /** matrix body */
  int bs; /** number of brackets */
  int b[BMS_BRACKETS_MAX]; /** brackets */
}Bm;/* Bashicu Matrix (BM) with bracket */

#define VERSIONS (2)
typedef enum{
  eBMS_VER_4=0, /* BM4 */
  eBMS_VER_2=1, /* BM2 */
}eBMS_VER;

const char version_string[VERSIONS][16]={
  "BM4",
  "BM2"
};

/** @fn Bm* initbm(void)
 * @brief allocate memory for new BM and returns the pointer of BM.
 * @return the pointer for the new BM */
Bm* initbm(void);

/** @fn Bm* parse(char *str)
 * @brief Allocate memory for new BM and parse string str into Bm.
 * @param str = string expression of BM. For example, str="(0,0,0)(1,1,1)[12]". 
 * @return BM represented in str */
Bm* parse(char *str);

/** @fn void printbm(Bm *bm)
 * @brief print string expression of bm to stdout. 
 * @param bm = BM to print */
void printbm(Bm *bm);

/** @fn Bm* expand(Bm* bm0, eBMS_VER, int detail)
 * @brief Allocate memory for new BM, expand bm0 into new BM memory, and return its pointer.
 * @param bm0 = initial matrix.
 * @param ver = version of expansion.
 * @param detail != 0:show detail.
 * @return the pointer for the expansion result */
Bm* expand(Bm* bm0, eBMS_VER ver, int detail);

/** @fn int compmat(Bm* a, Bm *b)
 * @brief compare matrices of a and b and return which is larger. (brackets are ignored.)
 * @param a, b input bm
 * @return +1: if a>b
 *          0: if a=b
 *         -1: if a<b. */
int compmat(Bm* a, Bm* b);

/** @fn int isstd(Bm* b)
 * @brief check b is standard.
 * @param b input bm
 * @return +1: if b is standard.
 *          0: if b is not standard. */
int isstd(Bm *b, eBMS_VER ver, int detail);
