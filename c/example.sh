#!/bin/bash
VERSIONS="1.1 2.0 3.3 4.0"
M0="(0,0,0)(1,1,1)(2,1,0)"
M1="(0,0,0)(1,1,1)(2,1,0)(1,1,1)"
Mn=1
mkdir -p $Mn
for d in {6..50}; do
  echo $0
  echo depth=$d
  for v in $VERSIONS; do
    echo depth=$d >$Mn/bm${v}_depth$d.txt
    time ./bms -lrv 1.1 $M0 $M1 $d >>$Mn/bm${v}_depth$d.txt
    echo "">>$Mn/bm${v}_depth$d.txt
  done
done

