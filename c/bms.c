#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "bms.h"
static void printhelp(void);
int main(int argc, char **argv){
  eBMS_VER ver=eBMS_VER_4;
  int flagstd=0;
  int flagloop=0;
  int flagexp=0;
  int flagrec=0;
  int detail=0;
  /* check option */
  char arg;
  while((arg=getopt(argc, argv, "v:hdeslcr")) != -1){
    switch(arg){
      case 'h':
        printhelp();
        return EXIT_SUCCESS;
      case 'd': detail   = 1; break;
      case 'l': flagloop = 1; break;
      case 'e': flagexp  = 1; break;
      case 'r': flagrec  = 1; break;
      case 's': flagstd  = 1; break;
      case 'v':
        if(
          strcmp(optarg,"4")  ==0 ||
          strcmp(optarg,"4.0")==0 ||
          strcmp(optarg,"2.3")==0 ||
          strcmp(optarg,"BM4")  ==0 ||
          strcmp(optarg,"BM4.0")==0 ||
          strcmp(optarg,"BM2.3")==0 ||
          0){
          ver = eBMS_VER_4;
        }else if(
          strcmp(optarg,"BM2"  )==0 ||
          strcmp(optarg,"BM2.0")==0 ||
          strcmp(optarg,"2"  )==0 ||
          strcmp(optarg,"2.0")==0 ||
          0){
          ver = eBMS_VER_2;
        }else if(
          strcmp(optarg,"BM1.1")==0 ||
          strcmp(optarg,"1.1"  )==0 ||
          0){
          ver = eBMS_VER_1d1;
        }
      break;
      default:
      break;
    }
  }
  if(argc<optind+1){
    printf("error: input is not enough.\n");
    return EXIT_FAILURE;
  }
  if(!flagloop&&!flagexp&&!flagstd)flagexp=1;
  Bm *bm0=parse(argv[optind]);

  /* check std */
  if(flagstd){
    int std=isstd(bm0,ver,detail);
    if(detail){
      printf(std?"standard.\n":"non-standard.\n");
    }else{
      printf(std?"1\n":"0\n");
    }
  }

  /* expand */
  if(flagexp){
    while(bm0->bs>0){
      Bm *bm1=expand(bm0,ver,detail);
      printbm(bm1);
      printf("\n");
      if(!flagrec)break;
      memcpy(&bm0->m[0], &bm1->m[0], sizeof(int)*bm1->xs*bm1->xs);
      memcpy(&bm0->b[0], &bm1->b[0], sizeof(int)*bm1->bs);
      bm0->bs=bm1->bs;
      bm0->xs=bm1->xs;
      bm0->ys=bm1->ys;
      if(bm1)free(bm1);
    }
  }

  /* check loop */
  if(flagloop){
    if(flagrec){
      if(argc!=optind+2){
        printf("error: input is not enough.\n");
        printf("usage: ./bms -lr bms0 bms1\n");
        return EXIT_FAILURE;
      }
      Bm *bm1=parse(argv[optind+1]);
      char *str=bm2str(bm1);
      int ret=checklooprec(bm0, bm1, 3, str, ver, detail);
      if(str)free(str);
      if(detail){
        printf("%s",ret?"loop!\n":"No loops found.\n");
      }else{
        printf("%s",ret?"1":"0");
      }
      if(bm1)free(bm1);
    }else{
      if(detail)printf("version : %s\n",version_string[ver]);
      while(bm0->bs>0){
        if(detail){
          printf("original: ");printbm(bm0);printf("\n");
        }
        Bm *bm1=expand(bm0,ver,0);
        memcpy(&bm0->m[0], &bm1->m[0], sizeof(int)*bm1->xs*bm1->xs);
        memcpy(&bm0->b[0], &bm1->b[0], sizeof(int)*bm1->bs);
        bm0->bs=bm1->bs;
        bm0->xs=bm1->xs;
        bm0->ys=bm1->ys;
        if(bm1)free(bm1);
      }
      int isloop=checkloop(bm0,ver,detail);
      if(!detail){
        printf(isloop?"1\n":"0\n");
      }
    }
  }

  if(bm0)free(bm0);
  return EXIT_SUCCESS;
}
char *bm2str(Bm *bm){
  char *str=malloc(1024);
  str[0]='\0';
  if(bm){
    int xs=bm->xs;
    int ys=bm->ys;
    if(xs!=0){
      for(int x=0;x<xs;x++){
        sprintf(str,"%s(",str);
        sprintf(str,"%s%d",str,bm->m[x*ys]);
        for(int y=1;y<ys;y++){
          sprintf(str,"%s,%d",str,bm->m[x*ys+y]);
        }
        sprintf(str,"%s)",str);
      }
    }
    for(int b=0;b<bm->bs;b++){
      sprintf(str,"%s[%d]",str,bm->b[b]);
    }
  }
  return str;
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
Bm* initbm(void){
  Bm *bm=malloc(sizeof(Bm));
  return bm;
}
Bm* clone(Bm *bm0){
  Bm *bm1=initbm();
  bm1->xs=bm0->xs;
  bm1->ys=bm0->ys;
  bm1->bs=bm0->bs;
  memcpy(bm1->m, bm0->m, sizeof(int)*bm0->xs*bm0->ys);
  memcpy(bm1->b, bm0->b, sizeof(int)*bm0->bs);
  return bm1;
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

Bm* expand(Bm* bm0, eBMS_VER ver, int detail){
  Bm *bm1=initbm(); /* expand result */

  /* in the case of no brackets */
  if(bm0->bs==0){ 
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
  
  
  /* zero */
  if(bm0->xs==0){ 
    bm1->xs=0;
    bm1->ys=0;
    return bm1;
  }

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
  if(y==0||b==0){ /* child=(0,...,0) or X[0] */
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
  switch(ver){
    case eBMS_VER_1d1:
      for(int n=0;n<bpxs*lnz;n++)*wp++=1;
    break;
    case eBMS_VER_4:
      for(y=0;y<lnz;y++) *wp++=1; /* all bad root elements are ascent */
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
      for(y=0;y<lnz;y++) *wp++=1; /* all bad root elements are ascent */
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
    printf("\nversion             = %s\n",version_string[ver]);
    printf("Input               =");
    printbm(bm0);
    printf("\nParent Index Matrix =");
    for(x=0;x<xs;x++){
      printf("(%d",pim[x*ys]);
      for(y=1;y<ys;y++) printf(",%d",pim[x*ys+y]);
      printf(")");
    }
    printf("\ngood part           =");
    for(x=0;x<r;x++){
      printf("(");
      for(y=0;y<ys;y++){
        printf("%d",bm0->m[x*ys+y]);
        if(y<ys-1)printf(",");
      }
      printf(")");
    }
    printf("\nbad part            =");
    for(x=r;x<xs-1;x++){
      printf("(");
      for(y=0;y<ys;y++){
        printf("%d",bm0->m[x*ys+y]);
        if(y<ys-1)printf(",");
      }
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
int compmat(Bm *a, Bm *b){
  int *pa=&a->m[0];
  int *pb=&b->m[0];
  if(a->ys < b->ys){
    for(int x=0;x<b->xs;x++){
      for(int y=a->ys+1;y<b->ys;y++){
        if(b->m[x*b->ys+y]!=0){
          return +1;
        }
      }
    }
    Bm *b1=initbm();
    b1->xs=b->xs;
    b1->ys=a->xs;
    for(int x=0;x<a->xs;x++){
      memcpy(&b1->m[x*b1->ys],&b->m[x*b->ys],sizeof(int)*(a->ys));
    }
    b1->bs=0;
    int ret = compmat(a, b1);
    if(b1)free(b1);
    return ret;
  }else if(a->ys < b->ys){
    for(int x=0;x<a->xs;x++){
      for(int y=b->ys+1;y<a->ys;y++){
        if(a->m[x*a->ys+y]!=0){
          return +1;
        }
      }
    }
    Bm *a1=initbm();
    a1->xs=a->xs;
    a1->ys=b->xs;
    for(int x=0;x<b->xs;x++){
      memcpy(&a1->m[x*a1->ys],&a->m[x*a->ys],sizeof(int)*(b->ys));
    }
    a1->bs=0;
    int ret = compmat(a1, b);
    if(a1)free(a1);
    return ret;
  }else{
    /* lexicographical compare */
    int n=a->xs*a->ys;
    for(int i=0;i<n;i++){
      if(*pa>*pb)return +1;
      if(*pa<*pb)return -1;
      pa++;
      pb++;
    }
    if     (a->xs > b->xs) return +1;
    else if(a->xs < b->xs) return -1;
    else                   return  0;
  }
}
int isstd(Bm *b, eBMS_VER ver, int detail){
  /* find the first non-lexicographical element from stringly left */
  Bm *s=initbm();
  s->xs=b->xs;
  s->ys=b->ys;
  s->bs=0;
  int *rp=b->m;
  int *wp=s->m;
  for(int x=0;x<s->xs;x++){
    for(int y=0;y<s->ys;y++){
      if(*rp>x){ /* illegal */
        return 0;
      }else if(*rp<x){ /* found */
        *wp++=*rp+1;
        memset(wp,0,sizeof(int)*(s->ys-y-1));
        s->xs=x+1;
        break;
      }
      *wp=x;
      rp++;
      wp++;
    }
  }

  int ret=-1;
  while(ret==-1){
    int oldxsm1=s->xs-1;
    int bplen;
    if(detail){printbm(s);printf("\n");}
    switch(compmat(s,b)){
      case 0:
        ret = 1;
      break;
      case +1:
        /* try expand(s[1]) */
        s->bs=1;
        s->b[0]=1; 
        Bm *s2=expand(s,ver,0);
        bplen = s2->xs-oldxsm1;
        if(bplen!=0){
          s->b[0] = (b->xs-oldxsm1)/bplen+1;
        }else{
          s->b[0]=0;
        }
        if(s2)free(s2);
        /* expand(s[n]) > b */
        s2=expand(s,ver,0);
        if(s)free(s);
        s=s2;
        /* cut in the size of b */
        s->xs=(s->xs>b->xs)?b->xs:s->xs;
        /* reduce  */
        for(int x=0;x<s->xs;x++){
          for(int y=0;y<s->ys;y++){
            if(s->m[x*s->ys+y] > b->m[x*s->ys+y]){
              s->xs=x+1;
            }
          }
        }
        if(detail){printbm(s);printf("\n");}

      break;
      case -1:
        ret = 0;
      default:
      break;
    }
  }
  if(s)free(s);
  return ret;
}
int checkloop(Bm *b, eBMS_VER ver, int detail){
  int ret=-1;
  int ys=b->ys;
  b->bs=1;
  b->b[0]=1;
  Bm *eb=expand(b,ver,0);
  int bplen=eb->xs-(b->xs-1);

  Bm *bpeb=initbm();
  bpeb->xs=bplen+1;
  bpeb->ys=ys;
  bpeb->bs=0;
  memcpy(bpeb->m, &b->m[(b->xs-(bplen+1))*ys], sizeof(int)*bpeb->xs*ys);
  Bm *obpeb=clone(bpeb);
  for(int y=0;y<ys;y++){
    int offset=bpeb->m[y];
    for(int x=0;x<bpeb->xs;x++){
      if(bpeb->m[x*ys+y]-offset>=0) obpeb->m[x*ys+y]-=offset;
    }
  }
  if(compmat(obpeb,b)>0){
    ret = 1;
  }else{
    ret = 0;
  }
  if(detail){
    printf("original: ");printbm(b);printf("\n");
    printf("bad part: ");printbm(bpeb);printf("\n");
    printf("offset  : ");printbm(obpeb);printf("\n");
    if(compmat(obpeb,b)>0){
      printbm(obpeb);
      printf(" > ");
      printbm(b);
      printf(" : Loop!\n");
    }else{
      if(detail){
        printbm(bpeb);
        printf(" < ");
        printbm(b);
        printf(" : The loop was not found.\n");
      }
    }
  }

  if(eb)free(eb);
  if(bpeb)free(bpeb);
  if(obpeb)free(obpeb);
  return ret;
}
static void printhelp(void){
  printf("usage  : bms [-e] [-d] [-r] [-v ver] <bm> # expands bm.\n"
         "       : bms  -s  [-d]      [-v ver] <bm> # check bm is standard or not.\n"
         "       : bms  -l  [-d] [-r] [-v ver] <bm> # check bm has loop in a next expand.\n"
         "       : bms  -h                          # shows help.\n"
         "\n"
         "example: bms \"(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]\"\n"
         "         (0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(3,0,0)(2,2,0)(3,3,1)(4,0,0)\n"
         "\n"
         "example: bms -r \"(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2][1]\"\n"
         "         (0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(3,0,0)(2,2,0)(3,3,1)(4,0,0)[1]\n"
         "         (0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(3,0,0)(2,2,0)(3,3,1)(3,3,1)\n"
         "\n"
         "param  : bm  = bashicu matrix with bracket to expand\n"
         "\n"
         "options:\n"
         " -v ver : expand with version ver.\n"
         "          ver = {4, 2, 1.1} (default = 4)\n"
         " -r     : continue to expand multi-brackets\n"
         " -d     : show detail output\n"
         "\n"
         "notes  : activate function is f(x)=x.\n");
}

int checklooprec(Bm *bm0, Bm *bm1, int depth, char *str, eBMS_VER ver, int detail){
  if(depth == 0) return 0;
  int ret=0;

  /* [0]^1--9 */
  Bm *bm2=clone(bm1);
  for(int i=1;i<=9;i++){
    bm2->xs=bm1->xs-1;
    if(compmat(bm2,bm0)<=0) continue;
    char *str2=malloc(strlen(str)+6);
    sprintf(str2, "%s[0]^%d",str,i);
    if(detail)printf("%s\n", str2);
    ret=checkloop(bm2, ver, detail);
    if(ret)return ret;
    checklooprec(bm0, bm2, depth-1, str2, ver, detail);
  }
  /* [n] */
  for(int k=1;k<=2;k++){
    Bm *bm11=clone(bm1);
    bm11->bs=1;
    bm11->b[0]=1;
    bm2=expand(bm11,ver,0);
    if(compmat(bm2, bm0)<=0) continue;
    char *str2=malloc(strlen(str)+4);
    sprintf(str2, "%s[1]",str);
    if(detail)printf("%s\n", str2);
    ret=checkloop(bm2, ver, detail);
    if(ret)return ret;
    checklooprec(bm0, bm2, depth-1, str2, ver, detail);
  }
}
 
/* 0: cut 1
 * 1: 0^1
 * 9: 0^9
 * 10: 1
 * 11: 2
 * */
