/* Bashicu Matrix
  in: _s[c][r] = matrix, c=column index, r=row index 
  in: _b = bracket */
var Bms=function(_s,_b){
    this.s=_s;
    if(!isNaN(_b)) this.b=_b;
};
/* m.findParent(ci) returns index of parent of ci.
   It returns -1 when the parent of ci cannot be found. */
Bms.prototype.findParent=function(ci){
  if(ci===undefined)ci=this.length()-1;
  for(var c=ci-1;c>=0;c--){
    if(this.s[c][0]==this.s[ci][0]-1){
      return c;
    }
  }
  return -1;
}
Bms.prototype.cols=function(){
  return this.s.length;
}
Bms.prototype.rows=function(){
  return this.s[0].length;
}
/* convert to string */
Bms.prototype.toString=function(){
  var str="";
  for(var c=0;c<this.cols();c++){
    str+="(";
    for(var r=0;r<this.rows();r++){
      str += this.s[c][r];
      if(r!=this.rows()-1)str += ",";
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
