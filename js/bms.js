/* ---------------------------------------------------------
  Bashicu Matrix
  in: _s[c][r] = matrix, c=column index, r=row index 
  in: _b = bracket */
var Bms=function(_s,_b){
    this.s=_s;
    if(!isNaN(_b)) this.b=_b;
};
/* ---------------------------------------------------------
  bms.expand() returns one step expanded matrix from bms */
Bms.prototype.expand=function(){
  var r=this.getBadRoot();
};
/* ---------------------------------------------------------
  bms.findParent(x,y) returns column index of parent of (x,y).
  It returns -1 when the parent of (x,y) cannot be found. */
Bms.prototype.getParent=function(x,y){
  var p = x; /* parent candidate */
  while(p>0){
    /* p = next */
    if(y!=0){
      p=this.getParent(p,y-1); /* try column of upper parent */
    }else{
      p=p-1; /* simply left */
    }
    if(p==-1)return p; /* parent is not found */
    if(this.s[p][y]<this.s[x][y]){ /* judge smaller */
      return p;
    }
  }
  return -1;
}
/* ---------------------------------------------------------
  bms.getBadRoot() returns column index of bad root of bms.
   It returns -1 when the parent of (x,y) cannot be found. */
Bms.prototype.getBadRoot=function(){
  /* x = rightmost column */
  var x=this.xs()-1;
  /* y = Lowermost Nonzero row of x */
  var y=this.getLowermostNonzero(this.s[x]);
  return this.getParent(x,y);
}
/* ---------------------------------------------------------
  bms.getAscension() returns ascension matrix A[x][y] of bms. 
   A[x][y]==0 (x,y) is not ascended in copy
   A[x][y]==1 (x,y) is ascended in copy
*/
Bms.prototype.getAscension=function(){
  var A = Array.zeros([this.xs(), this.ys()]); /* init */
  var r = this.getBadRoot(); /* bad root */
  if(r==-1) return A; /* bad root is not found -> all zero */
  for(var y=0;y<this.ys();y++){
    A[r][y]=1; /* bad root is ascended */
    for(var x=1;x<this.xs();x++){
      var p=this.getParent(x,y);
      if(p!=-1 && A[p][y]==1) A[x][y]=1; /* propagate from parent */
    }
  }
  return A;
}
/* ---------------------------------------------------------
  bms.getLowermostNonzero(c) returns row index of the lowermost nonzero element of c. 
   if lowermost nonzero element of c is not found, it returns -1.*/
Bms.prototype.getLowermostNonzero=function(c){
  var y;
  for(y=c.length-1;y>=0;y--){
    if(c[y]>0) break;
  }
  return y;
}
/* ---------------------------------------------------------
  bms.xs() returns number of columns of bms. */
Bms.prototype.xs=function(){
  return this.s.length;
}
/* ---------------------------------------------------------
  bms.ys() returns number of rows of bms. */
Bms.prototype.ys=function(){
  return this.s[0].length;
}
/* ---------------------------------------------------------
  convert to string */
Bms.prototype.toString=function(){
  var str="";
  for(var c=0;c<this.xs();c++){
    str+="(";
    for(var r=0;r<this.ys();r++){
      str += this.s[c][r];
      if(r!=this.ys()-1)str += ",";
    }
    str+=")";
  }
  if(this.b!==undefined){
    str+="["+this.b+"]";
  }
  return str;
}
/* ---------------------------------------------------------
  Parse multiple matrices
  Bms.multiparse("(0,1,2)(3,4,5)\n(6,7,8)(9,10,11)")
= [ [ [0,1,2],[3,4,5]],[[6,7,8],[9,10,11]  ] ]  */
Bms.multiparse=function(str){
  var a=str.split("\n");
  var mm=new Array(a.length);
  for(var m=0;m<a.length;m++){
    mm[m]=Bms.parse(a[m]);
  }
  return mm;
}
/* ---------------------------------------------------------
  parse matrix from String
  parse("(0,1,2)(3,4,5)") = Bms([[0,1,2],[3,4,5] ]) */
Bms.parse=function(str){
  var s=[[]];
  //              1  2   3   4
  var r = /^(\s*\()(.*?)(\))(.*)/;
  var m=str.match(r);
  var ci=0;
  while(m!=null){
    var c=m[2].split(",");
    for(ri=0;ri<c.length;ri++){
      s[ci].push(parseInt(c[ri],10));
    }
    str=m[4];
    if(str=="")break;
    m=str.match(r);
    if(m!=null){
      s.push([]);
      ci++;
    }
  }
  var b;
  //                     1   2    3
  m=str.match(/(\[)([\s\d]*)(\])/);
  if(m!=null) b = parseInt(m[2]);
  if(s.dims==1){
    return new Bms([s],b); // primitive sequence
  }else{
    return new Bms(s,b);
  }
}
