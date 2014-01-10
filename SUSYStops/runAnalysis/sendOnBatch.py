#! /usr/bin/env python
import os
import sys
import re
import time


pwd = os.environ['PWD']



PDname              = sys.argv[1]
SDname              = sys.argv[2]
queue               = sys.argv[3]
application         = sys.argv[4]
NNvar               = sys.argv[5]
MLP_dir             = sys.argv[6]
Decay_Mode          = sys.argv[7]

if (len(sys.argv) > 8 ):
  stopmass            = sys.argv[8]
  neutmass            = sys.argv[9]


dataset_path = PDname + "/" + SDname
dataset_name = PDname + "_" + SDname
inputlist = "../../babyReaderSTOPS/lists/"+dataset_name+".list"


if (len(sys.argv) > 8 ):
  dir = pwd+"/ntuplesMT150/"+MLP_dir+"/"+Decay_Mode+"/signal/"+stopmass+"/"+neutmass+"/"
else:
  dir = pwd+"/ntuplesMT150/"+MLP_dir+"/"+Decay_Mode+"/"+PDname+"/"


os.system("rm -r "+dir)
os.system("mkdir -p "+dir)
os.system("mkdir -p "+dir+"/log/")
os.system("mkdir -p "+dir+"/input/")
os.system("mkdir -p "+dir+"/output/")
os.system("mkdir -p "+dir+"/src/")


#######################################
input = open(inputlist)
inputfiles = input.readlines()
######################################

ijob=0

while (len(inputfiles) > 0):
    inputfilename = dir+"/input/input_"+str(ijob)
    inputfile = open(inputfilename,'w')
    for line in range(min(1,len(inputfiles))):
        ntpfile = inputfiles.pop()
        if ntpfile != '':
            inputfile.write(ntpfile)

    inputfile.close()

######################################


    outputname = dir+"/src/"+dataset_name+"_"+str(ijob)+".src"
    outputfile = open(outputname,'w')
    outputfile.write('#!/bin/bash\n')
    outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc462\n')
    outputfile.write('cd '+pwd+'; eval `scramv1 runtime -sh`; \n')
    if (len(sys.argv) > 8 ):
      outputfile.write("./"+application+" "+ntpfile[:-1]+ " "+dir+"/output/"+dataset_name+"_"+str(ijob)+".root "+NNvar+" "+MLP_dir+" "+Decay_Mode+ " "+stopmass+" "+neutmass+"  ;")
    else: 
      outputfile.write("./"+application+" "+ntpfile[:-1]+ " "+dir+"/output/"+dataset_name+"_"+str(ijob)+".root "+NNvar+" "+MLP_dir+" "+Decay_Mode+" ;")
    outputfile.close
    os.system("echo bsub -q 1nd -o "+dir+"/log/"+dataset_name+"_"+str(ijob)+".log source "+outputname)
    os.system("bsub -q "+queue+" -o "+dir+"/log/"+dataset_name+"_"+str(ijob)+".log source "+outputname)
    #os.system("sleep 0.5")

    ijob = ijob+1
    continue
