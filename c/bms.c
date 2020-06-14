#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "bms.h"
#define BM_ELEMS_MAX 256
int main(int argc, char **argv){
  Bm *bm=parse(argv[1]);
  printbm(bm);
  free(bm);
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
    printf("[%d]\n",bm->b);
  }
}
Bm *parse(char *str){
  Bm *bm=malloc(sizeof(Bm));
  bm->m=malloc(sizeof(Int)*BM_ELEMS_MAX);
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

