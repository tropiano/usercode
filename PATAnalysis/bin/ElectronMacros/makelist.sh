#!/bin/bash

FLAG_POOLSOURCE=0;

LIST=PATsignal.list
TMPLIST=tmp
DIR=/data/sandro/Analisi/rel_CMSSW_4_2_4/CMSSW_4_2_4/src/Firenze/Reduction/test/crab_0_120104_190703/res/

ls -l $DIR | grep root > $TMPLIST

COUNT=0

for i in `gawk '{print $9}' $TMPLIST`; do

if [ $COUNT -eq 0 ] ; then
echo $DIR$i > $LIST
COUNT=1
else
if [ $FLAG_POOLSOURCE -eq 0 ] ; then
echo $DIR$i >> $LIST
else
echo "'file:"$DIR$i"'," >> $LIST
fi
fi
done;
