#!/bin/bash

SOURCE=runAnalysis.C
EXECUTABLE=`echo $SOURCE | tr '.' ' ' | awk '{print $1}'`




g++  -l:$PWD/vectorDictionnary_C.so                       \
     -I `root-config --incdir`                            \
     -L $ROOTSYS/lib                                      \
     -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf  \
     -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix  \
     -lPhysics -lMathCore -lThread -lTMVA                 \
     -pthread -lm -ldl -rdynamic                          \
     $SOURCE -o $EXECUTABLE                                                    

