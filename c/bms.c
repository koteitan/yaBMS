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
  Int *wp = &bm->m[0];
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
  bm->m = malloc(sizeof(Int)*BM_ELEMS_MAX);
  return bm;
}
Bm* expand(Bm* bm0){
  int x,y;
  Bm *bm1=initbm();
  int xs=bm0->xs;
  int ys=bm0->ys;
  Int *m=bm0->m;
  Int b =bm0->b;
  int *p=&m[(xs-1)*ys];

  /* find lowermost non zero */
  for(y=0;y<ys;y++){
    if(*p++==0) break;
  }
  int lnz=y-1;

  /* simple cut case */
  if(y==0){ /* child=(0,...,0)  */
    memcpy(bm1->m, bm0->m, sizeof(Int)*(xs-1)*ys);
    bm1->xs=xs-1;
    bm1->ys=ys;
    bm1->b=b;
    return bm1;
  }
  /* make parent index matrix */
  int *pim=malloc(sizeof(int)*xs*ys); /* parent index matrix  */
  for(y=0;y<ys;y++){
    for(x=0;x<xs;x++){
      Int c=m[x*ys+y];
      if(c==0){
        pim[x*ys+y]=-1;
        continue;
      }
      int px; /* parent index */
      if(y==0){
        for(px=x-1;px>=0;px--){
          if(m[px*ys+y]<c) break;
        }
      }else{
        for(px=pim[x*ys+y-1];px!=-1;px=pim[px*ys+y-1]){
          if(m[px*ys+y]<c) break;
        }
      }
      pim[x*ys+y]=px;
    }/* for x */
  }/* for y */

  /* find bad root */
  int r=pim[(x-1)*ys+lnz]; /* bad root index */
  int bpxs = xs-r-1; /* number of columns of bad part */

  /* make delta */
  Int *delta=malloc(sizeof(Int)*lnz); /* delta[y] = ascension height of row y */
  for(y=0;y<lnz;y++){
    delta[y]=m[(xs-1)*ys+y]-m[r*ys+y];
  }
  
  /* make ascension matrix */ 
  Int *am=malloc(sizeof(Int)*bpxs*lnz); /* am[x*ys+y]=0:not ascend/1:ascend */
  memset(am,0,sizeof(Int)*bpxs*lnz);
  Int *wp=&am[0];
  for(y=0;y<lnz;y++) *wp++=1;
  for(x=1;x<bpxs;x++){
    for(y=0;y<lnz;y++){
      if(pim[(r+x)*ys+y]==-1){
        *wp++=0;
      }else{
        *wp++ = am[(pim[(r+x)*ys+y]-r)*lnz+y];
      }
    }
  }

  /* copy good part+first bad part */
  memcpy(bm1->m,m,sizeof(Int)*(xs-1)*ys);
  
  /* copy new bad part */
  Int *rp  =&bm1->m[ r    *ys];
       wp  =&bm1->m[(xs-1)*ys];
  for(int a=0;a<b;a++){ /* copy b times */
    Int *rpam=&am[0];
    for(x=0;x<bpxs;x++){
      Int *pd=&delta[0];
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



