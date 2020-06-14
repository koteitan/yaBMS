#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "bms.h"
#define BMS_ELEMS_MAX 256
#define BMS_BRACKETS_MAX 256
static void printhelp(void);
int main(int argc, char **argv){
  eBMS_VER ver=eBMS_VER_4;
  char arg;
  int detail=0;
  while((arg=getopt(argc, argv, "v:hd")) != -1){
    switch(arg){
      case 'h':
        printhelp();
        return EXIT_SUCCESS;
      case 'd':
        detail = 1;
      break;
      case 'v':
        if(strcmp(optarg,"4")  ==0 ||
           strcmp(optarg,"4.0")==0 ||
           strcmp(optarg,"2,3")==0){
          ver = eBMS_VER_4;
        }else if(strcmp(optarg,"2"  )==0 ||
                 strcmp(optarg,"2.0")==0){
          ver = eBMS_VER_2;
        }
      break;
      default:
      break;
    }
  }
  if(optind!=argc-1){
    printf("error: input is invalid.\n\n");
    printhelp();
    return EXIT_FAILURE;
  }
  Bm *bm0=parse(argv[optind]);
  printf("%s",version_string[ver]);
  printbm(bm0);
  printf("\n");
  while(bm0->bs>0){
    Bm *bm1=expand(bm0,ver,detail);
    printf("%s",version_string[ver]);
    printbm(bm1);
    printf("\n");
    memcpy(bm0->m, bm1->m, sizeof(int)*bm1->xs*bm1->xs);
    memcpy(bm0->b, bm1->b, sizeof(int)*bm1->bs);
    bm0->bs=bm1->bs;
    bm0->xs=bm1->xs;
    bm0->ys=bm1->ys;
    free(bm1);
  }
  free(bm0);
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
    for(int b=0;b<bm->bs;b++){
      printf("[%d]",bm->b[b]);
    }
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

  //parse brackets
  wp=bm->b;
  bm->bs=0;
  for(int exit=0;!exit;c++){
    switch(*c){
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': 
        *wp*=10;
        *wp=*c-'0';
      break;
      case ']':
        wp++;
        *wp=0;
        bm->bs++;
      break;
      case '\0':
        exit=1; /* exit */
      break;
      default:
        /* ignore */
      break;
    }
  }
  return bm;
}
Bm* initbm(void){
  Bm *bm=malloc(sizeof(Bm));
  bm->m = malloc(sizeof(int)*BMS_ELEMS_MAX);
  bm->b = malloc(sizeof(int)*BMS_BRACKETS_MAX);
  return bm;
}
Bm* expand(Bm* bm0, eBMS_VER ver, int detail){
  Bm *bm1=initbm(); /* expand result */
  if(bm0->bs==0){ /* in the case of no brackets  */
    /* return identical */
    Bm *bm1=initbm();
    memcpy(bm1,bm0,sizeof(Bm));
    memcpy(bm1->m,bm0->m,sizeof(int)*bm0->xs+bm0->ys);
    memcpy(bm1->b,bm0->b,sizeof(int)*bm0->bs);
    return bm1;
  }
  /* brackets */
  int bs=bm0->bs;
  int b =bm0->b[0]; /* top bracket */
  bm1->bs=bs-1;
  memcpy(bm1->b, &(bm0->b[1]), sizeof(int)*(bs-1));
  
  int xs=bm0->xs;
  int ys=bm0->ys;
  int *m=bm0->m;
  int xsm1ys = (xs-1)*ys;
  int *p=&m[xsm1ys];
  int x,y;

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
  for(y=0;y<lnz;y++) *wp++=1; /* all bad root elements are ascent */
  switch(ver){
    case eBMS_VER_4:
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
    break;
    case eBMS_VER_2:
      for(x=1;x<bpxs;x++){
        for(y=0;y<lnz;y++){
          int pi = pim[(r+x)*ys+0]; /* BM2 always see first row(0) */
          *wp++ = pi>=0 && am[(pi-r)*lnz+y] && m[(r+x)*ys+y]>m[r*ys+y];
        }
      }
    break;
    default:
    break;
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
  if(detail){
    printf("\nInput               =");
    printbm(bm0);
    printf("\nParent Index Matrix =");
    for(x=0;x<xs;x++){
      printf("(%d",pim[x*ys]);
      for(y=1;y<ys;y++) printf(",%d",pim[x*ys+y]);
      printf(")");
    }
    printf("\nbad root            = %d\n",r);
    printf("lnz                 = %d\n",lnz);
    printf("delta               =(%d",delta[0]);
    for(y=1;y<lnz;y++)printf(",%d",delta[y]);
    printf(")\nAsension Matrix     =");
    for(x=0;x<bpxs;x++){
      printf("(%d",am[x*lnz]);
      for(y=1;y<lnz;y++) printf(",%d",am[x*lnz+y]);
      for(y=lnz;y<ys;y++) printf(",0");
      printf(")");
    }
    printf("\n\n");
  }
  if(am) free(am);
  if(pim) free(pim);
  if(delta) free(delta);
  return bm1;
}
static void printhelp(void){
  printf("usage  : bms [-v ver] [-h] [-d] <bm>\n"
         " expand bashicu matrix bm in version ver.\n"
         "example: bms -v 4 \"(0,0,0)(1,1,1)(2,0,0)(1,1,1)[12]\"\n"
         "param  : bm  = bashicu matrix with bracket to expand\n"
         "options:\n"
         "-v ver : expand with version ver.\n"
         "         ver = {4, 2} (default = 2)\n"
         "-d     : show detail output\n"
         "notes  : activate function is f(x)=x.\n");
}
