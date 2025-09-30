#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

void Analyze()
{
    const double energies[] = {898, 1836}; // keV   // Energies of discretely simulated gammas
    const TString plotName = "plot.pdf";
    
    TCanvas *c = new TCanvas;

    c->Print(plotName + "[", "pdf"); // open
    
    TGraphErrors *graph = new TGraphErrors;
    int iPoint = 0;
    
    for (const auto energykev : energies)
    {cout<<iPoint<<"-----------------------"<<endl;
        const auto inFileName = TString::Format("run_%.0fkeV.root", energykev);
        TFile* inFile = TFile::Open(inFileName, "READ");
        if (inFile->IsZombie())
        {
            cerr << "Failed to open input file \"" << inFileName << "\"." << endl;
            continue;
        }

        TH1D *hAll = static_cast<TH1D*>(inFile->Get("EdepAll"));
        TH1D *hVis = static_cast<TH1D*>(inFile->Get("Evis"));

        const double nSimulated = hAll->GetEntries();

        const double binWidth = hAll->GetBinWidth(1);

        const double energy = energykev/1000;

        TF1 *ff = new TF1("fitFunction", "gausn + pol1(3)", energy-0.02, energy+0.02);
        ff->SetParameter(0, 1e-2*nSimulated*binWidth);
        ff->SetParLimits(0, 0, nSimulated*binWidth);

        ff->SetParameter(1, energy);
        ff->SetParLimits(1, energy-0.01, energy+0.01);

        ff->SetParameter(2, 0.0005);
        ff->SetParLimits(2, 0, 0.010);

        ff->SetParameter(3, 0.000);
        ff->SetParameter(4, 0.000);

        hVis->Draw();
        hVis->GetXaxis()->SetRangeUser(energy-0.01, energy+0.01);
        
        hVis->Fit("fitFunction", "IRB");
           
        c->Print(plotName, "pdf");
        
        const double nFEP = hVis->GetFunction("fitFunction")->GetParameter(0) / binWidth;
        const double FEP = nFEP / nSimulated;
        cout<<energykev<<": \t"<<FEP<<endl;
        
        graph->SetPoint(iPoint, energy, FEP);
        graph->SetPointError(iPoint, 0, FEP * 1.0/sqrt(nFEP));
        
        iPoint += 1;
    }

    c->Print(plotName + "]", "pdf"); // close
    
    graph->SetTitle("; Gamma Energy (MeV); Full Energy Peak Efficiency (for single gamma)");
    graph->SetLineColor(kBlue);
    graph->SetMarkerColor(kBlue);
    graph->Draw("AP");
    c->SetLogx(1);
    c->SetLogy(1);
    c->Print("efficiency.pdf");

    TString outFileName = "SimulatedEfficiency.root";
    TFile* outFile = TFile::Open(outFileName.Data(), "UPDATE");
    TString graphName = "SimulatedEfficiency";
    graph->SetName(graphName.Data());
    outFile->cd();
    graph->Write(graphName.Data(), TObject::kOverwrite);
    outFile->Save();
    outFile->Close();
}
