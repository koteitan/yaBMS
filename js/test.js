window.onload=function(){ //entry point
  intext.value="(0,0,0)(1,1,1)(2,0,0)(1,1,1)[3]";  
}
var dothemall=function(){ //button
  var str=intext.value;
  //trimming \n
  str=str.replace(/^\n*/g, "");
  str=str.replace(/\n*$/g, "");
  str=str.replace(/\n+/g, "\n");
  //parse
  var s=Bms.parse(str);
  //test
  outtext.value = s.getAscension().toString();
};

