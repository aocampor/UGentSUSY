#! /usr/bin/env python
import os
import sys
import re
import time
import string, fileinput


def replaceStr(namein,nameout,list):
    fin=open(namein, 'r')
    fout=open(nameout, 'w')
    for line in fin:
        for ilist in range(len(list)):
            if ilist%2 == 0:
                lineno1 = 0
                lineno1 = string.find(line, list[ilist])
                if lineno1 > 0:
                    line =line.replace(list[ilist], list[ilist+1])
        fout.write(line)

def divide(file,numf):
    files = [item[:-1] for item in os.popen('cat ' + file)]
    tot = [item[:-1] for item in os.popen('cat ' + file + ' | wc | awk \'{print $1}\'')]
    out = []
    cont1 = 0;
    cont2 = 0;
    temp = ''
    for i in files:
        temp = temp + '\'file:' + str(i) + '\''
        cont1 = cont1 + 1
        cont2 = cont2 + 1
        if(numf != 1 ):
            if(int(cont1) == int(tot[0])-1):
                if (int(cont2) == int(numf)):
                    cont2 = cont2 - 1
                else :
                    cont2 = numf-1
        if(int(cont2) == int(numf)):
            out.append(temp)
            cont2 = 0
            temp = ''
        else :
            temp = temp + ','
    return out


if len(sys.argv) != 4:
    print "example : python sendOnBatch.py <Number of jobs> <Walltime> <Directory Name>"
    print "example : python sendOnBatch.py 1 00:05:00 JOB"
    sys.exit(1)


inputlist = "list.txt"
ijobmax = int(sys.argv[1])
walltime = sys.argv[2]
dir = sys.argv[3]


pwd = os.environ['PWD']


#dir = "JOB" 
os.system("rm -r "+dir)
os.system("mkdir -p "+dir)
os.system("mkdir -p "+dir+"/log/")
os.system("mkdir -p "+dir+"/input/")
os.system("mkdir -p "+dir+"/src/")
os.system("mkdir -p "+dir+"/res/")


input = open(inputlist)
inputfiles = input.readlines()




######################################
ijob=0


filenum = len(inputfiles) 
jobnum = filenum/ijobmax

splitter = divide("list.txt",jobnum)


while (ijob < ijobmax):


       	for line in range(0,ijobmax):
       	for line in range(0,ijobmax):
                    replaceStr('/localgrid_mnt/localgrid/sigamani/CMSSW_5_3_2_STOP/src/Patuples_prod_step1/patLayer1_fromAOD_MC_new_cfg.py',dir+'/input/patLayer1_fromAOD_MC_'+str(ijob)+'_new_cfg_tmp1.py',['--file--', splitter[ijob]])
		    replaceStr(dir+'/input/patLayer1_fromAOD_MC_'+str(ijob)+'_new_cfg_tmp1.py',dir+'/input/patLayer1_fromAOD_MC_'+str(ijob)+'_new_cfg_tmp.py',['--out--',str(ijob)])
		    replaceStr(dir+'/input/patLayer1_fromAOD_MC_'+str(ijob)+'_new_cfg_tmp.py',dir+'/input/patLayer1_fromAOD_MC_'+str(ijob)+'_new_cfg.py',['--pwd--',pwd+dir+"/res/"])

          	    os.system("rm "+dir+"/input/patLayer1_fromAOD_MC_"+str(ijob)+"_new_cfg_tmp.py")
          	    os.system("rm "+dir+"/input/patLayer1_fromAOD_MC_"+str(ijob)+"_new_cfg_tmp1.py")




######################################

	outputname = dir+"/src/submit_"+str(ijob)+".src"
	outputfile = open(outputname,'w')
	outputfile.write('pwd=$PWD\n')
	outputfile.write('setenv X509_USER_PROXY localgrid/sigamani\n')
	outputfile.write('source $VO_CMS_SW_DIR/cmsset_default.sh\n')
	outputfile.write('cd /localgrid/sigamani/CMSSW_5_3_2_STOP/src/Patuples_prod_step1/\n')
	outputfile.write('eval `scram runtime -sh`\n')                                        
	outputfile.write('cd $pwd\n')
	outputfile.write("cmsRun /localgrid/sigamani/CMSSW_5_3_2_STOP/src/Patuples_prod_step1/"+dir+"/input/patLayer1_fromAOD_MC_"+str(ijob)+"_new_cfg.py\n") 
	outputfile.close

#	os.system('echo qsub -q localgrid@cream01 -o '+dir+'/log/script'+str(ijob)+'.stdout -e '+dir+'/log/script'+str(ijob)+'.stderr -l walltime=0:50:00 '+dir+'/src/submit_'+str(ijob)+'.src')
	os.system("echo qsub -q localgrid@cream01 -o "+dir+"/log/script"+str(ijob)+".stdout -e "+dir+"/log/script"+str(ijob)+".stderr -l walltime="+str(walltime)+" "+dir+"/src/submit_"+str(ijob)+".src >> run.sh")
#	os.system("source run.sh") 
#        os.system("rm run.sh") 

	ijob = ijob+1
	#time.sleep(1.)
	continue
