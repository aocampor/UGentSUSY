#include "TColor.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TF2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPolyLine.h"
#include "TROOT.h"
#include "TF1.h"
#include "TGaxis.h"


#include <iomanip>
#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <utility>




using namespace std;
void rootlogon();


TString savedir = "~/www/";
TString indir = "/afs/cern.ch/work/s/sigamani/public/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit/"; 



/*


TGraph* plot_limit_p(TString &dir){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);


  //palette settings - completely independent
  const Int_t NRGBs = 6;
  const Int_t NCont = 999;

  Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.99, 0.0, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.0, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.99, 0.0, 1.00, 0.12, 0.00, 0.00 };


  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  gStyle->SetOptStat(0);


   double shell;

   
   if (dir == "T2bw025") {dataset_name = "t2bw025"; shell = 80. / 0.25;}
   if (dir == "T2bw050") {dataset_name = "t2bw050"; shell = 80. / 0.50;}
   if (dir == "T2bw075") {dataset_name = "t2bw075"; shell = 80. / 0.75;}
   if (dir == "T2tt")    {dataset_name = "t2tt";    shell = 173.;}



   TH2D *SMS = new TH2D("SMS","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_m = new TH2D("SMS_m","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_p = new TH2D("SMS_p","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_obs = new TH2D("SMS_obs","",26,157.5, 812.5, 28, 12.5,712.5);


              for(int x=175; x<=800; x+=25){


                      for(int y=25; y<=700; y+=25){


  		  char filename[500];
	          long begin,end;



                  sprintf(filename,"/afs/cern.ch/work/s/sigamani/public/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit/%s/higgsCombineS%d-N%d.Asymptotic.mH120.root", dataset_name, x, y);
                  ifstream ifile(filename);
	          if (!ifile.good()) continue; 

		  TFile* file  = new TFile(filename, "READ");  
		  char* name = (char*)file->GetName();



TTree *limittree = (TTree*)file->Get("limit");


double high_val = 100.;


TH1F* obs = new TH1F("obs","",100,0,high_val);
limittree->Draw("limit>>obs", "quantileExpected==-1");
TH1F* expm2 = new TH1F("expm2","",100,0,high_val);
limittree->Draw("limit>>expm2", "quantileExpected>0.02 && quantileExpected<0.03");
TH1F* expm1 = new TH1F("expm1","",100,0,high_val);
limittree->Draw("limit>>expm1", "quantileExpected>0.15 && quantileExpected<0.16");
TH1F* exp = new TH1F("exp","",100,0,high_val);
limittree->Draw("limit>>exp", "quantileExpected==0.5");
TH1F* expp1 = new TH1F("expp1","",100,0,high_val);
limittree->Draw("limit>>expp1", "quantileExpected>0.83 && quantileExpected<0.84");
TH1F* expp2 = new TH1F("expp2","",100,0,high_val);
limittree->Draw("limit>>expp2", "quantileExpected>0.97 && quantileExpected<0.98");

//cout << "limit: " << exp->GetMean() << endl;



   if (expp1->GetMean() < 0.99) { SMS->Fill(x,y, expp1->GetMean());} 

   //if (exp->GetMean() < 1.) { cout << "x: "<< x << ", y: "<< y << endl; }



   file->Close();

		}
	  }



   Double_t contours[3];
   contours[0] =  0.1;
   contours[1] =  0.99;
   contours[2] =  1.00;
   Double_t contours_m[3];
   contours_m[0] =  0.1;
   contours_m[1] =  0.99;
   contours_m[2] =  1.00;
   Double_t contours_p[3];
   contours_p[0] =  0.1;
   contours_p[1] =  0.99;
   contours_p[2] =  1.00;

   SMS->SetContour(3, contours);
   SMS->Draw("CONT Z LIST");
//   SMS_m->SetContour(3, contours_m);
//   SMS_m->Draw("CONT Z LIST");
//   SMS_p->SetContour(3, contours_p);
//   SMS_p->Draw("CONT Z LIST");


   c1->Update();


   // Get Contours
   TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   TList* contLevel = NULL;
   TGraph* curv     = NULL;
   TGraph* gc       = NULL;

   Int_t nGraphs    = 0;
   Int_t TotalConts = 0;


   if (conts == NULL){
      printf("*** No Contours Were Extracted!\n");
      TotalConts = 0;
      return;
   } else {
      TotalConts = conts->GetSize();
   }

   printf("TotalConts = %d\n", TotalConts);

   for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
      nGraphs += contLevel->GetSize();
   }

   nGraphs = 0;


   TCanvas* c2 = new TCanvas("c2","c2",800,600); 
   c2->SetLeftMargin(0.1706731);
   c2->SetRightMargin(0.1983173);
   c2->SetTopMargin(0.04895105);
   c2->SetBottomMargin(0.1416084);
   c2->Range(-289.7381,-191.8196,1334.643,1074.487);
   TH2F *hr = new TH2F("hr","", 26,157.5, 812.5, 28, 12.5,712.5);
   hr->Draw();

   Double_t x0, y0, z0;
   TLatex l;
   l.SetTextSize(0.03);
   char val[20];


    for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      if (i<3) z0 = contours[2-i];
          else z0 = contours[i];
      printf("Z-Level Passed in as:  Z = %f\n", z0);

      // Get first graph from list on curves on this level
      curv = (TGraph*)contLevel->First();
      for(j = 0; j < contLevel->GetSize(); j++){
         curv->GetPoint(0, x0, y0);
         if (z0<0.1) curv->SetLineColor(kWhite);
         if (z0>0.5) curv->SetLineColor(kRed); curv->SetLineStyle(2); curv->SetLineWidth(2.); 
         nGraphs ++;
         printf("\tGraph: %d  -- %d Elements\n", nGraphs,curv->GetN());

	 // Draw clones of the graphs to avoid deletions in case the 1st
	 // pad is redrawn.
         gc = (TGraph*)curv->Clone();
         gc->Draw("C");

         sprintf(val,"%g",z0);
         //l.DrawLatex(x0,y0,val);
         curv = (TGraph*)contLevel->After(curv); // Get Next graph
      }
   }

   c2->Update();
   printf("\n\n\tExtracted %d Contours and %d Graphs \n", TotalConts, nGraphs );
   gStyle->SetTitleW(0.);
   gStyle->SetTitleH(0.);


   TLine lm(shell, 0 , 800. , 800-shell);
   lm.SetLineColor(kBlack); lm.SetLineWidth(2); lm.SetLineStyle(2); lm.Draw();
   hr->GetYaxis()->SetTitle("LSP mass");
   hr->GetXaxis()->SetTitle("Stop mass");
   hr->GetZaxis()->SetTitle("Signal Strength"); 
   hr->GetZaxis()->SetRangeUser(0,1.);
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.04);
   l1.SetNDC();
   l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
   l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
   l1.SetTextSize(0.03);

   if (dir == "T2bw025") { tex = new TLatex(0.4736181,0.3409091,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw050") { tex = new TLatex(0.321608,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw075") { tex = new TLatex(0.2763819,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2tt")    { tex = new TLatex(0.3278894,0.3391608,"#bf{m_{#tilde{t}} - m_{#tilde{#chi^{0}_{1}}} = m_{t}}");}

   tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();

   //c1.SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.C");
   //c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.pdf");
   //c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.png");

return *gc;
}

*/



TGraph plot_limit_m(TString &dir){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);


  //palette settings - completely independent
  const Int_t NRGBs = 6;
  const Int_t NCont = 999;

  Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.99, 0.0, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.0, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.99, 0.0, 1.00, 0.12, 0.00, 0.00 };


  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  gStyle->SetOptStat(0);


   double shell;

   
   if (dir == "T2bw025") {dataset_name = "t2bw025"; shell = 80. / 0.25;}
   if (dir == "T2bw050") {dataset_name = "t2bw050"; shell = 80. / 0.50;}
   if (dir == "T2bw075") {dataset_name = "t2bw075"; shell = 80. / 0.75;}
   if (dir == "T2tt")    {dataset_name = "t2tt";    shell = 173.;}



   TH2D *SMS = new TH2D("SMS","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_m = new TH2D("SMS_m","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_p = new TH2D("SMS_p","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_obs = new TH2D("SMS_obs","",26,157.5, 812.5, 28, 12.5,712.5);


              for(int x=175; x<=800; x+=25){


                      for(int y=25; y<=700; y+=25){


  		  char filename[500];
	          long begin,end;



                  sprintf(filename,"/afs/cern.ch/work/s/sigamani/public/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit/%s/higgsCombineS%d-N%d.Asymptotic.mH120.root", dataset_name, x, y);
                  ifstream ifile(filename);
	          if (!ifile.good()) continue; 

		  TFile* file  = new TFile(filename, "READ");  
		  char* name = (char*)file->GetName();



TTree *limittree = (TTree*)file->Get("limit");


double high_val = 100.;


TH1F* obs = new TH1F("obs","",100,0,high_val);
limittree->Draw("limit>>obs", "quantileExpected==-1");
TH1F* expm2 = new TH1F("expm2","",100,0,high_val);
limittree->Draw("limit>>expm2", "quantileExpected>0.02 && quantileExpected<0.03");
TH1F* expm1 = new TH1F("expm1","",100,0,high_val);
limittree->Draw("limit>>expm1", "quantileExpected>0.15 && quantileExpected<0.16");
TH1F* exp = new TH1F("exp","",100,0,high_val);
limittree->Draw("limit>>exp", "quantileExpected==0.5");
TH1F* expp1 = new TH1F("expp1","",100,0,high_val);
limittree->Draw("limit>>expp1", "quantileExpected>0.83 && quantileExpected<0.84");
TH1F* expp2 = new TH1F("expp2","",100,0,high_val);
limittree->Draw("limit>>expp2", "quantileExpected>0.97 && quantileExpected<0.98");

//cout << "limit: " << exp->GetMean() << endl;



   if (expm2->GetMean() < 0.99) { SMS->Fill(x,y, expm2->GetMean());} 

   //if (exp->GetMean() < 1.) { cout << "x: "<< x << ", y: "<< y << endl; }



   file->Close();

		}
	  }



   Double_t contours_m[3];
   contours_m[0] =  0.1;
   contours_m[1] =  0.99;
   contours_m[2] =  1.00;

   SMS->SetContour(3, contours_m);
   SMS->Draw("CONT Z LIST");
//   SMS_m->SetContour(3, contours_m);
//   SMS_m->Draw("CONT Z LIST");
//   SMS_p->SetContour(3, contours_p);
//   SMS_p->Draw("CONT Z LIST");


   c1->Update();


   // Get Contours
   TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   TList* contLevel = NULL;
   TGraph* curv     = NULL;
   TGraph gc       = NULL;

   Int_t nGraphs    = 0;
   Int_t TotalConts = 0;


   if (conts == NULL){
      printf("*** No Contours Were Extracted!\n");
      TotalConts = 0;
      return;
   } else {
      TotalConts = conts->GetSize();
   }

   printf("TotalConts = %d\n", TotalConts);

   for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
      nGraphs += contLevel->GetSize();
   }

   nGraphs = 0;


   TCanvas* c2 = new TCanvas("c2","c2",800,600); 
   c2->SetLeftMargin(0.1706731);
   c2->SetRightMargin(0.1983173);
   c2->SetTopMargin(0.04895105);
   c2->SetBottomMargin(0.1416084);
   c2->Range(-289.7381,-191.8196,1334.643,1074.487);
   TH2F *hr = new TH2F("hr","", 26,157.5, 812.5, 28, 12.5,712.5);
   hr->Draw();

   Double_t x0, y0, z0;
   TLatex l;
   l.SetTextSize(0.03);
   char val[20];


    for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      if (i<3) z0 = contours[2-i];
          else z0 = contours[i];
      printf("Z-Level Passed in as:  Z = %f\n", z0);

      // Get first graph from list on curves on this level
      curv = (TGraph*)contLevel->First();
      for(j = 0; j < contLevel->GetSize(); j++){
         curv->GetPoint(0, x0, y0);
         if (z0<0.1) curv->SetLineColor(kWhite);
         if (z0>0.5) curv->SetLineColor(kRed); curv->SetLineStyle(2); curv->SetLineWidth(2.); 
         nGraphs ++;
         printf("\tGraph: %d  -- %d Elements\n", nGraphs,curv->GetN());

	 // Draw clones of the graphs to avoid deletions in case the 1st
	 // pad is redrawn.
         gc = (TGraph*)curv->Clone();
         gc->Draw("C");

         sprintf(val,"%g",z0);
         //l.DrawLatex(x0,y0,val);
         curv = (TGraph*)contLevel->After(curv); // Get Next graph
      }
   }

   c2->Update();
   printf("\n\n\tExtracted %d Contours and %d Graphs \n", TotalConts, nGraphs );
   gStyle->SetTitleW(0.);
   gStyle->SetTitleH(0.);


   TLine lm(shell, 0 , 800. , 800-shell);
   lm.SetLineColor(kBlack); lm.SetLineWidth(2); lm.SetLineStyle(2); lm.Draw();
   hr->GetYaxis()->SetTitle("LSP mass");
   hr->GetXaxis()->SetTitle("Stop mass");
   hr->GetZaxis()->SetTitle("Signal Strength"); 
   hr->GetZaxis()->SetRangeUser(0,1.);
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.04);
   l1.SetNDC();
   l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
   l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
   l1.SetTextSize(0.03);

   if (dir == "T2bw025") { tex = new TLatex(0.4736181,0.3409091,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw050") { tex = new TLatex(0.321608,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw075") { tex = new TLatex(0.2763819,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2tt")    { tex = new TLatex(0.3278894,0.3391608,"#bf{m_{#tilde{t}} - m_{#tilde{#chi^{0}_{1}}} = m_{t}}");}

   tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();

   //c1.SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.C");
   //c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.pdf");
   //c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.png");

return gc;
}





void plot_limit(TString &dir){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);


  //palette settings - completely independent
  const Int_t NRGBs = 6;
  const Int_t NCont = 999;

  Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.99, 0.0, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.0, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.99, 0.0, 1.00, 0.12, 0.00, 0.00 };


  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  gStyle->SetOptStat(0);


   double shell;

   
   if (dir == "T2bw025") {dataset_name = "t2bw025"; shell = 80. / 0.25;}
   if (dir == "T2bw050") {dataset_name = "t2bw050"; shell = 80. / 0.50;}
   if (dir == "T2bw075") {dataset_name = "t2bw075"; shell = 80. / 0.75;}
   if (dir == "T2tt")    {dataset_name = "t2tt";    shell = 173.;}



   TH2D *SMS = new TH2D("SMS","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_m = new TH2D("SMS_m","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_p = new TH2D("SMS_p","",26,157.5, 812.5, 28, 12.5,712.5);
   TH2D *SMS_obs = new TH2D("SMS_obs","",26,157.5, 812.5, 28, 12.5,712.5);


              for(int x=175; x<=800; x+=25){


                      for(int y=25; y<=700; y+=25){


  		  char filename[500];
	          long begin,end;



                  sprintf(filename,"/afs/cern.ch/work/s/sigamani/public/CMSSW_6_1_1/src/HiggsAnalysis/CombinedLimit/%s/higgsCombineS%d-N%d.Asymptotic.mH120.root", dataset_name, x, y);
                  ifstream ifile(filename);
	          if (!ifile.good()) continue; 

		  TFile* file  = new TFile(filename, "READ");  
		  char* name = (char*)file->GetName();



TTree *limittree = (TTree*)file->Get("limit");


double high_val = 100.;


TH1F* obs = new TH1F("obs","",100,0,high_val);
limittree->Draw("limit>>obs", "quantileExpected==-1");
TH1F* expm2 = new TH1F("expm2","",100,0,high_val);
limittree->Draw("limit>>expm2", "quantileExpected>0.02 && quantileExpected<0.03");
TH1F* expm1 = new TH1F("expm1","",100,0,high_val);
limittree->Draw("limit>>expm1", "quantileExpected>0.15 && quantileExpected<0.16");
TH1F* exp = new TH1F("exp","",100,0,high_val);
limittree->Draw("limit>>exp", "quantileExpected==0.5");
TH1F* expp1 = new TH1F("expp1","",100,0,high_val);
limittree->Draw("limit>>expp1", "quantileExpected>0.83 && quantileExpected<0.84");
TH1F* expp2 = new TH1F("expp2","",100,0,high_val);
limittree->Draw("limit>>expp2", "quantileExpected>0.97 && quantileExpected<0.98");

//cout << "limit: " << exp->GetMean() << endl;


   if (expp1->GetMean() < 0.99)  continue;
   //if (expp1->GetMean() > 1.)  continue;
   SMS->Fill(x,y,expp1->GetMean()); 



   file->Close();

		}
	  }



   Double_t contours[3];
   contours[0] =  1.0;
   contours[1] =  1.4;
   contours[2] =  1.5;

   SMS->SetContour(3, contours);


   SMS->Draw("CONT Z LIST");


   c1->Update();


   // Get Contours
   TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   TList* contLevel = NULL;
   TGraph* curv     = NULL;
   TGraph gc       = NULL;

   Int_t nGraphs    = 0;
   Int_t TotalConts = 0;


   if (conts == NULL){
      printf("*** No Contours Were Extracted!\n");
      TotalConts = 0;
      return;
   } else {
      TotalConts = conts->GetSize();
   }

   printf("TotalConts = %d\n", TotalConts);

   for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
      nGraphs += contLevel->GetSize();
   }

   nGraphs = 0;


   TCanvas* c2 = new TCanvas("c2","c2",800,600); 
   c2->SetLeftMargin(0.1706731);
   c2->SetRightMargin(0.1983173);
   c2->SetTopMargin(0.04895105);
   c2->SetBottomMargin(0.1416084);
   c2->Range(-289.7381,-191.8196,1334.643,1074.487);
   TH2F *hr = new TH2F("hr","", 26,157.5, 812.5, 28, 12.5,712.5);
   hr->Draw();

   Double_t x0, y0, z0;
   TLatex l;
   l.SetTextSize(0.03);
   char val[20];


    for(i = 0; i < TotalConts; i++){
      contLevel = (TList*)conts->At(i);
      if (i<3) z0 = contours[2-i];
          else z0 = contours[i];
      printf("Z-Level Passed in as:  Z = %f\n", z0);

      // Get first graph from list on curves on this level
      curv = (TGraph*)contLevel->First();
      for(j = 0; j < contLevel->GetSize(); j++){
         curv->GetPoint(0, x0, y0);
         if (z0<0) curv->SetLineColor(kRed);
         if (z0>0) curv->SetLineColor(kBlue);
         nGraphs ++;
         printf("\tGraph: %d  -- %d Elements\n", nGraphs,curv->GetN());

	 // Draw clones of the graphs to avoid deletions in case the 1st
	 // pad is redrawn.
         gc = (TGraph*)curv->Clone();
         gc->Draw("C");

         sprintf(val,"%g",z0);
         l.DrawLatex(x0,y0,val);
         curv = (TGraph*)contLevel->After(curv); // Get Next graph
      }
   }

   c2->Update();
   printf("\n\n\tExtracted %d Contours and %d Graphs \n", TotalConts, nGraphs );
   gStyle->SetTitleW(0.);
   gStyle->SetTitleH(0.);


   TLine lm(shell, 0 , 800. , 800-shell);
   lm.SetLineColor(kBlack); lm.SetLineWidth(2); lm.SetLineStyle(2); lm.Draw();
   hr->GetYaxis()->SetTitle("LSP mass");
   hr->GetXaxis()->SetTitle("Stop mass");
   hr->GetZaxis()->SetTitle("Signal Strength"); 
   hr->GetZaxis()->SetRangeUser(0,1.);
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.04);
   l1.SetNDC();
   l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
   l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
   l1.SetTextSize(0.03);

   if (dir == "T2bw025") { tex = new TLatex(0.4736181,0.3409091,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw050") { tex = new TLatex(0.321608,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw075") { tex = new TLatex(0.2763819,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2tt")    { tex = new TLatex(0.3278894,0.3391608,"#bf{m_{#tilde{t}} - m_{#tilde{#chi^{0}_{1}}} = m_{t}}");}

   tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();

   //c1.SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.C");
   c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.pdf");
   c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.png");

//return gc;
}





void plot(TString &dir){

          gStyle->SetOptStat(0);
          gStyle->SetCanvasColor(0);
          gStyle->SetPadColor(0);
          gStyle->SetMarkerStyle(15);
          gStyle->SetMarkerSize(0.25);
          gStyle->SetTextFont(42);
          gStyle->SetMarkerColor(37);


  //palette settings - completely independent
  const Int_t NRGBs = 6;
  const Int_t NCont = 999;

  Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.99, 0.0, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.0, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.99, 0.0, 1.00, 0.12, 0.00, 0.00 };


  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  gStyle->SetOptStat(0);


   double shell;

   
   if (dir == "T2bw025") {dataset_name = "t2bw025"; shell = 80. / 0.25;}
   if (dir == "T2bw050") {dataset_name = "t2bw050"; shell = 80. / 0.50;}
   if (dir == "T2bw075") {dataset_name = "t2bw075"; shell = 80. / 0.75;}
   if (dir == "T2tt")    {dataset_name = "t2tt";    shell = 173.;}


   // Get Contours
   TGraph gc         =  plot_limit("T2tt");
   //TGraph gc_m       =  plot_limit_m("T2tt");
   //TGraph gc_p       =  plot_limit_p("T2tt");



   TCanvas* c2 = new TCanvas("c2","c2",800,600); 
   c2->SetLeftMargin(0.1706731);
   c2->SetRightMargin(0.1983173);
   c2->SetTopMargin(0.04895105);
   c2->SetBottomMargin(0.1416084);
   c2->Range(-289.7381,-191.8196,1334.643,1074.487);
   TH2F *hr = new TH2F("hr","", 26,157.5, 812.5, 28, 12.5,712.5);
   hr->Draw();
         
	 gc.Draw("C");
         //gc_m.Draw("C");
         //gc_p.Draw("C");


   c2->Update();
//   printf("\n\n\tExtracted %d Contours and %d Graphs \n", TotalConts, nGraphs );
//   gStyle->SetTitleW(0.);
//   gStyle->SetTitleH(0.);


   TLine lm(shell, 0 , 800. , 800-shell);
   lm.SetLineColor(kBlack); lm.SetLineWidth(2); lm.SetLineStyle(2); lm.Draw();
   hr->GetYaxis()->SetTitle("LSP mass");
   hr->GetXaxis()->SetTitle("Stop mass");
   hr->GetZaxis()->SetTitle("Signal Strength"); 
   hr->GetZaxis()->SetRangeUser(0,1.);
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.04);
   l1.SetNDC();
   l1.DrawLatex(0.155, 0.98, "CMS Simulation, 20 fb^{-1}");
   l1.DrawLatex(0.7, 0.98, "#sqrt{s} = 8 TeV");
   l1.SetTextSize(0.03);

   if (dir == "T2bw025") { tex = new TLatex(0.4736181,0.3409091,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw050") { tex = new TLatex(0.321608,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2bw075") { tex = new TLatex(0.2763819,0.3513986,"#bf{m_{#tilde{#chi^{#pm}_{1}}} - m_{#tilde{#chi^{0}_{1}}} = m_{W}}");} 
   if (dir == "T2tt")    { tex = new TLatex(0.3278894,0.3391608,"#bf{m_{#tilde{t}} - m_{#tilde{#chi^{0}_{1}}} = m_{t}}");}

   tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.03);
   tex->SetTextAngle(41.15586);
   tex->SetLineWidth(2);
   tex->Draw();

   //c1.SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.C");
   //c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.pdf");
   ///c2->SaveAs(savedir+"exp_exclusion_SMS_t2bW_75.png");

}





void rootlogon() {
   gROOT->SetStyle("Plain");
  // gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
   gStyle->SetOptFit(111110);
   gStyle->SetOptFile(1);
   gStyle->SetMarkerStyle(20);
   gStyle->SetMarkerSize(2.);
   gStyle->SetMarkerColor(1);
   gStyle->SetTitleOffset(1.20,"Y");
   gStyle->SetTextFont(42);
   
    //define high def color palette
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    cout << "loading TDR style and setting as default" << endl;
    gROOT->ProcessLine(".L ~/tdrstyle.C");
    setTDRStyle();

}






