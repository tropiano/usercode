#!/bin/bash

WORK_DIR=$PWD

cd $WORK_DIR

for (( i=403; i<410; i++))
do

FILE1="/castor/cern.ch/user/s/sfrosali/ZeeSummer09/Background/QCD_EMEnriched/QCD_EMEnriched_Pt20to30_Summer09_"$i.root;

echo $i;

rfcp $FILE1 . ;

done;

