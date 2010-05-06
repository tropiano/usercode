#!/bin/bash

LIST=Summer09_test.txt
TMPLIST=tmp_list.txt
DIR=/data/sfrosali/PATtuple/MC/ZeeSummer09_7TeV_akt5_335_wGenP/

ls -l $DIR > $TMPLIST
#rfdir $DIR > $TMPLIST

COUNT=0

for i in `gawk '{print $9}' $TMPLIST`; do

if [ $COUNT -eq 0 ] ; then
echo $DIR$i > $LIST
COUNT=1
else
echo $DIR$i >> $LIST
fi
done;
