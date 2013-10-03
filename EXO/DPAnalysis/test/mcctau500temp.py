from os import listdir
import ROOT
from ROOT import TCanvas, gStyle, TFile, TH1F, gROOT, TLegend, THStack, TColor, kOrange, kRed, kWhite, kBlack,TPaveText,TAttMarker

#all background lists
a = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-120to170_TuneZ2star/")
b = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-170to300_TuneZ2star/")
c = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-300to470_TuneZ2star/")
d = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-470to800_TuneZ2star/")
e = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-50to80_TuneZ2star/")
f = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/G_Pt-80to120_TuneZ2star/")

#all signal lists
g = listdir("/pnfs/iihe/cms/store/user/sigamani/Summer12/v4/GMSB_Lambda-180_CTau-500/")

#all data lists
h = listdir("/pnfs/iihe/cms/store/user/sigamani/DATA/v4/Run2012C-EXODisplacedPhoton-19Dec2012-v1/")
i = listdir("/pnfs/iihe/cms/store/user/sigamani/DATA/v4/Run2012D-EXODisplacedPhoton-19Dec2012-v1/")


#saving the maps: 0-5:bkg, 6: signal, 7-8:data
maplist = ['G_Pt-120to170_TuneZ2star/','G_Pt-170to300_TuneZ2star/','G_Pt-300to470_TuneZ2star/','G_Pt-470to800_TuneZ2star/','G_Pt-50to80_TuneZ2star/','G_Pt-80to120_TuneZ2star/', 'GMSB_Lambda-180_CTau-500/','Run2012C-EXODisplacedPhoton-19Dec2012-v1/','Run2012D-EXODisplacedPhoton-19Dec2012-v1/']
listnames = ['a','b','c','d','e','f','g','h','i']
list = [a,b,c,d,e,f,g,h,i]

#maplist = ['G_Pt-120to170_TuneZ2star/']
#listnames = ['a']
#list = [a]


#cross section for different bins: 0-5:bkg, 6: signal
sigma = [108.0068,30.12207,2.138632,0.2119244,3322.309,558.2865,0.0145]

#open histogram, one to add all data for bkg, one for signal and one for data
metbkg = TH1F("metbkg","",200,0,400)
metsignal = TH1F("metsignal","",200,0,400)
metdata = TH1F("metdata","",200,0,400)
photonPtbkg = TH1F("photonPtbkg","",200,0,400)
photonPtsignal = TH1F("photonPtsignal","",200,0,400)
photonPtdata = TH1F("photonPtdata","",200,0,400)
dxybkg = TH1F("dxybkg","",100,-100,100)
dxysignal = TH1F("dxysignal","",100,-100,100)
dxydata = TH1F("dxydata","",100,-100,100)
hsmet = THStack("hsmet","")
hsphotonPt = THStack("hsphotonPt","")
hsdxy = THStack("hsdxy","")



#loop over all bkg maps to add all events in one histogram
for item in range(1):


    #loop over all root files in one map
    for i in range(len(list[item])):
        print listnames[item]+": " + str(i) + "/" + str(len(list[item])-1)
        data=TFile.Open('dcap://maite.iihe.ac.be///pnfs/iihe/cms/store/user/sigamani/Summer12/v4/'+maplist[item]+list[item][i])
        tree = data.Get("DPAnalysis")
        entries = data.Get("Entries")
        eff = 1./entries.GetBinContent(1)
        #print "Efficiency file /pnfs/iihe/cms/store/user/sigamani/Summer12/v4/" + str(maplist[item]) + "/" + str(list[item][i]) + ":" + str(eff)

        #scaling needed for different cross sections, assuming lumin of 20 000 pb^-1
        scale = 10000*sigma[item]*eff
        
        metbkgtemp = TH1F("metbkgtemp","",200,0,400)
        metbkgtemp.Draw()
        tree.Draw("met>>metbkgtemp")
        metbkgtemp.Scale(scale)
        metbkg.Add(metbkgtemp,1.0)
        
        photonPtbkgtemp = TH1F("photonPtbkgtemp","",200,0,400)
        photonPtbkgtemp.Draw()
        tree.Draw("phoPt>>photonPtbkgtemp")
        photonPtbkgtemp.Scale(scale)
        photonPtbkg.Add(photonPtbkgtemp,1.0)
        
        dxybkgtemp = TH1F("dxybkgtemp","",100,-100,100)
        dxybkgtemp.Draw()
        tree.Draw("phoDxy>>dxybkgtemp")
        dxybkgtemp.Scale(scale)
        dxybkg.Add(dxybkgtemp,1.0)
        data.Close()

#loop over signal file
for i in range(len(list[6])):
    print "g: " + str(i) + "/" + str(len(g)-1)
    data=TFile.Open('dcap://maite.iihe.ac.be///pnfs/iihe/cms/store/user/sigamani/Summer12/v4/GMSB_Lambda-180_CTau-500/'+g[i])
    tree = data.Get("DPAnalysis")
    entries= data.Get("Entries")
    eff = entries.GetBinContent(7)/entries.GetBinContent(1)
    #print "Efficiency file /pnfs/iihe/cms/store/user/sigamani/Summer12/v4/GMSB_Lambda-180_CTau-500/" + "/" + str(g[i]) + ":" + str(eff)

    scale = 10000*sigma[6]*eff
    
    metsignaltemp = TH1F("metsignaltemp","",200,0,400)
    metsignaltemp.Draw()
    tree.Draw("met>>metsignaltemp")
    metsignaltemp.Scale(scale)
    metsignal.Add(metsignaltemp,1.0)

    photonPtsignaltemp = TH1F("photonPtsignaltemp","",200,0,400)
    photonPtsignaltemp.Draw()
    tree.Draw("phoPt>>photonPtsignaltemp")
    photonPtsignaltemp.Scale(scale)
    photonPtsignal.Add(photonPtsignaltemp,1.0)

    dxysignaltemp = TH1F("dxysignaltemp","",100,-100,100)
    dxysignaltemp.Draw()
    tree.Draw("phoDxy>>dxysignaltemp")
    dxysignaltemp.Scale(scale)
    dxysignal.Add(dxysignaltemp,1.0)
    data.Close()

#loop over data files
for item in range(7,9):

        #loop over all root files in one map
        for i in range(len(list[item])):
            print listnames[item]+": " + str(i) + "/" + str(len(list[item])-1)
            data=TFile.Open('dcap://maite.iihe.ac.be///pnfs/iihe/cms/store/user/sigamani/DATA/v4/'+maplist[item]+list[item][i])
            tree = data.Get("DPAnalysis")

            metdatatemp = TH1F("metdatatemp","",200,0,400)
            metdatatemp.Draw()
            tree.Draw("met>>metdatatemp")
            metdata.Add(metdatatemp,1.0)

            photonPtdatatemp = TH1F("photonPtdatatemp","",200,0,400)
            photonPtdatatemp.Draw()
            tree.Draw("phoPt>>photonPtdatatemp")
            photonPtdata.Add(photonPtdatatemp,1.0)

            dxydatatemp = TH1F("dxydatatemp","",100,-100,100)
            dxydatatemp.Draw()
            tree.Draw("phoDxy>>dxydatatemp")
            dxydatatemp.Scale(scale)
            dxydata.Add(dxydatatemp,1.0)
            data.Close()

#label function
def label(quadrant):
    if (quadrant!=1 and quadrant !=2):
        print "This quadrant is not defined, choose a value of 1 or 2!"
    else:
        if (quadrant == 1):
            x1 = 0.1
            x2 = 0.25
            y1 = 0.91
            y2 = 1.0
        if (quadrant == 2):
            x1 = 0.75
            x2 = 0.9
            y1 = 0.91
            y2 = 1.0
                                                
            
    headlabel = TPaveText( x1, y1, x2, y2, "brNDC" )
    if (quadrant == 1):
        headlabel.AddText("CMS = ??? fb^{-1}")
    if (quadrant == 2):
        headlabel.AddText("#sqrt{s} = 8 TeV")
    
    headlabel.SetFillColor(kWhite)
    headlabel.SetTextSize(0.038)
    headlabel.SetTextFont(42)
    headlabel.SetBorderSize(0)
    headlabel.SetShadowColor(kWhite)

    return headlabel
        

#setting the labels
cmslabel = label(1)
sqrtlabel = label(2)

#drawing the met histogram
metsignal.SetLineColor(kRed)
metbkg.SetFillColor(kOrange)
metdata.SetMarkerStyle(20)
metdata.SetMarkerSize(0.5)
metdata.SetMarkerColor(kBlack)

hsmet.Add(metbkg)

#metbkg.SetTitle('Estimated met, c#tau = 100 cm, Simulation')

c1 = TCanvas("c1","c1",600,500);
gStyle.SetOptStat(0)
c1.SetLogy();
leg = TLegend(0.55,0.75,0.89,0.89)
leg.SetFillColor(kWhite)
leg.SetTextSize(0.038)
leg.SetTextFont(42)
leg.SetBorderSize(0)
leg.AddEntry(metdata, "DATA","pe")
leg.AddEntry(metbkg, "bkg","f")
leg.AddEntry(metsignal, "signal","lp")
hsmet.Draw()
hsmet.GetYaxis().SetTitle("Number of Events")
hsmet.GetYaxis().SetTitleSize(0.05)
hsmet.GetXaxis().SetTitle("MET")
hsmet.GetXaxis().SetTitleSize(0.05)
c1.Update()
metsignal.Draw("same")
metdata.Draw("PEsame")
leg.Draw("same")
cmslabel.Draw("same")
sqrtlabel.Draw("same")
c1.SaveAs("mcmetctau500.pdf")




#drawing the photonpt histogram
photonPtsignal.SetLineColor(kRed)
photonPtbkg.SetFillColor(kOrange)
photonPtdata.SetMarkerStyle(20)
photonPtdata.SetMarkerSize(0.5)
photonPtdata.SetMarkerColor(kBlack)

hsphotonPt.Add(photonPtbkg)

c2 = TCanvas("c2","c2",600,500);
gStyle.SetOptStat(0)
c2.SetLogy();
leg = TLegend(0.55,0.75,0.89,0.89)
leg.SetFillColor(kWhite)
leg.SetTextSize(0.038)
leg.SetTextFont(42)
leg.SetBorderSize(0)
leg.AddEntry(photonPtdata, "DATA","pe")
leg.AddEntry(photonPtbkg, "bkg","f")
leg.AddEntry(photonPtsignal, "signal","lp")
hsphotonPt.Draw()
hsphotonPt.GetYaxis().SetTitle("Number of Events")
hsphotonPt.GetYaxis().SetTitleSize(0.05)
hsphotonPt.GetXaxis().SetTitle("photonPt")
hsphotonPt.GetXaxis().SetTitleSize(0.05)
c2.Update()
photonPtsignal.Draw("same")
photonPtdata.Draw("PEsame")
leg.Draw("same")
cmslabel.Draw("same")
sqrtlabel.Draw("same")
c2.SaveAs("mcphotonptctau500.pdf")



#drawing the dxy histogram
dxysignal.SetLineColor(kRed)
dxybkg.SetFillColor(kOrange)
dxydata.SetMarkerStyle(20)
dxydata.SetMarkerSize(0.5)
dxydata.SetMarkerColor(kBlack)

hsdxy.Add(dxybkg)

c3 = TCanvas("c3","c3",600,500);
gStyle.SetOptStat(0)
c3.SetLogy();
leg = TLegend(0.55,0.75,0.89,0.89)
leg.SetFillColor(kWhite)
leg.SetTextSize(0.038)
leg.SetTextFont(42)
leg.SetBorderSize(0)
leg.AddEntry(dxydata, "DATA","pe")
leg.AddEntry(dxybkg, "bkg","f")
leg.AddEntry(dxysignal, "signal","lp")
hsdxy.Draw()
hsdxy.GetYaxis().SetTitle("Number of Events")
hsdxy.GetYaxis().SetTitleSize(0.05)
hsdxy.GetXaxis().SetTitle("dXY")
hsdxy.GetXaxis().SetTitleSize(0.05)
c3.Update()
dxysignal.Draw("same")
dxydata.Draw("PEsame")
leg.Draw("same")
cmslabel.Draw("same")
sqrtlabel.Draw("same")
c3.SaveAs("mcdxyctau500.pdf")

