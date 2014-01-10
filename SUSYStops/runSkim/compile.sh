#!/bin/bash

SOURCE1=MakeBDTtree_signal.C 
SOURCE2=MakeBDTtree_bkg.C
EXECUTABLE1=`echo $SOURCE1 | tr '.' ' ' | awk '{print $1}'`
EXECUTABLE2=`echo $SOURCE2 | tr '.' ' ' | awk '{print $1}'`


g++  -l:$PWD/vectorDictionnary_C.so                       \
     -I `root-config --incdir`                            \
     -L $ROOTSYS/lib                                      \
     -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf  \
     -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix  \
     -lPhysics -lMathCore -lThread                        \
     -pthread -lm -ldl -rdynamic                          \
     $SOURCE1 -o $EXECUTABLE1                                                    


g++  -l:$PWD/vectorDictionnary_C.so                       \
     -I `root-config --incdir`                            \
     -L $ROOTSYS/lib                                      \
     -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf  \
     -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix  \
     -lPhysics -lMathCore -lThread                        \
     -pthread -lm -ldl -rdynamic                          \
     $SOURCE2 -o $EXECUTABLE2                                                    
