#include "TH1D.h"
#include "TH2D.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TArrow.h"
#include "TF1.h"
#include "TGraph.h"
#include "TPaveStats.h"
#include <TUUID.h>
#include "TRolke.h"
#include "TLegend.h"
#include <map>
#include <iostream>
using namespace std;
#include <fstream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TStopwatch.h"
#include "TLatex.h"
#include <TSpectrum.h>

#include <cstdio>


void peakanalyse_simulation_208Tl()
{
    
    const int num_spek = 1.;
    const int num_files = 1;
    TString Rootfiles[num_files] = {"run208Tl.root"};//{"run_122keV_25mm.root"};
    
    const int num_peaks = 2;
    
    double MP[num_peaks] = {583.187/1000.,
							2614.511/1000.}; 
    int fix[num_peaks];
    for (int i = 0; i < num_peaks; i++) {
        fix[i] = false;
    }
    
    
    // Interval for left BG approximation (für die Analyse wäre es sinnvoll wenn der linke und der rechte BG Abschnitt gleich groß ist)
    double border_ll = -0.015; 
    double border_lr = -0.010;
    
    //Interval of ROI
    double border_ROI_l = -0.010;
    double border_ROI_r = +0.010;
    
    // Interval for right BG approximation (für die Analyse wäre es sinnvoll wenn der linke und der rechte BG Abschnitt gleich groß ist)
    double border_rl = +0.010;
    double border_rr = +0.015;
    
    // hier wird nur definiert welcher Spektrenabschnitt (x-Achse) im Fenster unten angezeigt werden soll
    double fit_left = -0.03;
    double fit_right = +0.03;
    
    // hier wird nur definiert welcher Spektrenabschnitt (y-Achse) im Fenster unten angezeigt werden soll
    double y_min = 0;
    double y_max = 10000000.;
   
    double sigma = 6;
   
//................................................................................................................

    double average_counts1 = 0.0;
    double all_counts1 = 0;
    
    int firstBin = -1;
    int lastBin = -1;
    
    int firstBin_interr = -1;
    int lastBin_interr = -1;
 
//................................................................................................................

 //TCanvas* c[num_infile];
    TFile* fTest[num_spek];
    TH1D* histo_counts[num_spek];
    TH1D* histo_main[num_spek];

for(int i=0; i<num_files; i++)
{
    cout << "Opening file: "<< Rootfiles[i] << endl;
    fTest[i] = new TFile(Rootfiles[i]);
    
    for(int h=0; h<num_spek; h++)
    {
        if(h == 0) histo_counts[h] = (TH1D*)fTest[i]->Get("Evis;1");
        
    }   
}  
   
//==========================================GESAMTES SPEKTRUM============================================================
    
    TCanvas *c1 = new TCanvas("raw spectra","raw spectra",100,100,1000,600);  
    c1->cd(1);

   // gStyle->SetOptStat(1001111);
    gPad->SetLogy(); 
    gStyle->SetOptStat(1001111);
  
    histo_counts[0]->GetXaxis()->SetRangeUser(0,4);
    //histo_all->GetYaxis()->SetRangeUser(1e-4,1);
    histo_counts[0]->GetYaxis()->SetTitleOffset(0.8);
    histo_counts[0]->GetXaxis()->SetTitleOffset(1.2);
    histo_counts[0]->GetYaxis()->SetTitleSize(0.04);
    histo_counts[0]->GetXaxis()->SetTitleSize(0.04);
    histo_counts[0]->GetXaxis()->SetLabelSize(0.04);
    histo_counts[0]->GetYaxis()->SetLabelSize(0.04);
    histo_counts[0]->GetXaxis()->CenterTitle();
    histo_counts[0]->GetYaxis()->CenterTitle();
    histo_counts[0]->GetXaxis()->SetLabelOffset(0.007);
    histo_counts[0]->GetYaxis()->SetLabelOffset(0.006);
    histo_counts[0]->GetYaxis()->SetNdivisions(5);
    histo_counts[0]->GetXaxis()->SetNdivisions(5);
    histo_counts[0]->SetTitle("resulting spectrum");
    histo_counts[0]->GetXaxis()->SetTitle("Energy [MeV]");
    histo_counts[0]->GetYaxis()->SetTitle("Counts [MeV^{-1}]");
    histo_counts[0]->GetYaxis()->CenterTitle();
    histo_counts[0]->SetNdivisions(410);
    
    histo_counts[0]->SetLineColor(kBlack);
    histo_counts[0]->Draw("h");
   
    

    //======================== Auswertung ===============================

     //----------------------------------

    TCanvas* c[num_peaks];
    double Peakcontent[num_peaks];
    double uncert[num_peaks];
    double BG_ROI[num_peaks];
     
    for (int j = 0; j < num_peaks; j++)
    {


    cout<<"______________________________________________________________________" <<endl;
    

        TString canvasName = TString::Format("Canvas_%d", j);
        c[j] = new TCanvas(canvasName, canvasName, 100,100,1100,350);
        // ... (adjust the canvas settings as needed for each canvas)
        
    c[j]->cd();
    
    histo_main[0] = (TH1D *)histo_counts[0]->Clone();
    
    // Scaling th histogram to get the number of counts per day and kg
  /*  for(int i=0;i<histo_main[0]->GetNbinsX();i++){
        histo_main[0]->SetBinContent(i,1.*histo_main[0]->GetBinContent(i)*(60.*60.*24./(realtime1*detector_mass*sample_mass))); 
        } // */
    
    histo_main[0]->Draw(); 
    
    
    //---------------Background --------
    
    
    double bin_ll = histo_main[0]->GetXaxis()->FindBin(MP[j] + border_ll);
    double bin_lr = histo_main[0]->GetXaxis()->FindBin(MP[j] + border_lr);
    double bin_rl = histo_main[0]->GetXaxis()->FindBin(MP[j] + border_rl);
    double bin_rr = histo_main[0]->GetXaxis()->FindBin(MP[j] + border_rr);
    double truebin_ll = bin_ll;//histo_main[0]->GetBinLowEdge(bin_ll);
    double truebin_lr = bin_lr;//histo_main[0]->GetBinLowEdge(bin_lr);
    double truebin_rl = bin_rl;//histo_main[0]->GetBinLowEdge(bin_rl);
    double truebin_rr = bin_rr;//histo_main[0]->GetBinLowEdge(bin_rr);
    
    double Numberbins_BG_left = (truebin_lr- truebin_ll); 
    double Numberbins_BG_right = (truebin_rr- truebin_rl);  
    double SUM_BG_left = 0.;
    double SUM_BG_right = 0.;
    for(float bin=truebin_ll+1; bin<truebin_lr+1; bin++)
        {
        SUM_BG_left += histo_main[0]->GetBinContent(bin);
        }
    for(float bin=truebin_rl+1; bin<truebin_rr+1; bin++)
        {
        SUM_BG_right += histo_main[0]->GetBinContent(bin);
        }
        
    double BGprobin_left = SUM_BG_left/Numberbins_BG_left;
    double BGprobin_right = SUM_BG_right/Numberbins_BG_right;
    
    double BG_average = (BGprobin_left+ BGprobin_right)/2.;
   
    cout<<"bin_ll "<< bin_ll<<endl;
    cout<<"bin_lr "<< bin_lr<<endl; 
    cout<<"truebin_ll "<< truebin_ll<<endl;
    cout<<"truebin_lr "<< truebin_lr<<endl;
    cout<<"Numberbins_BG_left "<< Numberbins_BG_left<<endl;
    cout<<"SUM_BG_left "<< SUM_BG_left<<endl;
    cout<<"SUM_BG_right "<< SUM_BG_right<<endl;
    cout<<"BGprobin_left "<< BGprobin_left<<endl;
    cout<<"BGprobin_right "<< BGprobin_right<<endl;
    cout<<"BG_average "<< BG_average<<endl;
  
    // Implementierung des linken BG Fits (Parameter werden vom gesharten Fit übernommen)
    TF1 *BG_left = new TF1("BG_left","[0]",MP[j]+border_ll, MP[j]+border_lr);
    BG_left->SetLineWidth(3);
    BG_left->SetLineColor(kYellow);  
    histo_main[0]->Fit(BG_left, "R");
    BG_left->Draw("same");
    
    // Implementierung des linken BG Fits (Parameter werden vom gesharten Fit übernommen)
    TF1 *BG_right = new TF1("BG_right","[0]",MP[j]+border_rl, MP[j]+border_rr);
    BG_right->SetLineWidth(3);
    BG_right->SetLineColor(kYellow);   
    histo_main[0]->Fit(BG_right, "R");
    BG_right->Draw("same");   
      
  //  double Nrbins_leftBG = (MP[j]+border_lr - (MP[j]+border_ll))/histo_main[0]->GetBinWidth(5);
  //  double Nrbins_rightBG = (MP[j]+border_rr - (MP[j]+border_rl))/histo_main[0]->GetBinWidth(5);
    
   // double BG_l = BG_left->GetParameter(0)/histo_main[0]->GetBinWidth(5)*Nrbins_leftBG;
  //  double BG_r = BG_right->GetParameter(0)/histo_main[0]->GetBinWidth(5)*Nrbins_rightBG;
    
 //   double BGproBin = (BG_l+BG_r)/(2.);
  //  cout<<"BG pro Bin "<<BGproBin<<endl;
  //  cout<<"BG_l "<<BG_l<<endl;
   // cout<<"BG_r "<<BG_r<<endl;
    

    TF1 *Gauss = new TF1("Gauss","[0]*TMath::Gaus(x,[1],[2])+[3]",MP[j]+border_ROI_l, MP[j]+border_ROI_r);
    Gauss->SetParLimits(0,0,1000000);
    Gauss->SetParameter(1,MP[j]);
    Gauss->SetParLimits(1,MP[j]-0.003,MP[j]+0.003);
    Gauss->SetParameter(2, 1/1000.);
    Gauss->SetParLimits(2, 0, 4/1000.);
    Gauss->FixParameter(3,(BG_left->GetParameter(0)+BG_right->GetParameter(0))/2); 
   
    Gauss->SetLineWidth(4);
    Gauss->SetLineColor(kRed);
    histo_main[0]->Fit("Gauss","Q");
     
   if(Gauss->GetParameter(2)<0.3/1000.)
    {
        //Gauss->Delete();
        Gauss->FixParameter(2, 1.5/1000.);
        histo_main[0]->Fit("Gauss","R");
        cout<<"ATTENTION! Peak width needs to be fixed due to low statistics"<<endl;
        fix[j]=true;
    }
    else{histo_main[0]->Fit("Gauss","R");}
    
    histo_main[0]->GetXaxis()->SetRangeUser(MP[j]+fit_left, MP[j]+fit_right);
    histo_main[0]->GetYaxis()->SetTitleOffset(0.982);
    histo_main[0]->GetXaxis()->SetTitleOffset(1.35);
    histo_main[0]->GetYaxis()->SetTitleSize(0.055);
    histo_main[0]->GetXaxis()->SetTitleSize(0.055);
    histo_main[0]->GetXaxis()->SetLabelSize(0.055);
    histo_main[0]->GetYaxis()->SetLabelSize(0.055);
    histo_main[0]->GetXaxis()->CenterTitle();
    histo_main[0]->GetYaxis()->CenterTitle();
    histo_main[0]->GetXaxis()->SetLabelOffset(0.02);
    histo_main[0]->GetYaxis()->SetLabelOffset(0.01);
    histo_main[0]->GetYaxis()->SetNdivisions(5);
    histo_main[0]->GetXaxis()->SetNdivisions(5);
    histo_main[0]->SetTitle("");
    histo_main[0]->GetXaxis()->SetTitle("Energy [keV]");
    histo_main[0]->GetYaxis()->SetTitle("Counts [keV^{-1}]");
    histo_main[0]->GetYaxis()->CenterTitle();
    histo_main[0]->SetNdivisions(410);
    
    histo_main[0]->Draw("same");
    Gauss->Draw("same");
    
    //_____________________________________________________________________-
     //----------- Number events +-2sigma vom Peakmittelpunkt---------------
   
    double Peak_links = Gauss->GetParameter(1)-sigma*Gauss->GetParameter(2); 
    double Peak_rechts = Gauss->GetParameter(1)+sigma*Gauss->GetParameter(2);
    double leftbin = histo_main[0]->GetXaxis()->FindBin(Peak_links) -1; //weil peak maximum in der regel in mitte eines bins
    double rightbin = histo_main[0]->GetXaxis()->FindBin(Peak_rechts);
    double truebinleft = leftbin;//histo_main[0]->GetBinLowEdge(leftbin);
    double truebinright = rightbin;//histo_main[0]->GetBinLowEdge(rightbin);
    
    cout<<"truebinleft "<<truebinleft<<endl;
    cout<<"truebinright "<<truebinright<<endl;
    
    TLine *Lineleft = new TLine(truebinleft/10000,y_min,truebinleft/10000 ,y_max);
    TLine *Lineright = new TLine(truebinright/10000,y_min,truebinright/10000 ,y_max);
    
    Lineleft->SetLineWidth(3);
    Lineright->SetLineWidth(3);

    Lineleft->Draw("same");
    Lineright->Draw("same");
    
    double Numberbins_2sigma = (truebinright- truebinleft);  
    double SUM_ROI = 0.;
    for(float bin=truebinleft+1; bin<truebinright+1; bin++)
        {
        SUM_ROI += histo_main[0]->GetBinContent(bin);
        }
        

    BG_ROI[j] = BG_average * Numberbins_2sigma;
    Peakcontent[j] = SUM_ROI-BG_ROI[j];
    uncert[j] = TMath::Sqrt(Peakcontent[j]+BG_ROI[j]*(1+(Numberbins_2sigma/(Numberbins_BG_left+Numberbins_BG_right))));

    cout<<"Number of bins ROI: "<< Numberbins_2sigma<<endl;
    cout<<"SUM_ROI: "<< SUM_ROI<<endl;
    cout<<"BG ROI: "<< BG_ROI[j]<<endl;
    cout<<"ROI: "<< Peakcontent[j]<<endl; 
     cout<<"uncert: "<< uncert[j]<<endl; 
    
    }
    
    cout<<"-----------------------------------------------------" <<endl;
    cout<<"Canvas  Peak [keV]      Counts Peak +-"<<sigma<<"sigma     +- uncert       Counts BG"<<endl;
    cout<<"                          /kg*day*1g sample              /kg*day"<<endl; 
      for (int k = 0; k < num_peaks; k++)
    {
        if(Peakcontent[k]>=0.0001)
        {
            if(fix[k]==false)
            {cout<<k<<"      "<<MP[k]<<"        "<<Peakcontent[k]<<"         "<<uncert[k]<<"         "<<BG_ROI[k]<<endl;}
            if(fix[k]==true)
            {cout<<k<<"      "<<MP[k]<<"        "<<Peakcontent[k]<<"*        "<<uncert[k]<<"         "<<BG_ROI[k]<<endl;}
        }
        else cout<<k<<"      "<<MP[k]<<"        "<<0<<"         "<<0<<"         "<<0<<endl;
    }
    cout<<"-----------------------------------------------------" <<endl;
    


    
    
   
}
