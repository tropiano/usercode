#!/bin/bash

LIST=QCD_30to80_FullSample.txt
TMPLIST=tmp_list.txt
#DIR=/raid/sfrosali/PATtuple/QCD_EMEnriched/Pt20to30
DIR=/raid/sfrosali/PATtuple/Background/QCD_EMEnriched/Pt30to80/
#DIR=/castor/cern.ch/user/s/sfrosali/ZeeSummer09/background/QCD_EMEnriched_Pt20to30/

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
