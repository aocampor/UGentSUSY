pwd=$PWD
cd /afs/cern.ch/work/s/sigamani/public/CMSSW_5_3_7_STOP/src/ 
eval `scram runtime -sh`                                    
cd $pwd
export PYTHONPATH=$STOPBASE/scripts:$PYTHONPATH
