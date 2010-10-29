#!/bin/bash

FLAG_POOLSOURCE=0;

LIST=ZeeCollisionsSep11_2.txt
TMPLIST=tmp.txt
DIR=/data/egallo/ZeeCollisions10/Sep11/

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
