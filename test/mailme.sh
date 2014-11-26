#!/bin/bash

#figure out the system I'm working on
hostname -f | grep 'cern' 2>&1 > /dev/null
CERN=$?
hostname -f | grep 'psi' 2>&1 > /dev/null
PSI=$?

[ "$CERN" == "0" ] && echo "You are at CERN"
[ "$PSI" == "0" ] && echo "You are at PSI"
[ "$CERN" != "0" ] && [ "$PSI" != "0" ] && echo "I don't know where you are."

while sleep 5s; do
	if [ "$PSI" == "0" ]; then
		NUM=$(qstat | grep -v 'job-ID' | grep -v -- '----------------------------' | wc -l)
	fi;
	if [ "$CERN" == "0" ]; then
		NUM=$(bjobs 2>/dev/null | grep -v 'JOBID'  | wc -l)
	fi;
	echo "Remaining $NUM jobs"
	if [ "$NUM" == "0" ];then
		echo "All jobs on $hostname are finished" | mail -s "Jobs Done" $USER@cern.ch
		break;
	fi 
done
