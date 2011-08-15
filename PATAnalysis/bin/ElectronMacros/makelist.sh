#!/bin/bash

FLAG_POOLSOURCE=0;

LIST=WJETSLNU_Winter10_START39_V9.list
TMPLIST=tmp.txt
DIR=/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/Reduction/test/Electrons/MC_Winter10/WLNUMadgraph/crab_0_110811_164213/res/

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
