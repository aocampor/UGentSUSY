// C / C++ headers
#include <cmath>
#include <iostream>
using namespace std;

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

// Define format and input file
#include "Reader_beta1002.h" 


bool deltaM(double, double, double, double, double, double);


void help(int argc, char* argv[]) {
  cout << "usage: \n"
       << "./babyReader infile.root outfile.root <deltaM low> <deltaM high> <mSTOP low> <mSTOP high>" << endl;
       exit(0);
}

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{

  // ################################
  // ##       Open the tree        ##
  // ################################

     
   if( argc <7  ) help(argc,argv);   

   //TFile *fin = TFile::Open("root://eoscms//eos/cms/store/group/phys_susy/StopBabies/V00-02/T2bw-025.root"); 
   //TTree* theInputTree = (TTree*) fin->Get("babyTuple"); 
   //TFile *fout = new TFile("out.root","recreate");


   TFile *fin = TFile::Open(argv[1]);
   TTree* theInputTree = (TTree*) fin->Get("babyTuple"); 
   TFile *fout = new TFile(argv[2],"recreate");
   
   Float_t met;
   Float_t mT;
   Float_t mT2W;
   Float_t HTfrac;
   Float_t b1_pt;
   Float_t lepton_pT;
   Float_t dPhi_JetMet;
   Float_t dR_LepB;
   Float_t m3b;
   Float_t mlb_hemi;
   Float_t jet1_pT;
   Float_t njets;
   Float_t METoverSqrtHT;
   Float_t dR_LepJet;
   Float_t HT_MET_lep_pt;
   Float_t Chi2SNT; 
   Float_t stop_mass;
   Float_t lsp_mass;
   
   TTree *MVAtree     = new TTree("MVAtree","Tree of variables");
   MVAtree->Branch("met", &met, "met/F");
   MVAtree->Branch("mT", &mT, "mT/F");
   MVAtree->Branch("mT2W", &mT2W, "mT2W/F");
   MVAtree->Branch("HTfrac", &HTfrac, "HTfrac/F");
   MVAtree->Branch("b1_pt", &b1_pt, "b1_pt/F");
   MVAtree->Branch("lepton_pT", &lepton_pT, "lepton_pT/F");
   MVAtree->Branch("dPhi_JetMet", &dPhi_JetMet, "dPhi_JetMet/F");
   MVAtree->Branch("dR_LepB", &dR_LepB, "dR_LepB/F");
   MVAtree->Branch("m3b", &m3b, "m3b/F");
   MVAtree->Branch("mlb_hemi", &mlb_hemi, "mlb_hemi/F");
   MVAtree->Branch("jet1_pT", &jet1_pT, "jet1_pT/F");
   MVAtree->Branch("njets", &njets, "njets/F");
   MVAtree->Branch("METoverSqrtHT", &METoverSqrtHT, "METoverSqrtHT/F");
   MVAtree->Branch("dR_LepJet", &dR_LepJet, "dR_LepJet/F");
   MVAtree->Branch("HT_MET_lep_pt", &HT_MET_lep_pt, "HT_MET_lep_pt/F");
   MVAtree->Branch("Chi2SNT", &Chi2SNT, "Chi2SNT/F");
   MVAtree->Branch("stop_mass", &stop_mass, "stop_mass/F");
   MVAtree->Branch("lsp_mass", &lsp_mass, "lsp_mass/F");

   babyEvent myEvent;
   intermediatePointers pointers;
   InitializeBranches(theInputTree,&myEvent,&pointers);
  

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
      ReadEvent(theInputTree,i,&pointers,&myEvent);

      
  //    cout << endl;
  //    cout << " index = " << i << endl;

	// Electron triggers
	if ((abs(myEvent.leadingLeptonPDGId) == 11) && (myEvent.triggerElec ==false)) continue;
	// Muon triggers

	if (abs(myEvent.leadingLeptonPDGId) == 13)
	{
		if ((myEvent.leadingLepton.Pt() < 25) && (myEvent.xtriggerMuon ==false)) continue;
		else if ((myEvent.leadingLepton.Pt() > 25) && (myEvent.triggerMuon ==false)) continue;
	}

	if (myEvent.isolatedTrackVeto == false ) continue;
	if (myEvent.tauVeto == false) continue;

        if (myEvent.MET < 80) continue;
        if (myEvent.MT < 100) continue;
        if (myEvent.nBTag < 1) continue;
        if (myEvent.nJets < 4) continue;
        if (myEvent.numberOfLepton != 1) continue;

	if ((int) myEvent.mNeutralino % 25 != 0) continue;
	if ((int) myEvent.mStop % 25 != 0) continue;
	if (myEvent.mNeutralino - (int) myEvent.mNeutralino > 0.00001) continue;
	if (myEvent.mStop       - (int) myEvent.mStop       > 0.00001) continue;
        if ((int) myEvent.mNeutralino == 0) continue;
        if ( deltaM( myEvent.mStop, myEvent.mNeutralino, atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6])) == 0) continue; 



//       cout << " mstop: " <<  myEvent.mStop << endl;
//       cout << " mNeut: " <<  myEvent.mNeutralino << endl;
      
        met = myEvent.MET; 
        mT = myEvent.MT;
	mT2W = myEvent.MT2W;
	HTfrac = myEvent.HTRatio;
	b1_pt = myEvent.leadingBPt;
	lepton_pT = myEvent.leadingLeptonPt;
	dPhi_JetMet = myEvent.deltaPhiMETJets;
	dR_LepB = myEvent.deltaRLeptonLeadingB;
	m3b = myEvent.M3b;
	mlb_hemi = myEvent.Mlb_hemi;
	jet1_pT = myEvent.leadingJetPt;
	njets = myEvent.nJets;
	METoverSqrtHT = myEvent.METoverSqrtHT;
	//dR_LepJet 
	HT_MET_lep_pt = myEvent.HTPlusLeptonPtPlusMET;
	Chi2SNT = myEvent.hadronicChi2;

        stop_mass = myEvent.mStop;
        lsp_mass = myEvent.mNeutralino; 

  /*    cout << " +---------+ " << endl;
      cout << " | Leptons | " << endl;
      cout << " +---------+ " << endl;

      cout << " number of selected leptons : " << myEvent.numberOfLepton << endl;
      cout << " leading lepton Pt : " << myEvent.leadingLepton.Pt() << endl;

      cout << " +------+ " << endl;
      cout << " | Jets | " << endl;
      cout << " +------+ " << endl;

      cout << " number of selected jets : " << myEvent.nJets << endl; 
      cout << " number of selected b-tagged jets : " << myEvent.nBTag << endl;
      for (unsigned int j = 0 ; j < myEvent.jets.size() ; j++)
      {
          cout << " jet n°"   << j 
               << " ; pT = "  << myEvent.jets[j].Pt() 
               << " ; eta = " << myEvent.jets[j].Eta() 
               << endl;
      }

      cout << " +------------------------+ " << endl;
      cout << " | 'High-level' variables | " << endl;
      cout << " +------------------------+ " << endl;

      cout << " MET = " << myEvent.MET << endl;
      cout << " MT = " << myEvent.MT << endl;
   
      cout << " +-----------------------------+ " << endl;
      cout << " | Generator-level information | " << endl;
      cout << " +-----------------------------+ " << endl;

      cout << " number of generated leptons (tau and e/µ not coming from a tau) : " << myEvent.numberOfGenLepton << endl;
      
      for (unsigned int j = 0 ; j < myEvent.genParticles.size() ; j++)
      {
          cout << " genParticles n° " << j 
              << " ; PDGId : " << myEvent.genParticlesPDGId[j]
              << " ; Pt : "    << myEvent.genParticles[j].Pt() 
              << " ; Eta : "   << myEvent.genParticles[j].Eta() 
              << endl;
      }

      cout << endl;
 */
      
      MVAtree->Fill();

  } 

  
      MVAtree->Write();
      fout->Write();
      fout->Close();
  return (0);
}


bool deltaM( double mstop, double lsp, double range1, double range2, double x1cutoff, double x2cutoff){
 // function to return a specific delta M range 
  bool deltam = false;
  double deltaM = mstop - lsp;

  if ( ((deltaM > range1 ) && (deltaM < range2 )) && (mstop > x1cutoff)   &&  (mstop < x2cutoff) ) {

      deltam = true;

  }

 return deltam;
}
