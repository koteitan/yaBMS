# BMS commander in C
Download:
- download with git
```bash
git clone https://github.com/koteitan/yaBMS.git
cd c/
```
- [download with zip](https://github.com/koteitan/yaBMS/archive/master.zip)

BMS commander has the following features:
- BMS expander
  - Expander: Expand a BMS with the given brackets.
```bash
usage  : ./bms [-e] [-d] [-r] [-v ver] <bm>
  -v ver : expand with version ver.
     ver = {4, 3.3, 2, 1.1, DBMS} (default = 4)
     ex:
      $ ./bms -v4   "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"
      (0,0,0)(1,1,1)(2,1,0)(1,1,0)(2,2,1)(3,2,0)(2,2,0)(3,3,1)(4,3,0)

      $ ./bms -v3.3 "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"
      (0,0,0)(1,1,1)(2,1,0)(1,1,0)(2,2,1)(3,1,0)(2,2,0)(3,3,1)(4,1,0)

  -r : continue to expand multi-brackets
     ex:
      $ ./bms "(0)(1)(2)(3)[1][1][1][1]"
      (0)(1)(2)(2)[1][1][1]

      $ ./bms -r "(0)(1)(2)(3)[1][1][1][1]"
      (0)(1)(2)(2)[1][1][1]
      (0)(1)(2)(1)(2)[1][1]
      (0)(1)(2)(1)(1)[1]
      (0)(1)(2)(1)(0)(1)(2)(1)

  -d : show detail output
     ex:
      $ ./bms -d "(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]"

      version             = BM4
      Input               =(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]
      Parent Index Matrix =(-1,-1,-1)(0,0,0)(1,-1,-1)(0,0,0)
      good part           =
      bad part            =(0,0,0)(1,1,1)(2,0,0)
      bad root            = 0
      lnz                 = 2
      delta               =(1,1)
      Asension Matrix     =(1,1,1)(1,1,1)(1,0,0)

      (0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,2,1)(3,0,0)(2,2,0)(3,3,1)(4,0,0)
```

  - Standard checker: check the given BMS is standard or not.
```./bms  -s [-d] [-v ver] <bm>
    ex:
      $ ./bms -s "(0)(1)"
      1
      $ ./bms -s "(1)"
      0
      $ ./bms -s "(0)(0)(1)"
      0

  -d : show detail output
    ex:
      $ ./bms -sd "(0,0,0)(1,1,1)(2,0,0)(1,1,1)(2,1,0)"
      (0,0,0)(1,1,1)(2,1,0)
      (0,0,0)(1,1,1)(2,0,0)(3,1,1)
      (0,0,0)(1,1,1)(2,0,0)(3,1,1)
      (0,0,0)(1,1,1)(2,0,0)(3,1,0)
      (0,0,0)(1,1,1)(2,0,0)(3,1,0)
      (0,0,0)(1,1,1)(2,0,0)(3,0,0)
      (0,0,0)(1,1,1)(2,0,0)(3,0,0)
      (0,0,0)(1,1,1)(2,0,0)(2,0,0)
      (0,0,0)(1,1,1)(2,0,0)(2,0,0)
      (0,0,0)(1,1,1)(2,0,0)(1,1,1)(2,0,0)
      (0,0,0)(1,1,1)(2,0,0)(1,1,1)(2,0,0)
      non-standard.
```
  - Comparator: compare two BMS.
``` 
./bms  -c[d] <bm0> <bm1> # compare the size of them.
  -d : show detail output
  ex.
    $ ./bms -c "(0,0)(1,1)(2,0)" "(0,0)(1,1)(1,1)"
    1

    $ ./bms -cd "(0,0)(1,1)(2,0)" "(0,0)(1,1)(1,1)"
    (0,0)(1,1)(2,0) > (0,0)(1,1)(1,1)
```
  - Loop finder
    - Find loop. It detects that the given matrix gets greater after expanding it. It may have a detection leak.
```./bms  -l  [-d] [-v ver] <bm> # check bm has loop in a next expand.
./bms  -l  [-d] [-r] [-v ver] <bm0> <bm1> [<depth>]
# search loop from bm1 until bm0 in <depth> times-expansion.
```
