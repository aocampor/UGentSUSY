#!/bin/csh


set SDname = $1 
set setup = $2
#set var = "met,mT2W,HTfrac,b1_pt,dPhi_JetMet,dR_LepB"
#set var = "met,lepton_pT,mT2W,HTfrac,b1_pt,dPhi_JetMet,dR_LepB"
set var = "met,mT2W,HTfrac,dPhi_JetMet,Chi2SNT" 
#set var = "met,mT2W,HTfrac,dPhi_JetMet,b1_pt"



foreach i ( `seq 175 25 499` )


   
	foreach j ( `seq 25 25 700` )

		if ($i - $j >= 100) then 
		#python sendOnBatch.py t2bw $SDname 2nd runAnalysis $var $setup "T2bw"$SDname $i $j  
		python sendOnBatch.py t2tt all 8nh runAnalysis $var $setup "T2tt" $i $j 
		endif
  	end
end


sleep 1h

foreach i ( `seq 500 25 800` )


   
	foreach j ( `seq 25 25 700` )

		if ($i - $j >= 100) then 
		#python sendOnBatch.py t2bw $SDname 2nd runAnalysis $var $setup "T2bw"$SDname $i $j  
                python sendOnBatch.py t2tt all 8nh runAnalysis $var $setup "T2tt" $i $j
		endif
  	end
end


python sendOnBatch.py ttbar all 2nd runAnalysis $var $setup "T2tt"
python sendOnBatch.py wjets all 2nd runAnalysis $var $setup "T2tt"
python sendOnBatch.py others all 2nd runAnalysis $var $setup "T2tt"

#python sendOnBatch.py ttbar all 2nd runAnalysis $var $setup "T2bw"$SDname
#python sendOnBatch.py wjets all 2nd runAnalysis $var $setup "T2bw"$SDname
#python sendOnBatch.py others all 2nd runAnalysis $var $setup "T2bw"$SDname
