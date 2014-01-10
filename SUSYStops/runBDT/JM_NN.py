#!/usr/bin/python

import os, sys, glob
import ConfigParser


if __name__ == "__main__":

    #if not len(sys.argv) == 3:
    #    print "Script needs two arguments: lepton type and preselection \nAbort "
    #    sys.exit()

    # get command line arguments
    #lepton = sys.argv[1]
    #preselection = sys.argv[2]
 

    # 1 job per NN configuration --> take input from config file
    config = ConfigParser.RawConfigParser()
    config.read(_FULLPATH_)

    setup = _FILENAME_
    # get info out config
    signalfilelist = config.get('NN','signalfilelist')
    backgroundfilelist = config.get('NN','backgroundfilelist')
    NN_vars = config.get('NN','NN_vars')
    NN_book_options = config.get('NN','NN_book_options')
    NN_train_test = config.get('NN','NN_train_test')

    #check if files exist
    #backgroundfilelist = "/localgrid/msigamani/Stop/CMSSW_4_4_4/src/work_ST600C150/" + lepton + "/" + preselection + "/filelists/bg_" + backgroundfilelist + "_filelist.txt"

    if not os.path.isfile(backgroundfilelist):
        print "Something went wrong: filelist for backgrounds cannot be found!!"
        sys.exit()

    #signalfilelist = "/localgrid/msigamani/Stop/CMSSW_4_4_4/src/work_ST600C150/" + lepton + "/" + preselection + "/filelists/" + signalsample + "_filelist.txt"

    if not os.path.isfile(signalfilelist):
        print "Something went wrong: filelist for signal cannot be found!!"
        sys.exit()

    outputdir = "[%PWD%]/OUTPUT/"+setup+"/NN/"
    output = outputdir + "NN_output.root"
    if not os.path.isdir(outputdir):
        os.makedirs(outputdir)

    executable = "[%STOPBASE%]/NN_allBGs_C.so"
    #first cp executable to /scratch
    c = ("cd [%PWD%];\n" +
         "mkdir msigamani_NN_"+setup+";\n" +
         "cd msigamani_NN_"+setup+";\n" +
         "touch macro.C;\n" +
         "cp " + executable + " NN_allBGs_C.so;\n" +
         "cp " + backgroundfilelist + " filelist.txt;\n" +
         "cp " + signalfilelist + " signal.txt;\n" )
    print "run command\n" + c
    os.system(c)

    tempoutput = "results.root"
    # write macro to load lib and execute NN
    MACRO = open("[%PWD%]/msigamani_NN_"+setup+"/macro.C","w")
    MACRO.write("{\n" +
                "gSystem->Load(\"NN_allBGs_C.so\");\n" +
                "NN(" + ("\"" + tempoutput + "\",\"" +
                         "signal.txt" + "\",\"" +
                         "filelist.txt" + "\",\"" +
                         NN_vars + "\",\"" +
                         NN_book_options + "\",\"" +
                         NN_train_test + "\"") + ");\n" +
                "}\n"
                )
    MACRO.close()

    #now run NN
    c = ("cd [%PWD%]/msigamani_NN_"+setup+";\n" +
         "source [%STOPBASE%]/set_batch_env.csh;\n" +
         "time root -b -l -q macro.C;\n")
    print "run command\n" + c
    os.system(c)

    #clean up
    c = ("cd [%PWD%]/msigamani_NN_"+setup+";\n" +
         "cp " + tempoutput + " " + output + ";\n" +
         "cp -r weights " + outputdir + ";\n" +
         "cd [%PWD%];\n" +
         "rm -r msigamani_NN_"+setup+"/;\n")
    print "run command\n" + c
    os.system(c)




