#!/bin/bash

FLAG_POOLSOURCE=0;

LIST=RUN2010B_new.list
TMPLIST=tmp
DIR=/data/sfrosali/Zjets/NEW_PATtuple/RUN2010B/

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
