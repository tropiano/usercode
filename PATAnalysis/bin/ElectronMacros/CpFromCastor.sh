#!/bin/bash

LISTTO=list_to_2.txt
TMPLISTFROM=tmp_from_2.txt
TMPLISTTO=tmp_to_2.txt
FROMDIR=/castor/cern.ch/user/s/sfrosali/ZeeCollisions10/MinBias_Commissioning10_GOODCOLL-v8/
TODIR=/raid/sfrosali/PATtuple/DATA/MinBias_Commissioning10_GOODCOLL-v8/

rfdir $FROMDIR > $TMPLISTFROM
ls -l $TODIR > $TMPLISTTO

COUNT=0

for i in `gawk '{print $9}' $TMPLISTTO`; do

if [ $COUNT -eq 0 ] ; then
echo $i > $LISTTO
COUNT=1
else
echo $i >> $LISTTO
fi
done;

COUNT=1

for i in `gawk '{print $9}' $TMPLISTFROM`; do

if grep -q $i $TMPLISTTO ; then
echo "File number " $COUNT
echo $i "Already copied"
COUNT=`expr $COUNT + 1`;
else
echo "File number " $COUNT
echo $i "Copying"
COUNT=`expr $COUNT + 1`;
rfcp $FROMDIR$i $TODIR$i
fi
done;



