#!/usr/bin/env
cmsenv
setenv STOPBASE $PWD
#setenv ROOTSYS         /afs/cern.ch/work/s/sigamani/public/user/cmssoft/root_5.32.00_x86_64/root/ 
setenv LD_LIBRARY_PATH $ROOTSYS/lib:$LD_LIBRARY_PATH
setenv PATH            $ROOTSYS/bin:$PATH
setenv PYTHONPATH      $ROOTSYS/lib:$PYTHONPATH
