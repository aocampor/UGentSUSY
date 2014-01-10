#include <iostream>   
#include <algorithm>  
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "TColor.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPolyLine.h"
#include "TROOT.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

double f_syst = 0.15;


double calcSoSqrtB(double S, double B){

if (B < 1) B = 1;

      if (S >= 3)
         return S / sqrt(B + f_syst*f_syst * B*B);
      else

    return 0.;
}



int maxbin(TH1D* histo){
  int bi = 0;
  double max = 0;
  for( int i = 1; i <= histo->GetNbinsX(); i++){

    //if( histo->GetBinContent(i) > max && histo->GetBinError(i) < histo->GetBinContent(i) ){
    if( histo->GetBinContent(i) > max ){
      max = histo->GetBinContent(i);
      bi = i;
    }
  }
  return bi;
}



void calcFOM(TString setup, TString MVA, TString dir, int MSTOP, int MLSP ){


	  gStyle->SetOptStat(0);
	  gStyle->SetCanvasColor(0);
	  gStyle->SetPadColor(0);
	  gStyle->SetMarkerStyle(15);
	  gStyle->SetMarkerSize(0.25);
	  gStyle->SetTextFont(42);
	  gStyle->SetMarkerColor(37);


          std::ostringstream ostr1;
          ostr1 << MSTOP; 
          std::string stop = ostr1.str();

          std::ostringstream ostr2;
          ostr2 << MLSP;
          std::string neut = ostr2.str();

          TString ntpdir = "/afs/cern.ch/work/s/sigamani/public/CMSSW_5_3_7_STOP/src/babyReaderSTOPS/runAnalysis/ntuples2/";

          TFile ttbar(ntpdir+"/"+setup+"/"+dir+"/ttbar/output/ttbar_all_0.root");
          TFile wjets(ntpdir+"/"+setup+"/"+dir+"/wjets/output/wjets_all_0.root");
          TFile others(ntpdir+"/"+setup+"/"+dir+"/others/output/others_all_0.root");
          TFile sig(ntpdir+"/"+setup+"/"+dir+"/"+TString(stop)+"/"+TString(neut)+"/output/"+dir+"_0.root");
//        TFile sig("../Leg3_NN_Strass/"+setup+"/"+TString(stop)+"/"+TString(neut)+"/output/SMS_t2tt_0.root");  



	  TH1D* TTBar= (TH1D*)ttbar.Get(MVA);
	  TH1D* WJets= (TH1D*)wjets.Get(MVA);
	  TH1D* Others= (TH1D*)others.Get(MVA);
	  TH1D* signal= (TH1D*)sig.Get(MVA);


	  TTBar->SetFillColor(40);
	  TTBar->SetFillStyle(3001);
	  TTBar->SetLineWidth(0.);
	  signal->SetLineColor(kRed);
	  signal->SetLineWidth(2.);


    	  WJets->SetFillColor(41);
    	  Others->SetFillColor(43);

          THStack *stack= new THStack("stack", "");
          stack->Add(WJets);
          stack->Add(TTBar);
          stack->Add(Others);


          int nbins = TTBar->GetNbinsX();
          double hmin = TTBar->GetXaxis()->GetBinLowEdge(1);
          double hmax = TTBar->GetXaxis()->GetBinUpEdge(nbins);


          TH1D* SoB = new TH1D("","",nbins,hmin,hmax);
          int max_bin;
          double cutvalue; 
	  double nsignal = signal->Integral();




          SoB->SetLineColor(kBlue);
          SoB->SetLineWidth(2.);




	      for(int b=1; b<=nbins; ++b){

	      //cout << "Bkg: " << TTBar->Integral(b,nbins+1) << endl;
	      //cout << "Sig: " << signal->Integral(b,nbins+1) << endl;

	      double sig_ = signal->Integral(b,nbins+1);
	      double bkg_ = TTBar->Integral(b,nbins+1) + WJets->Integral(b,nbins+1) + Others->Integral(b,nbins+1);
	      
	      double SoSqrtB = calcSoSqrtB(sig_, bkg_ );

	      //cout << "s/root(B) " << SoSqrtB<< endl;

	      SoB->SetBinContent(b,SoSqrtB);

	      }


//      cout << "TT " << TTBar->Integral()<< endl;
//      cout << "W " << WJets->Integral()<< endl;
//      cout << "signal " << signal->Integral()<< endl;


	   max_bin = maxbin(SoB);
	   cutvalue = TTBar->GetBinLowEdge(max_bin);


	  /*	
	   cout << "********" << endl;
	   cout << "MaxBin: " << max_bin << endl;
	   cout << "CutVal: " << cutvalue << endl;
           cout << "SoB: " << SoB->GetBinContent(max_bin)  << endl;
	   cout << "********" << endl;
	  */
 
	   double sob = SoB->GetBinContent(max_bin);

       	   char cutval_[32];
	   snprintf(cutval_, 32, "%.1g", cutvalue);
       	   char fom_[32];
	   snprintf(fom_, 32, "%.2g", sob);
       	   char signal_[32];
	   snprintf(signal_, 32, "%.2g", nsignal);

           TLegendEntry *legge;
           TLegend *leg;
           leg = new TLegend(0.6,0.55,0.9,0.85);
           leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.043);
           legge = leg->AddEntry(TTBar,   "t#bar{t} + Jets", "F");
           legge = leg->AddEntry(WJets,   "W + Jets", "F");
           legge = leg->AddEntry(Others,   "Others", "F");
           legge = leg->AddEntry(signal, "sig("+TString(stop)+","+TString(neut)+")", "l");
           leg->SetFillColor(0);

 
  /*         TCanvas c1("c1","c1",800,600);
	   SoB->Draw("");
	   SoB->GetYaxis()->SetTitle("FOM");
	   SoB->GetXaxis()->SetTitle("S/#sqrt{B + (f#timesB)}^{2}");
           c1.Print("~/www/SoB.png");
*/

           TCanvas c2("c2","c2",800,600);
	   stack->Draw("");
	   signal->Draw("same");
	   stack->GetYaxis()->SetTitle("Entries");
	   stack->GetXaxis()->SetTitle("BDT output");
           leg->Draw();
           TLatex l1;
           l1.SetTextAlign(12);
           l1.SetTextSize(0.04);
           l1.SetNDC();
           l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
           l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");

           TLatex l2;
           l2.SetTextAlign(12);
           l2.SetTextSize(0.04);
           l2.SetNDC();
           l2.DrawLatex(0.22, 0.8, "#color[2]{Optimal cut: \t "+TString(cutval_)+"}");
           l2.DrawLatex(0.22, 0.75, "#color[2]{FOM@cut: \t "+TString(fom_)+"}");
           l2.DrawLatex(0.22, 0.7, "#color[2]{Tot. N_{Signal}: \t "+TString(signal_)+"}");
           c2.SetLogy(); 
           c2.Print("~/www/STOP/NNTraining/8TeV/Plots13/t2bw-025/"+TString(MVA)+"/histo_"+TString(stop)+"_"+TString(neut)+".png");
           //c2.Print("~/www/STOP/NNTraining/8TeV/Plots12/t2tt/"+TString(MVA)+"/histo_"+TString(stop)+"_"+TString(neut)+".png");


}


double FOM(TString setup, TString BDT, TString dir, int MSTOP, int MLSP, int num){


	  gStyle->SetOptStat(0);
	  gStyle->SetCanvasColor(0);
	  gStyle->SetPadColor(0);
	  gStyle->SetMarkerStyle(15);
	  gStyle->SetMarkerSize(0.25);
	  gStyle->SetTextFont(42);
	  gStyle->SetMarkerColor(37);

	  
          std::ostringstream ostr1;
          ostr1 << MSTOP; 
          std::string stop = ostr1.str();

          std::ostringstream ostr2;
          ostr2 << MLSP;
          std::string neut = ostr2.str();




	  TString ntpdir = "/afs/cern.ch/work/s/sigamani/public/CMSSW_5_3_7_STOP/src/babyReaderSTOPS/runAnalysis/ntuples2/";

	  TFile ttbar(ntpdir+"/"+setup+"/"+dir+"/ttbar/output/ttbar_all_0.root");
	  TFile wjets(ntpdir+"/"+setup+"/"+dir+"/wjets/output/wjets_all_0.root");
	  TFile others(ntpdir+"/"+setup+"/"+dir+"/others/output/others_all_0.root");
	  TFile sig(ntpdir+"/"+setup+"/"+dir+"/"+TString(stop)+"/"+TString(neut)+"/output/"+dir+"_0.root"); 	
//	  TFile sig("../Leg3_NN_Strass/"+setup+"/"+TString(stop)+"/"+TString(neut)+"/output/SMS_t2tt_0.root"); 	


	  TH1D* TTBar= (TH1D*)ttbar.Get(BDT);
	  TH1D* WJets= (TH1D*)wjets.Get(BDT);
	  TH1D* Others= (TH1D*)others.Get(BDT);
	  TH1D* signal= (TH1D*)sig.Get(BDT);


          int nbins = TTBar->GetNbinsX();
          double hmin = TTBar->GetXaxis()->GetBinLowEdge(1);
          double hmax = TTBar->GetXaxis()->GetBinUpEdge(nbins);


          TH1D* FOM = new TH1D("","",nbins,hmin,hmax);
          int max_bin;
          double cutvalue; 
	  double nsignal = signal->Integral();


	      for(int b=1; b<=nbins; ++b){

	      double sig_ = signal->Integral(b,nbins+1);
	      double bkg_ = TTBar->Integral(b,nbins+1) + WJets->Integral(b,nbins+1) + Others->Integral(b,nbins+1) ;
	      
	      double SoSqrtB = calcSoSqrtB(sig_, bkg_ );

	      FOM->SetBinContent(b,SoSqrtB);

	      }


	    max_bin = maxbin(FOM);
	    cutvalue = TTBar->GetBinLowEdge(max_bin);
            double fom = FOM->GetBinContent(max_bin);


   if (num==0) return nsignal;
   if (num==1) return fom;
   if (num==2) return cutvalue;
}


void make2Dplot(TString setup, TString BDT, int num){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);
          if (num <3) {gStyle->SetPaintTextFormat("4.1f");}

                             /*  TFile file("../Leg3_NN_Strass/mapT2tt.root");
                               TH2D* hist1 = (TH2D*)file.Get("mStopVSmNeutralino");
                               int nbinsx = hist1->GetNbinsX();
                               double hminx = hist1->GetXaxis()->GetBinLowEdge(1);
                               double hmaxx = hist1->GetXaxis()->GetBinUpEdge(nbinsx);
                               int nbinsy = hist1->GetNbinsX();
                               double hminy = hist1->GetXaxis()->GetBinLowEdge(1);
                               double hmaxy = hist1->GetXaxis()->GetBinUpEdge(nbinsy);

                               TH2D* TwoDPlot = new TH2D("","",nbinsx,hminx,hmaxx,nbinsy,hminy,hmaxy);
				*/

              TH2D* TwoDPlot = new TH2D("","",26,157.5, 812.5, 28, 12.5,712.5);

 	 
              for(int x=175; x<=800; x+=25){
	
    	               for(int y=25; y<=700; y+=25){
			       if (x - y > 99){ 
				TwoDPlot->Fill(x,y,FOM(setup,BDT,x,y,num));

			       }	
              }
	   }

           TCanvas c1("c1","c1",800,600);
	   c1.SetLeftMargin(0.1706731);
	   c1.SetRightMargin(0.1983173);
	   c1.SetTopMargin(0.04895105);
	   c1.SetBottomMargin(0.1416084);
	   c1.Range(-289.7381,-191.8196,1334.643,1074.487);
           TwoDPlot->SetMarkerSize(1.);
           TwoDPlot->SetMarkerColor(kWhite);
           TwoDPlot->Draw("COLZ TEXT");
           TwoDPlot->GetYaxis()->SetTitle("LSP mass");
           TwoDPlot->GetXaxis()->SetTitle("Stop mass");
           TwoDPlot->GetZaxis()->SetTitle("Entries");
           TwoDPlot->GetZaxis()->SetTitleOffset(1.1);
           if (num==0) {TwoDPlot->GetZaxis()->SetTitle("Entries"); TwoDPlot->GetZaxis()->SetRangeUser(0,5000);}
           if (num==1) {TwoDPlot->GetZaxis()->SetTitle("FOM"); TwoDPlot->GetZaxis()->SetRangeUser(0,15);}
           if (num==2) {TwoDPlot->GetZaxis()->SetTitle("Cutval"); TwoDPlot->GetZaxis()->SetRangeUser(0,1);}
           TLatex l1;
           l1.SetTextAlign(12);
           l1.SetTextSize(0.04);
           l1.SetNDC();
           l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
           l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
           c1.Print("~/www/STOP/NNTraining/8TeV/Plots13/t2bw-025/"+BDT+""/"FOM.png");

}



void make2Dplot_MAX(TString setup, TString dir, int num){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);
	  if (num <3) {gStyle->SetPaintTextFormat("4.1f");}


              TH2D* TwoDPlot = new TH2D("","",26,157.5, 812.5, 28, 12.5,712.5);

 	 
              for(int x=175; x<=800; x+=25){
           //   for(int x=175; x<=225; x+=25){

	
    	               for(int y=25; y<=700; y+=25){
    	       //       for(int y=25; y<=100; y+=25){
			       if (x - y > 99){ 
 
					if (num <3){
					double array[6] = {
							   FOM(setup,"BDT1",dir,x,y,num), 
							   FOM(setup,"BDT2",dir,x,y,num), 
							   FOM(setup,"BDT3",dir,x,y,num), 
							   FOM(setup,"BDT4",dir,x,y,num), 
							   FOM(setup,"BDT5",dir,x,y,num), 
							   FOM(setup,"BDT6",dir,x,y,num)
							   };
					} else 
					                                        
					double array[6] = {
                                                           FOM(setup,"BDT1",dir,x,y,1),
                                                           FOM(setup,"BDT2",dir,x,y,1),
                                                           FOM(setup,"BDT3",dir,x,y,1),
                                                           FOM(setup,"BDT4",dir,x,y,1),
                                                           FOM(setup,"BDT5",dir,x,y,1),
                                                           FOM(setup,"BDT6",dir,x,y,1)
                                                           };


					double temp = 0.;
					int mvaval = 0;

						// Get the maximum of each point for all MVAs
						  for(int i=0;i<6;i++){

							if(array[i]>temp){ 

								temp	= array[i]; 
								mvaval 	= i; 
							//	cout << "i: "<< mvaval << endl; 
							//	cout << "val: "<< temp << endl; 
							}  
						  }
			
	
								if (num <3) {TwoDPlot->Fill(x,y,temp);}
								else TwoDPlot->Fill(x,y,mvaval+1);
								//	createTableCLs(x,y,temp);

			       }	
              	        }
	   }


           TCanvas c1("c1","c1",800,600);
	   c1.SetLeftMargin(0.1706731);
	   c1.SetRightMargin(0.1983173);
	   c1.SetTopMargin(0.04895105);
	   c1.SetBottomMargin(0.1416084);
	   c1.Range(-289.7381,-191.8196,1334.643,1074.487);
           TwoDPlot->SetMarkerSize(1.); 
           TwoDPlot->SetMarkerColor(kWhite); 
           TwoDPlot->Draw("COLZ TEXT");
/*	   TLine lm(125, 0 , 800, 675);
	   TLine lp(175, 0 , 800, 625);
	   TLine imm(225, 0 , 800, 575);
	   TLine imp(275, 0 , 800, 525);
	   TLine iimm(325, 0 , 800, 475);
	   TLine iimp(375, 0 , 800, 425);
	   TLine ijmm(425, 0 , 800, 375);
	   TLine ijmp(475, 0 , 800, 325);
	   TLine hm(525, 0 , 800, 275);
	   TLine hp(575, 0 , 800, 225);
	   TLine hmm(625, 0 , 800, 175);
	   TLine hmp(675, 0 , 800, 125);
	   lm.SetLineColor(kBlack); lm.SetLineWidth(2); lm.SetLineStyle(2); lm.Draw();
	   lp.SetLineColor(kBlack); lp.SetLineWidth(2); lp.SetLineStyle(2); lp.Draw();
	   imm.SetLineColor(kBlack); imm.SetLineWidth(2); imm.SetLineStyle(2); imm.Draw();
	   imp.SetLineColor(kBlack); imp.SetLineWidth(2); imp.SetLineStyle(2); imp.Draw();
	   iimm.SetLineColor(kBlack); iimm.SetLineWidth(2); iimm.SetLineStyle(2); iimm.Draw();
	   iimp.SetLineColor(kBlack); iimp.SetLineWidth(2); iimp.SetLineStyle(2); iimp.Draw();
	   ijmm.SetLineColor(kBlack); ijmm.SetLineWidth(2); ijmm.SetLineStyle(2); ijmm.Draw();
	   ijmp.SetLineColor(kBlack); ijmp.SetLineWidth(2); ijmp.SetLineStyle(2); ijmp.Draw();
	   hm.SetLineColor(kBlack); hm.SetLineWidth(2); hm.SetLineStyle(2); hm.Draw();
	   hp.SetLineColor(kBlack); hp.SetLineWidth(2); hp.SetLineStyle(2); hp.Draw();
	   hmm.SetLineColor(kBlack); hmm.SetLineWidth(2); hmm.SetLineStyle(2); hmm.Draw();
	   hmp.SetLineColor(kBlack); hmp.SetLineWidth(2); hmp.SetLineStyle(2); hmp.Draw();
*/ 
           TwoDPlot->GetYaxis()->SetTitle("LSP mass"); 
           TwoDPlot->GetXaxis()->SetTitle("Stop mass");  
           if (num==0) {TwoDPlot->GetZaxis()->SetTitle("Entries"); TwoDPlot->GetZaxis()->SetRangeUser(0,5000);}
           if (num==1) {TwoDPlot->GetZaxis()->SetTitle("FOM"); TwoDPlot->GetZaxis()->SetRangeUser(0,15);}
           if (num==2) {TwoDPlot->GetZaxis()->SetTitle("Optimal cut point"); TwoDPlot->GetZaxis()->SetRangeUser(0,1);}
           if (num==3) {TwoDPlot->GetZaxis()->SetTitle("Best performing BDT region"); TwoDPlot->GetZaxis()->SetRangeUser(0,6);}
           TLatex l1;
           l1.SetTextAlign(12);
           l1.SetTextSize(0.04);
           l1.SetNDC();
           l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
           l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
           l1.SetTextSize(0.03);
	   if (num==3){
 
  /*         l1.DrawLatex(0.22, 0.85, "BDT2: #DeltaM = 250 #pm 25 GeV");
           l1.DrawLatex(0.22, 0.80, "BDT3: #DeltaM = 350 #pm 25 GeV");
           l1.DrawLatex(0.22, 0.75, "BDT4: #DeltaM = 450 #pm 25 GeV");
           l1.DrawLatex(0.22, 0.70, "BDT5: #DeltaM = 550 #pm 25 GeV");
           l1.DrawLatex(0.22, 0.65, "BDT6: #DeltaM = 650 #pm 25 GeV");
*/

      tex = new TLatex(0.7449749,0.8251748,"BDT1");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.7449749,0.7097902,"BDT2");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.7437186,0.5891608,"BDT3");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.7424623,0.4755245,"BDT4");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.7424623,0.3583916,"BDT5");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.7424623,0.2447552,"BDT6");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();


	   }
           c1.Print("~/www/test.png");

}






void make2d(){


make2Dplot("setup_2", "MVA1", 1);
make2Dplot("setup_2", "MVA2", 1);
make2Dplot("setup_2", "MVA3", 1);
make2Dplot("setup_2", "MVA4", 1);
make2Dplot("setup_2", "MVA5", 1);
make2Dplot("setup_2", "MVA6", 1);


/*make2Dplot("setup_2", "MVA1", 2);
make2Dplot("setup_2", "MVA2", 2);
make2Dplot("setup_2", "MVA3", 2);
make2Dplot("setup_2", "MVA4", 2);
make2Dplot("setup_2", "MVA5", 2);
make2Dplot("setup_2", "MVA6", 2);
*/
}


void make1dhistos(TString BDT){

              for(int x=175; x<=800; x+=25){

                       for(int y=25; y<=700; y+=25){
                               
				if (x - y > 99){
				calcFOM("setup_0",BDT,"050",x,y);
				}
			}
		}
}



void createTableCLs(int S, int N, double signal, double ttbar){
 
  char datacardname[100];
  sprintf(datacardname,"CLsS%dN%d.txt",S,N);
 
  ofstream  tablesFile(datacardname);
  tablesFile.setf(ios::fixed);
  tablesFile.precision(1);
 
  tablesFile << "imax 1  number of channels" << endl
             << "jmax 2  number of backgrounds" << endl
             << "kmax 4  number of nuisance parameters (sources of systematical uncertainties)" << endl
             << "------------"<<endl
             << "bin 1"<<endl    
  //           << "observation \t" << fillTable(map_yields_mu,"Data",cutname)+fillTable(map_yields_el,"Data",cutname) << endl
             << "------------" << endl
             << "bin            \t 1              \t 1              \t 1" << endl
             << "process        \t signal         \t TTJets         \t others" << endl
             << "process        \t 0              \t 1              \t 2" << endl
             << "rate           \t " << signal << "  \t \t \t "
  //           << fillTable(map_yields_mu,"TTbar",cutname) + fillTable(map_yields_el,"TTbar",cutname) <<  " \t \t \t "
  //           << fillTable(map_yields_mu,"others",cutname) + fillTable(map_yields_el,"others",cutname)<< endl
             << "------------" << endl
             << "lumi    \t lnN \t 1.022        \t 1.022          \t 1.022         \t  lumi affects both signal and mc. lnN = lognormal" << endl
             << "eff_unc \t lnN \t 1.3          \t -              \t -             \t  stop cross section + signal efficiency + other minor ones." << endl
             << "stat_unc \t lnN \t -             \t 1.045          \t 1.8           \t  4.5\% and 80\% stat uncertainty on TTJets and others" << endl
             << "bg_others \t lnN \t -            \t -              \t 1.30          \t  30\% uncertainty on the rest of the backgrounds" << endl;
 
  tablesFile.close();
 
}
