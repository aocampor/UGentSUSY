#python ../makeconfigs.py
../JM.py "setup/setup_12.cfg" ../JM_NN.py --create
python ../JM.py --submit --mode bsub --queue 8nh
