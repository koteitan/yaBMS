/* Bashicu Matrix
  in: _s[c][r] = matrix, c=column index, r=row index 
  in: _b = bracket */
var Bms=function(_s,_b){
    this.s=_s;
    if(!isNaN(_b)) this.b=_b;
};
/* bms.getLowermostNonzero(c) returns row index of the lowermost nonzero element of c. 
   if lowermost nonzero element of c is not found, it returns -1.*/
Bms.prototype.getLowermostNonzero=function(c){
  var y;
  for(y=c.length-1;y>=0;y--){
    if(c[y]>0) break;
  }
  return y;
}
/* bms.findParent(x,y) returns column index of parent of (x,y).
   bms.findParent() returns bad root of bms.
   It returns -1 when the parent of (x,y) cannot be found. */
Bms.prototype.getParent=function(x,y){
  if(x===undefined){ /* if no parameters */
    x=this.xs()-1; /* x = rightmost column */
    y=this.getLowermostNonzero(this.s[x]);
    if(y<0) return null;
  }
  
  var xp=x; /* parent candidate */
  while(xp>0){
    /* xp = next */
    if(y!=0){
      xp=this.getParent(xp,y-1); /* try column of upper parent */
    }else{
      xp=xp-1; /* simply left */
    }
    if(xp==-1)return xp; /* parent is not found */
    if(this.s[xp][y]<this.s[x][y]){ /* judge smaller */
      return xp;
    }
  }
  return -1;
}
/* m.getAscension() returns ascension matrix A[x][y]. 
   A[x][y]==0 (x,y) is not ascended in copy
   A[x][y]==1 (x,y) is ascended in copy
*/
Bms.prototype.getAscension=function(){
  var A=new Array(this.xs());
  for(var x=0;x<this.xs();x++){
    A[x]=new Array(this.ys());
  }
  for(var y=0;y<this.ys();y++){
  }
}
Bms.prototype.xs=function(){
  return this.s.length;
}
Bms.prototype.ys=function(){
  return this.s[0].length;
}
/* convert to string */
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
/* Parse multiple matrices
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
/* parse matrix from String
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
