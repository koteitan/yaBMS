import re # regexp

# BMS
class Bms:
  # constructor
  # text = string of BMS
  # "(0,0,0)(1,1,1)" or "(0,0,0)(1,1,1)[3]"
  def __init__(self, text):
    str=re.sub(r'[\s\t]',"",text) # trim spaces and tabs
    clist = re.findall(r'\([\d,]*\)',text)

    # parse matrix
    self.matrix=[]
    for c in clist:
      elist=re.findall(r'\d+',c)
      self.matrix.append([])
      c=self.matrix[len(self.matrix)-1]
      for e in elist:
        c.append(int(e))
        
    # parse bracket
    m=re.findall(r'\[\d*\]',text)
    if len(m)==0:
      self.bracket = None
    else:
      self.bracket = int(re.sub(r'[\[\]]',"",m[0]))

  # to string
  def tostr(self):
    out=""
    for c in self.matrix:
      out+="("+str(c[0])
      for ri in range(1,len(c)):
        out+=","+str(c[ri])
      out+=")"
      
    if self.bracket is not None:    
      out+="["+str(self.bracket)+"]"
      return out
      
# test
def test():
  x=Bms("(0,0,0)(1,1,1)[1]")
  print(x.tostr())

test()
