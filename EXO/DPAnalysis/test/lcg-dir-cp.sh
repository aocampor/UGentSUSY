#!/bin/bash


MKDIR="/opt/d-cache/srm/bin/srmmkdir"
#CP="/afs/cern.ch/user/s/sigamani/scratch0/lcg-cp -v --vo cms"
CP="lcg-cp"

if [ $# -ne 2 ]
then
    echo "Usage:$0 <from> <to>" 
    echo "<from> and <to> are two srm dir endpoints (so srm://..../path/to/a/dir/not/a/file)"
    exit 1
fi

FROM=$1
TO=$2
DIRNAME=`basename $FROM`
#echo MKDIR $TO/$DIRNAME
for i in `lcg-ls srm://srm-eoscms.cern.ch:8443/$FROM/`
do

    FILENAME=`basename $i`
    COMMAND="$CP srm://maite.iihe.ac.be:8443/$FROM/$FILENAME srm://srm-eoscms.cern.ch:8443/srm/v2/server?SFN=/eos/cms/store/caf/user/sigamani/$TO/$DIRNAME/$FILENAME"
#    COMMAND="$CP srm://srm-eoscms.cern.ch:8443/$FROM/$FILENAME srm://maite.iihe.ac.be/pnfs/iihe/cms/store/user/sigamani/$DIRNAME/$FILENAME"
    echo $COMMAND
    $COMMAND
done
