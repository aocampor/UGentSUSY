#! /usr/bin/env python
import os
import sys
import re
import time


pwd = os.environ['PWD']


PDname 		    = sys.argv[1]
SDname 		    = sys.argv[2]
queue               = sys.argv[3]
application         = sys.argv[4]

if (len(sys.argv) > 5 ):	
  dM1                 = sys.argv[5]
  dM2                 = sys.argv[6]
  StopM1              = sys.argv[7]
  StopM2              = sys.argv[8]
	
  if dM1 == "100":
    BDT = "R1" 
  elif dM1 == "200":
    BDT = "R2" 
  elif dM1 == "300":
    BDT = "R3" 
  elif dM1 == "400":
    BDT = "R4" 
  elif dM1 == "500":
    BDT = "R5"
  elif dM1 == "600":
    BDT = "R6"

dataset_path = PDname + "/" + SDname
dataset_name = PDname + "_" + SDname
inputlist = "lists/"+dataset_name+".list"

dir = pwd+"/BDT_skim/"+dataset_name 
if (len(sys.argv) > 5 ):
  dir = pwd+"/BDT_skim/"+dataset_name+"/"+BDT
     


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

    if application == "MakeBDTtree_signal":	
    	   outputfile.write("./"+application+" "+ntpfile[:-1]+ " "+dir+"/output/"+dataset_name+"_"+BDT+".root "+dM1+" "+dM2+" "+StopM1+" "+StopM2+";")
    if application == "MakeBDTtree_bkg":	
    	   outputfile.write("./"+application+" "+ntpfile[:-1]+ " "+dir+"/output/"+dataset_name+"_"+str(ijob)+".root;")

    outputfile.close
    os.system("echo bsub -q 1nd -o "+dir+"/log/"+dataset_name+"_"+str(ijob)+".log source "+outputname)
    os.system("bsub -q "+queue+" -o "+dir+"/log/"+dataset_name+"_"+str(ijob)+".log source "+outputname)

    ijob = ijob+1
    continue
