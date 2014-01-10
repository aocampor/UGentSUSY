#!/bin/csh


if ($argv[1] == 0) then

cd Reg1_T2bw075/ 
python ../makeconfigs.py

cd ../Reg2_T2bw075/ 
python ../makeconfigs.py

cd ../Reg3_T2bw075/ 
python ../makeconfigs.py

cd ../Reg4_T2bw075/ 
python ../makeconfigs.py

cd ../Reg5_T2bw075/ 
python ../makeconfigs.py

cd ../Reg6_T2bw075/ 
python ../makeconfigs.py

cd ../Reg1_T2bw050/ 
python ../makeconfigs.py

cd ../Reg2_T2bw050/ 
python ../makeconfigs.py

cd ../Reg3_T2bw050/ 
python ../makeconfigs.py

cd ../Reg4_T2bw050/ 
python ../makeconfigs.py

cd ../Reg5_T2bw050/ 
python ../makeconfigs.py

cd ../Reg6_T2bw050/ 
python ../makeconfigs.py

cd ../Reg1_T2bw025/ 
python ../makeconfigs.py

cd ../Reg2_T2bw025/ 
python ../makeconfigs.py

cd ../Reg3_T2bw025/ 
python ../makeconfigs.py

cd ../Reg4_T2bw025/ 
python ../makeconfigs.py

cd ../Reg5_T2bw025/ 
python ../makeconfigs.py

cd ../Reg6_T2bw025/ 
python ../makeconfigs.py

cd ..

else


cd Reg1_T2bw075/ 
rm -rf JM*
source ../make.sh

cd ../Reg2_T2bw075/
rm -rf JM*
source ../make.sh

cd ../Reg3_T2bw075/
rm -rf JM*
source ../make.sh

cd ../Reg4_T2bw075/
rm -rf JM*
source ../make.sh

cd ../Reg5_T2bw075/
rm -rf JM*
source ../make.sh

cd ../Reg6_T2bw075/
rm -rf JM*
source ../make.sh

cd ../Reg1_T2bw050/ 
rm -rf JM*
source ../make.sh

cd ../Reg2_T2bw050/
rm -rf JM*
source ../make.sh

cd ../Reg3_T2bw050/
rm -rf JM*
source ../make.sh

cd ../Reg4_T2bw050/
rm -rf JM*
source ../make.sh

cd ../Reg5_T2bw050/
rm -rf JM*
source ../make.sh

cd ../Reg6_T2bw050/
rm -rf JM*
source ../make.sh


cd ../Reg1_T2bw025/ 
rm -rf JM*
source ../make.sh

cd ../Reg2_T2bw025/
rm -rf JM*
source ../make.sh

cd ../Reg3_T2bw025/
rm -rf JM*
source ../make.sh

cd ../Reg4_T2bw025/
rm -rf JM*
source ../make.sh

cd ../Reg5_T2bw025/
rm -rf JM*
source ../make.sh

cd ../Reg6_T2bw025/
rm -rf JM*
source ../make.sh

cd ..

endif
