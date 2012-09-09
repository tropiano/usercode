#!/bin/bash 

#to set: TIER = 0: rm @ CERN; TIER =1: rm @ PISA 
TIER=1

LIST_RM=list_rm.txt
PATH_PISA=srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=/
PATH_CERN=srm://srm-eoscms.cern.ch:8443/srm/v2/server?SFN=/

USER_PISA=/cms/store/user/sandro/
USER_CERN=/eos/cms/store/user/sandro/

DIR=SandroGonziCrabTest/SandroGonziCrabTest/07b5bb36faa323ae4fefd73d7a1ce628/Directory/

EXIT_SUCCESS=1
EXIT_FAILURE=0

if [ $TIER != 0 -a $TIER != 1 ]; then

	echo "ERROR" 
	echo "You have to fix TIER: 0 (Directory at CERN) or 1 (Directory at Pisa)"
	exit $EXIT_FAILURE
fi

if [ $TIER -eq 0 ]; then

	PATHDIR=$PATH_CERN
	USER=$USER_CERN

	/afs/cern.ch/project/eos/installation/pro/bin/eos.select ls -l $USER$DIR > $LIST_RM
	# cmsLs -l $USER$DIR > $LIST_RM

	COUNT=1

	for i in `gawk '{print $9}' $LIST_RM`; do
#	for i in `gawk '{print $5}' $LIST_RM`; do # for cmsLs
		echo "File number " $COUNT
		echo $i "Removing"
		COUNT=`expr $COUNT + 1`;

		srmrm -2 "$PATHDIR$USER$DIR$i"
	done
	
	echo "..." 
	echo "Directory $PATHDIR$USER$DIR Removing"
	srmrmdir -2 "$PATHDIR$USER$DIR"

	exit $EXIT_SUCCESS
fi

if [ $TIER -eq 1 ]; then

	PATHDIR=$PATH_PISA
	USER=$USER_PISA

	lcg-ls -l -b -D srmv2 "$PATHDIR$USER$DIR" > $LIST_RM

	COUNT=1

	for i in `gawk '{print $7}' $LIST_RM`; do
		echo "File number " $COUNT
		echo $i "Removing"
		COUNT=`expr $COUNT + 1`;

		srmrm -2 "$PATHDIR$i"

	done
	
	echo "..." 
	echo "Directory $PATHDIR$USER$DIR Removing"
	srmrmdir -2 "$PATHDIR$USER$DIR"


	exit $EXIT_SUCCESS
fi
