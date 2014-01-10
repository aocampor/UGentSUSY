#!/usr/bin/python

usage=(
    """
    JM is a tool to run a script over many input files using the batch system

    #############################################
                       USAGE
    #############################################
    
    CREATING JOBS:

    ./JM.py \"path/to/input/files/*\" script.tmplt.sh --create [more options]
    ./JM.py --filelist=path/to/file_list.txt script.tmplt.sh --create [more options]
    ./JM.py \"path/to/input/files/*\" \"script.tmplt.sh option1 option2\" --create [more options]

    each line in file_list.txt or file matching the wildcard will result in a job
    processing that particular file with a script built from script.tmplt.sh.
    (When using wildcards, the first argument must be between quotation marks.)
    (When giving options to the template script, mind the quotation marks.)
    
    JM will create a direcory named JM_<date>_<time> to store jobs and job output.
    A soft link named JM is created for easy access to the latest created jobs.
    
    The template script should be a ordinary script, except that:

     -following variables must be considered as predefined:
    
          _FILENAME_=<name of input file without extension(s)>; # usefull to form a unique name for the output file of a job.
          _FULLPATH_=<full path to input file>;                 # usefull to access the input file of the job.
          _JMDIR_=<full path to JM_<date>_<time>;               # usefull to gather all output of one creation submission round in the same directory.

      eg in a bash script this means one should use variable $_FILENAME_, $_FULLPATH_, $_JMDIR_
    
     - JM will replace any string of the form [%VARNAME%]
       by the value of the environmental variable VARNAME at the time of the job creation.
       JM will check whether the environmental variable VARNAME does exist.
       This is useful to set the environment in the job, eg:

          cd [%CMSSW_BASE%]/src
          cmsenv
          cd [%PWD%]

    options compatible with --create
    --group=N                      N input files will be processed in one batch job
                                   this means that N script will run in one batch job


    SUBMITTING JOBS
    
    ./JM.py --submit

    options compatible with --submit:
    --create                        see above
    --status                        see below
    --njobs N                       only submit first N jobst
    --mode MODE                     use mode MODE to submit. possible options: 'bsub' (default,lxplus), 'qsub' (T2_BE_IIHE)
    --continue JM_YYYYMMDD_HHMMSS   continue from directory JM_YYYYMMDD_HHMMSS
    --walltime HH:MM:SS             only used with --mode qsub
    --queue 	                    only used with --mode bsub

    CHECK JOB STATUS

    ./JM.py --status [more options]

    options compatible with --status
    --create                        see above
    --status                        see below
    --mode MODE                     use mode MODE to submit. possible options: 'bsub' (default,lxplus), 'qsub' (T2_BE_IIHE)
    --continue JM_YYYYMMDD_HHMMSS   continue from directory JM_YYYYMMDD_HHMMSS    


    RUN LOCAL

    for testing and debuggin, it is good to run a single job local.
    to do this, simply do the creation step then run a single job like this:

    ./JM/jobs/job_0.sh
    """
    )

import time
import stat
import sys
import os
import subprocess
import ConfigParser
from optparse import OptionParser
from time import localtime, strftime
import shutil
import re
import glob
from math import floor,log
import StringIO

timestamp = strftime("%Y%m%d_%H%M%S",localtime())

#######################################
#    parse command line options
#######################################


def crash(message=None):
    if not message is None:
        print message
        print "type " + sys.argv[0] + " --help for help"
        print "type " + sys.argv[0] + " --usage for detailed information un usage"
        sys.exit()

parser = OptionParser()

# main options
parser.add_option("--create",dest="create",action="store_true",default=False,
                  help ="create jobs")
parser.add_option("--submit",dest="submit",action="store_true",default=False,
                  help ="submit jobs")
parser.add_option("--status",dest="status",action="store_true",default=False,
                  help ="get job status")

parser.add_option("--filelist",dest="filelist",action="store",type="string",
                  help="give a list of files in .txt format rather than wildcard",metavar="LIST")
parser.add_option("--njobs",dest="njobs",action="store",type="int",
                  help ="only create/submit N jobs",metavar="N")
parser.add_option("-c","--continue",dest="cont",action="store",type="string",
                  help ="apply command to task stored in DIR",metavar="DIR")
parser.add_option("--mode",dest="mode",action="store",default="bsub",
                  help = "work in mode MODE, available: bsub(lxplus), qsub(T2_BE_IIHE))",metavar="MODE")
parser.add_option("--group",dest="group",action="store",default=1,
                  type="int", help="run N jobs in 1 batchjob",metavar="N")
parser.add_option("--pretend",dest="pretend",action="store_true",default=False,
                  help="only print",metavar="N")
parser.add_option("--wait",dest="wait",action="store",default=0,
                  type="int",help="wait N seconds between two submits",metavar="N")
parser.add_option("--usage",dest="usage",action="store_true",default=False,
                  help ="print detailed information on usage")
parser.add_option("--walltime",dest="walltime",action="store",default="36:00:00",
                  type="string",help="walltime for the qsub command",metavar="HH:MM:SS") # added by Nadja
parser.add_option("--queue",dest="queue",action="store",default="1nd",
                  type="string",help="queue for the bsub command",metavar="queue") # added by Nadja
(options, args) = parser.parse_args()

if options.usage:
    print usage
    sys.exit()
if not options.create and not options.submit and not options.status:
    crash("ERROR: use at least one of the options '--create,--submit,--status'")

tmplt = ""
tmplt_options = ""
files = []
if options.create:
    ok = True
    message = ""
    # --create needs two arguments
    if (options.filelist==None and len(args)!=2) or (options.filelist!=None and len(args)!=1):
        message = "wrong number of arguments"
        ok = False
    else:
        if options.filelist == None:
            tmplt = args[1]
            files = glob.glob(args[0])
        else:
            tmplt = args[0]
            FILES=open(options.filelist)
            for line in FILES:
                line = line.strip("\n")
                line = line.strip(" ")
                re.sub("#.*$",line,"")
                files.append(line)
            FILES.close()
        
        # first argument must be a string matching at least one file
        if len(files) == 0:
            message = "no files matching '" + args[0]
            ok = False
        # second must be an existing script
        # if not os.path.isfile(args[1]):
        #    message = "no such file or directory: " + tmplt
        #    ok = False
    if not ok:
        crash("ERROR: wrong usage of option --create\n" +
              "       " + message + "\n"
              "       use option --create as follows:\n" +
              "       "  + sys.argv[0] + " \"path/to/input/files/*\" script.tmplt.sh --create [more options]\n"
              "       "  + sys.argv[0] + " --filelist=path/to/file/list.txt script.tmplt.sh --create [more options]\n"
              "       !!! NOTE THE QUOTATION MARKS !!!")
    args = []
        
if options.submit and len(args)!=0:
    crash("ERROR: too many arguments\n"
          "       use option --submit as follows:\n"
          "       " + sys.argv[0] + " --submit\n" +
          "       or in combination with --create:\n" +
          "       " + sys.argv[0] + " \"path/to/input/files/*\" script.tmplt.sh --create --submit [more options]\n"+
          "       !!! NOTE THE QUOTATION MARKS !!!")

if options.mode != "bsub" and options.mode != "qsub":
    crash("ERROR: use of unsuported mode\n" +
          "       please use 'bsub' or 'qsub'")

if options.cont and options.create:
    crahs("ERROR: options --create and --continue are not compatible")

#######################################
#   find/create current work dir
#######################################

JMworkdir = ""
if options.create:
    # create the JM directory
    if os.path.exists("JM"):
        os.remove("JM")
    JMworkdir = strftime("JM_" + timestamp,localtime())
    if os.path.exists(JMworkdir):
        print "ERROR: directory '" + JMworkdir + "' already exist"
        sys.exit()
    os.mkdir(JMworkdir)
    os.symlink(JMworkdir,"JM")

else:
    JMworkdir = ""
    if options.cont:
        JMworkdir = options.cont
        if not os.path.isdir(JMworkdir):
            print "ERROR: no such directory " + JMworkdir
            sys.exit()
    else:
        if not os.path.isdir("JM"):
            print "ERROR: could not find directory 'JM'"
            print "       try running with option --create"
            sys.exit()
        JMworkdir = os.readlink("JM")

JMworkdir_short = JMworkdir.replace("JM_20","");

#######################################
#   create entry in log
#######################################

LOG = None
if os.path.isfile("JM.log"):
    LOG = open("JM.log","a")
else:
    LOG = open("JM.log","w")
    LOG.write("TIMESTAMP         JMDIR                COMMAND\n")
LOG.write(timestamp + "   " + JMworkdir + "   " + " ".join(sys.argv) + "\n")
LOG.close()
    
#######################################
#    create jobs
#######################################

# function to parse template script
def parseTmplt(template):

    TMPLT = open(template,"r")
    tmplt = TMPLT.read()
    TMPLT.close()
    tmplt_header = ""
    tmplt_body = ""
    in_header = False

    lines = reversed(tmplt.split("\n"))
    for line in lines:
        # catch the header of the script
        if re.match("^#!.*$",line):
            in_header = True
        
        # substitutions global variables
        while True:
            #m = re.match("^.*\[%\$(.*?)%\].*$",line)
            m = re.match("^.*\[%(.*?)\%].*$",line)
            if not m:
                break
            var = m.group(1)
            toreplace = "\[%" + var + "%\]"
            if not var in os.environ:
                print "ERROR: no environmental variable " + var + " defined"
                sys.exit()
            replacement = os.environ[var]
            (line,n) = re.subn(toreplace,replacement,line)
        # add line to header or to body
        if in_header:
            tmplt_header = line + "\n" + tmplt_header
        else:
            tmplt_body = line + "\n" + tmplt_body
    
    return tmplt_header,tmplt_body
    
# create those jobs
if options.create:
    # split tmplt script from options
    pos = tmplt.find(" ")
    tmplt_options = ""
    if pos > 0:
        tmplt_options = tmplt[pos:len(tmplt)]
        tmplt = tmplt[0:pos]
    
    # create the list of files to run over
    # store the list in a txt file (for documentation only)
    files.sort()
    FILES = open(JMworkdir + "/files.txt",'w')
    for file in files:
        FILES.write(file + "\n")

    # keep a copy of the template script
    shutil.copyfile(tmplt,JMworkdir + "/tmplt.sh")

    # create subjobs for each file
    jobdir = JMworkdir + "/job/"
    subjobdir = JMworkdir + "/job/subjob/"
    os.mkdir(jobdir)
    os.mkdir(subjobdir)
    (tmplt_header,tmplt_body) = parseTmplt(tmplt)

    index = 0
    subindex = 0
    subjobs = []
    for f in range(0,len(files)):
        fullpath = os.path.abspath(files[f])
        m = re.match(".*/([^\.]*).*$",files[f])
        if m is None or m.group(1) is None:
            print "ERROR: I could not parse the filenames correctly, sorry"
            sys.exit()
        filename = m.group(1)

        script = tmplt_header
        script = script + "_INDEX_=\"" + str(index) + "\"\n"
        script = script + "_FULLPATH_=\"" + fullpath + "\"\n"
        script = script + "_FILENAME_=\"" + filename + "\"\n"
        script = script + "_JMDIR_=\"" + os.path.abspath(JMworkdir) + "\"\n"
        script = script + tmplt_body
        
        subjobn = subjobdir + "/" + filename
        SCRIPT = open(subjobn,"w")
        SCRIPT.write(script)
        SCRIPT.close()
        os.chmod(subjobn,stat.S_IRWXU)
        subjobs.append(subjobn)
        if len(subjobs)%options.group==0 or index==len(files)-1:
            jobn = jobdir + "/job_" + str(index/options.group) + ".sh"
            SCRIPT = open(jobn,"w")
            for subjob in subjobs:
                SCRIPT.write(os.path.abspath(subjob) + tmplt_options+"\n")
            subjobs = []
            SCRIPT.close()
        index = index + 1
            


#######################################
#    submit jobs
#######################################

if options.submit:
    if not os.path.isdir(JMworkdir + "/job"):
        print "ERROR: no such directory: " + JMworkdir + "/job"
        print "       try to run with option --create first"
        
        sys.exit()
    if not os.path.isdir(JMworkdir + "/log"):
        os.mkdir(JMworkdir + "/log")

    jobs = glob.glob(JMworkdir + "/job/*")
    jobs.sort()
    index = 0
    for job in jobs:
        if os.path.isdir(job):
            continue
        if not options.njobs is None:
            if index >= options.njobs:
                break
        
        m = re.match(".*/([^\.]*).*$",job)
        filename = m.group(1)

        logpath = os.path.abspath(JMworkdir + "/log") + "/" + filename + ".log"
        epath = os.path.abspath(JMworkdir + "/log") + "/" + filename + ".err"
        jobpath = os.path.abspath(job)
        c = ""
        
        if options.mode=="bsub":
            c = "bsub -q " + options.queue + " -o " +  logpath + " source " + jobpath 
        #    c = c + " -q " + options.queue
        elif options.mode=="qsub":
            c = "qsub -o " + logpath + " -e " + epath + " " + jobpath + " -N " + JMworkdir_short + "/" + filename + " -q localgrid@cream01"
            # added by nadja
            c = c + " -l walltime=" + options.walltime
        # end added by nadja
        print c
        if options.wait > 0:
            time.sleep(options.wait)
        if not options.pretend:
            os.system(c)
        index = index + 1

#######################################
#    check job status
#######################################

if options.status:

    # retrieve status of all jobs
    c = ""
    if options.mode == "bsub":
        c = "bjobs"
    elif options.mode == "qsub":
        c = "qstat -a -u " + os.environ["USER"] + " localgrid@cream01"
    p = subprocess.Popen(c,shell=True,stdout=subprocess.PIPE,stderr=None)
    p.wait()
    (_stdout,_stderr) = p.communicate()

    # compile some regexp to parse the status
    r_wd = re.compile("^.*" + JMworkdir_short + ".*$")
    r_run = None
    r_pen = None
    if options.mode == "bsub":
        r_run = re.compile("^.*RUN.*$")
        r_pen = re.compile("^.*PEN.*$")
    elif options.mode == "qsub":
        r_run = re.compile("^.*\sR\s.*$")
        r_pen = re.compile("^.*\sQ\s.*$")

    # count pending and running jobs
    rjobs = 0
    pjobs = 0    
    for line in _stdout.split("\n"):
        if r_wd.match(line):
            if r_run.match(line):
                rjobs = rjobs + 1
            elif r_pen.match(line):
                pjobs = pjobs + 1
    print "# running  jobs: ",rjobs
    print "# pending  jobs: ",pjobs

    # count finished jobs
    fjobs = 0
    if os.path.isdir(JMworkdir + "/log/"):
        fjobs = len(os.listdir(JMworkdir + "/log/"))
        if options.mode == "qsub":
            fjobs = fjobs/2
    print "# finished jobs: ",fjobs

    # count created jobs
    cjobs = len(glob.glob(JMworkdir + "/job/*.sh"))
    print "# created  jobs: ",cjobs
            
        


        
        
    






