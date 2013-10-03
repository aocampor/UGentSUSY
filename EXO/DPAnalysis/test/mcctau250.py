from os import listdir
import ROOT
from ROOT import TCanvas, gStyle, TFile, TH1F, gROOT, TLegend, THStack, TColor, kOrange, kRed, kWhite, kBlack,TPaveText,TAttMarker

#fill the tree with desired variables
def FillHist(met,photonPt,dxy,sMinPho,sMajPho,tree):

    for event in tree:
        met.Fill(event.met)
        for j in range(len(event.phoPt)):
            photonPt.Fill(event.phoPt[j])
        for j in range(len(event.phoDxy)):
            dxy.Fill(event.phoDxy[j])
        for j in range(len(event.phoPt)):
            sMinPho.Fill(event.sMinPho[j])
        for j in range(len(event.phoPt)):
            sMajPho.Fill(event.sMajPho[j])


#label function
def label(quadrant):
    if (quadrant!=1 and quadrant !=2):
        print "This quadrant is not defined, choose a value of 1 or 2!"
    else:
        if (quadrant == 1):
            x1 = 0.1
            x2 = 0.25
            y1 = 0.94
            y2 = 1.0
        if (quadrant == 2):
            x1 = 0.75
            x2 = 0.9
            y1 = 0.94
            y2 = 1.0


    headlabel = TPaveText( x1, y1, x2, y2, "brNDC" )
    if (quadrant == 1):
        headlabel.AddText("CMS = ??? fb^{-1}")
    if (quadrant == 2):
        headlabel.AddText("#sqrt{s} = 8 TeV")

    headlabel.SetFillColor(kWhite)
    headlabel.SetTextSize(0.034)
    headlabel.SetTextFont(42)
    headlabel.SetBorderSize(0)
    headlabel.SetShadowColor(kWhite)

    return headlabel

#drawing a histogram
def DrawHist(string,bkg,signal,data,stack,cmslabel,sqrtlabel):
    signal.SetLineColor(kRed)
    bkg.SetFillColor(kOrange)
    data.SetMarkerStyle(20)
    data.SetMarkerSize(0.5)
    data.SetMarkerColor(kBlack)

    stack.Add(bkg)

    c1 = TCanvas("c1","c1",600,500);
    gStyle.SetOptStat(0)
    c1.SetLogy();
    leg = TLegend(0.55,0.75,0.89,0.89)
    leg.SetFillColor(kWhite)
    leg.SetTextSize(0.038)
    leg.SetTextFont(42)
    leg.SetBorderSize(0)
    leg.AddEntry(data, "DATA","pe")
    leg.AddEntry(bkg, "bkg","f")
    leg.AddEntry(signal, "signal","l")
    stack.Draw()
    stack.GetYaxis().SetTitle("Number of Events")
    stack.GetYaxis().SetTitleSize(0.045)
    stack.GetXaxis().SetTitle(string)
    stack.GetXaxis().SetTitleSize(0.05)
    c1.Update()
    signal.Draw("same")
    data.Draw("PEsame")
    leg.Draw("same")
    cmslabel.Draw("same")
    sqrtlabel.Draw("same")
    c1.SaveAs(string+"ctau250.pdf")

#saving the maps: 0-5:bkg, 6: signal, 7-8:data
maplist = ["bkg120to170.root","bkg170to300.root","bkg300to470.root","bkg470to800.root","bkg50to80.root","bkg80to120.root","Signalctau250.root","Run2012Cdata.root","Run2012Ddata.root"]

#Cross section for different bins: 0-5:bkg, 6: signal
sigma = [108.0068,30.12207,2.138632,0.2119244,3322.309,558.2865,0.0145]

#open histogram, one to add all data for bkg, one for signal and one for data
metbkg = TH1F("metbkg","",200,0,400)
metsignal = TH1F("metsignal","",200,0,400)
metdata = TH1F("metdata","",200,0,400)
hsmet = THStack("hsmet","")

photonPtbkg = TH1F("photonPtbkg","",200,0,400)
photonPtsignal = TH1F("photonPtsignal","",200,0,400)
photonPtdata = TH1F("photonPtdata","",200,0,400)
hsphotonPt = THStack("hsphotonPt","")

dxybkg = TH1F("dxybkg","",100,-100,100)
dxysignal = TH1F("dxysignal","",100,-100,100)
dxydata = TH1F("dxydata","",100,-100,100)
hsdxy = THStack("hsdxy","")

nJetsbkg = TH1F("nJetsbkg","",4,-2,2)
nJetssignal = TH1F("nJetssignal","",4,-2,2)
nJetsdata = TH1F("nJetsdata","",4,-2,2)
hsnJets = THStack("hsnJets","")

sMajPhobkg = TH1F("sMajPhobkg","",100,0,2)
sMajPhosignal = TH1F("sMajPhosignal","",100,0,2)
sMajPhodata = TH1F("sMajPhodata","",100,0,2)
hssMajPho = THStack("hssMajPho","")

sMinPhobkg = TH1F("sMinPhobkg","",100,0,0.5)
sMinPhosignal = TH1F("sMinPhosignal","",100,0,0.5)
sMinPhodata = TH1F("sMinPhodata","",100,0,0.5)
hssMinPho = THStack("hsMinPho","")


#loop over all bkg maps to add all events in one histogram
for item in range(6):

    print "Total files: " + str(len(maplist)) + ". Busy on: " + str(item+1)
    data=TFile.Open(maplist[item])
    tree = data.Get("DPAnalysis")
    entries = data.Get("Entries")
    eff = 1./entries.GetBinContent(1)

    #scaling needed for different cross sections, assuming lumin of 20 000 pb^-1
    scale = 10000*sigma[item]*eff

    metbkgtemp = TH1F("metbkgtemp","",200,0,400)
    photonPtbkgtemp = TH1F("photonPtbkgtemp","",200,0,400)
    dxybkgtemp = TH1F("dxybkgtemp","",100,-100,100)
    sMinPhobkgtemp = TH1F("sMinPhobkgtemp","",100,0,0.5)
    sMajPhobkgtemp = TH1F("sMajPhobkgtemp","",100,0,2)
    nJetsbkgtemp = TH1F("nJetsbkgtemp","",4,-2,2)
        
    FillHist(metbkgtemp,photonPtbkgtemp,dxybkgtemp,sMinPhobkgtemp,sMajPhobkgtemp,tree)

    tree.Draw("nJets>>nJetsbkgtemp")
    
    nJetsbkg.SetBinContent(3,nJetsbkgtemp.GetBinContent(3))
        
    metbkgtemp.Scale(scale)
    photonPtbkgtemp.Scale(scale)
    dxybkgtemp.Scale(scale)
    sMinPhobkgtemp.Scale(scale)
    sMajPhobkgtemp.Scale(scale)

    metbkg.Add(metbkgtemp,1.)
    photonPtbkg.Add(photonPtbkgtemp,1.)
    dxybkg.Add(dxybkgtemp,1.)
    sMinPhobkg.Add(sMinPhobkgtemp,1.)
    sMajPhobkg.Add(sMajPhobkgtemp,1.)
    data.Close()

#loop over signal file
print "Total files: " + str(len(maplist)) + ". Busy on: 7"
data=TFile.Open(maplist[6])
tree = data.Get("DPAnalysis")
entries= data.Get("Entries")
eff = 1./entries.GetBinContent(1)

scale = 10000*sigma[6]*eff
    
metsignaltemp = TH1F("metsignaltemp","",200,0,400)
photonPtsignaltemp = TH1F("photonPtsignaltemp","",200,0,400)
dxysignaltemp = TH1F("dxysignaltemp","",100,-100,100)
sMinPhosignaltemp = TH1F("sMinPhosignaltemp","",100,0,0.5)
sMajPhosignaltemp = TH1F("sMajPhosignaltemp","",100,0,2)
nJetssignaltemp = TH1F("nJetssignaltemp","",4,-2,2)

FillHist(metsignaltemp,photonPtsignaltemp,dxysignaltemp,sMinPhosignaltemp,sMajPhosignaltemp,tree)

tree.Draw("nJets>>nJetssignaltemp")

nJetssignal.SetBinContent(3,nJetssignaltemp.GetBinContent(3))
    
metsignaltemp.Scale(scale)
photonPtsignaltemp.Scale(scale)
dxysignaltemp.Scale(scale)
sMinPhosignaltemp.Scale(scale)
sMajPhosignaltemp.Scale(scale)

metsignal.Add(metsignaltemp,1.)
photonPtsignal.Add(photonPtsignaltemp,1.)
dxysignal.Add(dxysignaltemp,1.)
sMinPhosignal.Add(sMinPhosignaltemp,1.)
sMajPhosignal.Add(sMajPhosignaltemp,1.)
    
data.Close()

#loop over data files
for item in range(7,9):

    print "Total files: " + str(len(maplist)) + ". Busy on: " + str(item+1)
    data=TFile.Open(maplist[item])
    tree = data.Get("DPAnalysis")

    metdatatemp = TH1F("metdatatemp","",200,0,400)
    photonPtdatatemp = TH1F("photonPtdatatemp","",200,0,400)
    dxydatatemp = TH1F("dxydatatemp","",100,-100,100)
    sMinPhodatatemp = TH1F("sMinPhodatatemp","",100,0,0.5)
    sMajPhodatatemp = TH1F("sMajPhodatatemp","",100,0,2)
    nJetsdatatemp = TH1F("nJetsdatatemp","",4,-2,2)

    FillHist(metdatatemp,photonPtdatatemp,dxydatatemp,sMinPhodatatemp,sMajPhodatatemp,tree)

    tree.Draw("nJets>>nJetsdatatemp")

    nJetsdata.SetBinContent(3,nJetsdatatemp.GetBinContent(3))

    metdata.Add(metdatatemp,1.)
    photonPtdata.Add(photonPtdatatemp,1.)
    dxydata.Add(dxydatatemp,1.)
    sMinPhodata.Add(sMinPhodatatemp,1.)
    sMajPhodata.Add(sMajPhodatatemp,1.)
    
    data.Close()        

#setting the labels
cmslabel = label(1)
sqrtlabel = label(2)

#drawing the met, photonPt and dXY  histogram
DrawHist("met",metbkg,metsignal,metdata,hsmet,cmslabel,sqrtlabel)
DrawHist("photonpt",photonPtbkg,photonPtsignal,photonPtdata,hsphotonPt,cmslabel,sqrtlabel)
DrawHist("dxy",dxybkg,dxysignal,dxydata,hsdxy,cmslabel,sqrtlabel)
DrawHist("sMinPho",sMinPhobkg,sMinPhosignal,sMinPhodata,hssMinPho,cmslabel,sqrtlabel)
DrawHist("sMajPho",sMajPhobkg,sMajPhosignal,sMajPhodata,hssMajPho,cmslabel,sqrtlabel)
DrawHist("nJets",nJetsbkg,nJetssignal,nJetsdata,hsnJets,cmslabel,sqrtlabel)
