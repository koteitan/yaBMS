#!/bin/bash
for i in {6..50}; do
  echo depth=$i 
  echo depth=$i >bm1.1_depth$i.txt
  echo depth=$i >bm2.0_depth$i.txt
  echo depth=$i >bm3.3_depth$i.txt
  echo depth=$i >bm4.0_depth$i.txt
  time ./bms -lrv 1.1 "(0,0,0)(1,1,1)(2,1,0)" "(0,0,0)(1,1,1)(2,1,0)(1,1,1)" $i >>bm1.1_depth$i.txt
  time ./bms -lrv 2   "(0,0,0)(1,1,1)(2,1,0)" "(0,0,0)(1,1,1)(2,1,0)(1,1,1)" $i >>bm2.0_depth$i.txt
  time ./bms -lrv 3.3 "(0,0,0)(1,1,1)(2,1,0)" "(0,0,0)(1,1,1)(2,1,0)(1,1,1)" $i >>bm3.3_depth$i.txt
  time ./bms -lrv 4   "(0,0,0)(1,1,1)(2,1,0)" "(0,0,0)(1,1,1)(2,1,0)(1,1,1)" $i >>bma.04_depth$i.txt
  echo "">>bm1.1_depth$i.txt
  echo "">>bm2.0_depth$i.txt
  echo "">>bm3.3_depth$i.txt
  echo "">>bm4.0_depth$i.txt
done

