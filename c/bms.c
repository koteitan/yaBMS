#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "bms.h"
#define BM_ELEMS_MAX 256
int main(int argc, char **argv){
  Bm *bm=parse(argv[1]);
  Bm *bm1=expand(bm);
  printbm(bm1);
  printf("\n");
  free(bm);
  free(bm1);
  return EXIT_SUCCESS;
}
void printbm(Bm *bm){
  if(bm){
    int xs=bm->xs;
    int ys=bm->ys;
    if(xs!=0){
      for(int x=0;x<xs;x++){
        printf("(");
        printf("%d",bm->m[x*ys]);
        for(int y=1;y<ys;y++){
          printf(",%d",bm->m[x*ys+y]);
        }
        printf(")");
      }
    }
    printf("[%d]",bm->b);
  }
}
Bm *parse(char *str){
  Bm *bm=initbm();
  bm->xs=0;
  bm->ys=-1;
  int ys = 0;
  int *wp = &bm->m[0];
  char *c;
  //parse matrix
  for(c=&str[0];*c!='['&&*c!='\0';c++){
    switch(*c){
      case '(':
        ys=0;
      break;
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': 
        (*wp)*=10;
        (*wp)+=*c-'0';
      break;
      case ',':
        ys++;
        wp++;
        *wp=0;
      break;
      case ')':
        ys++;
        wp++;
        *wp=0;
        bm->xs++;
        if(bm->ys!=-1 && bm->ys!=ys){
          fprintf(stderr,"error:ys mismatch in %s\n",str);
          return NULL;
        }
        bm->ys=ys;
      break;
      default:
      break;
    }//switch
  }//for
  //parse bracket
  for(;*c!=']'&&*c!='\0';c++){
    switch(*c){
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': 
      bm->b*=10;
      bm->b+=*c-'0';
    }
  }
  return bm;
}
Bm* initbm(void){
  Bm *bm=malloc(sizeof(Bm));
  bm->m = malloc(sizeof(int)*BM_ELEMS_MAX);
  return bm;
}
Bm* expand(Bm* bm0){
  int x,y;
  Bm *bm1=initbm();
  int xs=bm0->xs;
  int ys=bm0->ys;
  int *m=bm0->m;
  int b =bm0->b;
  int xsm1ys = (xs-1)*ys;
  int *p=&m[xsm1ys];

  /* find lowermost non zero */
  for(y=0;y<ys;y++){
    if(*p++==0) break;
  }
  int lnz=y-1;

  /* simple cut case */
  if(y==0){ /* child=(0,...,0)  */
    memcpy(bm1->m, bm0->m, sizeof(int)*xsm1ys);
    bm1->xs=xs-1;
    bm1->ys=ys;
    bm1->b=b;
    return bm1;
  }
  /* make parent index matrix */
  int *pim=malloc(sizeof(int)*xs*ys); /* parent index matrix  */
  int *rp=&m[0];
  int *wp=&pim[0];
  for(x=0;x<xs;x++){

    /* the 1st row */
    int c=*rp++;
    int px;
    for(px=x-1;px>=0;px--){
      if(m[px*ys]<c) break;
    }
    *wp++=px;
      
    /* the 2nd or lower rows */
    for(y=1;y<ys;y++){
      c=*rp++;
      if(c==0){
        *wp++=-1;
        continue;
      }
      for(px=*(wp-1);px!=-1;px=pim[px*ys+y-1]){
        if(m[px*ys+y]<c) break;
      }
      *wp++=px;
    }/* for y */
  }/* for x */

  /* find bad root */
  int r=pim[(x-1)*ys+lnz]; /* bad root index */
  int bpxs = xs-r-1; /* number of columns of bad part */

  /* make delta */
  int *delta=malloc(sizeof(int)*lnz); /* delta[y] = ascension height of row y */
  for(y=0;y<lnz;y++){
    delta[y]=m[xsm1ys+y]-m[r*ys+y];
  }
  
  /* make ascension matrix */ 
  int *am=malloc(sizeof(int)*bpxs*lnz); /* am[x*lnz+y]=0:not ascend/1:ascend */
  memset(am,0,sizeof(int)*bpxs*lnz);
  wp=am;
  for(y=0;y<lnz;y++) *wp++=1;
  for(x=1;x<bpxs;x++){
    rp=&pim[(r+x)*ys];
    for(y=0;y<lnz;y++){
      if(*rp==-1){
        *wp++=0;
      }else{
        *wp++ = am[(*rp-r)*lnz+y];
      }
      rp++;
    }
  }

  /* copy good part+first bad part */
  memcpy(bm1->m,m,sizeof(int)*xsm1ys);
  
  /* copy new bad part */
  rp  =&bm1->m[ r    *ys];
  wp  =&bm1->m[xsm1ys];
  for(int a=0;a<b;a++){ /* copy b times */
    int *rpam=&am[0];
    for(x=0;x<bpxs;x++){
      int *pd=&delta[0];
      for(y=0;y<lnz;y++){
        *wp++=*rp++ + (*rpam++)*(*pd++);
      }
      for(y=lnz;y<ys;y++){
        *wp++=*rp++;
      }
    }
  }
  bm1->ys=ys;
  bm1->xs=xs-1+b*bpxs;
  bm1->b=b;
#if 1
  printbm(bm0);
  printf(")\nParent Index Matrix =");
  for(x=0;x<xs;x++){
    printf("(%d",pim[x*ys]);
    for(y=1;y<ys;y++) printf(",%d",pim[x*ys+y]);
    printf(")");
  }
  printf("\nbad root        = %d\n",r);
  printf("lnz             = %d\n",lnz);
  printf("delta           =(%d",delta[0]);
  for(y=1;y<lnz;y++)printf(",%d",delta[y]);
  printf(")\nAsension Matrix =");
  for(x=0;x<bpxs;x++){
    printf("(%d",am[x*lnz]);
    for(y=1;y<lnz;y++) printf(",%d",am[x*lnz+y]);
    for(y=lnz;y<ys;y++) printf(",0");
    printf(")");
  }
  printf("\n\n");
#endif
  if(am) free(am);
  if(pim) free(pim);
  if(delta) free(delta);
  return bm1;
}



